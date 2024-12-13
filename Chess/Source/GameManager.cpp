//
// Created by Andrew Fagan on 11/28/24.
//

#include "GameManager.h"

#include "ECS/ECSManager.h"
#include "Window/Window.h"
#include "Input/InputState.h"
#include "Board/Board.h"
#include "Board/BoardManager.h"
#include "Window/WindowManager.h"
#include "glm/ext/matrix_transform.hpp"
#include "Renderer/RendererCommands.h"
#include "Conversions.h"

#include "ECS/Systems/RenderingSystem.h"
#include "EntityManager.h"

namespace Game {

    GameManager& g_GameManager = GameManager::Get();

    GameManager& GameManager::Get()
    {
        static GameManager s_GameManager;
        return s_GameManager;
    }


    void GameManager::Init()
    {
        TRACE("Initializing Game Manager");
        m_UpdateListener.StartListening();

        RendererCommand::SetClearColor(.3,.3,.8,1);

        float vertices[20] = {
                -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
                0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
                -0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
                0.5f,  0.5f, 0.0f, 1.0f, 1.0f
        };


        m_ShaderProgram.reset(ShaderProgram::CreateShaderProgram(ShaderSource(std::string(SHADER_DIR) + "basic.vert")
                , ShaderSource(std::string(SHADER_DIR) + "basic.frag")));
        m_ShaderProgram->Bind();

        uint32 indices[6] = { 0, 1, 2, 1, 3, 2};

        BufferLayout bufferLayout = {
                {Float3, "a_Position"},
                {Float2, "a_TexCords"}
        };

        m_VertexBuffer.reset(VertexBuffer::CreateVertexBuffer(vertices, sizeof(vertices), bufferLayout));
        m_VAO.reset(VertexArrayObject::CreateVertexArrayObject());
        m_VAO->Bind();

        m_VAO->AddVertexBuffer(m_VertexBuffer.get());

        m_Texture.reset(Texture::CreateTexture(std::string(SHADER_DIR) + "water.jpeg"));
        m_Texture->Bind();

        m_ShaderProgram->SetTextureUniform("u_Texture", 0);

        m_IndexBuffer.reset(IndexBuffer::CreateIndexBuffer(indices, 6));
        m_VAO->SetIndexBuffer(m_IndexBuffer.get());

        EntityManager::InitEntities(m_VAO.get());

        RendererCommand::SetBlending(true);
    }


    void GameManager::Update()
    {
        RendererCommand::Clear();

        Mat4 transform;

        static bool isMouseDown;
        static uint8 lockedOnPieceID = 255;
        static PieceColor lockedOnPieceColor;
        Game::Board& board = Game::g_BoardManager.GetBoard();
        static ECS::Entity lockedOnEntity;
        static ECS::Entity none = ECS::g_ECSManager.GetECS().AddEntity();
        if (InputState::IsKeyDown(Keycode::KEY_LEFT_CLICK))
        {
            uint8 location = Game::ConvertCoordinatesToPieceLocation({InputState::MousePositionX(), InputState::MousePositionY()});
            if (!isMouseDown)
            {
                if (board.ReadSquareType(location) != PieceType::NONE)
                {
                    lockedOnEntity = EntityManager::GetEntity(location);
                    lockedOnPieceID = board.ReadSquarePieceID(location);
                    lockedOnPieceColor = board.ReadSquareColor(location);
                }
                else
                {
                    lockedOnPieceID = 255;
                }
            }
            isMouseDown = true;
            ECS::ECS& ecs = ECS::g_ECSManager.GetECS();

            TransformComponent& transformComponent = ecs.GetComponent<TransformComponent>(lockedOnEntity);
            transformComponent.x = InputState::MousePositionX();
            transformComponent.y = InputState::MousePositionY();
            ecs.AddComponent<TransformComponent>(lockedOnEntity,
                                                 TransformComponent(transformComponent));

        }
        else
        {
            if (isMouseDown)
            {
                if (lockedOnPieceID != 255)
                {
                    uint8 location = Game::ConvertCoordinatesToPieceLocation({InputState::MousePositionX(), InputState::MousePositionY()});
                    if (board.ReadSquareType(location) == PieceType::NONE)
                    {
                        board.MovePiece(lockedOnPieceColor, (PieceID)lockedOnPieceID, location);
                    }
                    else if (board.ReadSquareColor(location) != lockedOnPieceColor)
                    {
                        ECS::ECS& ecs = ECS::g_ECSManager.GetECS();

                        SpriteComponent& pieceSprite = ecs.GetComponent<SpriteComponent>(EntityManager::GetEntity(location));
                        pieceSprite.isUsed = false;
                        board.TakePiece(lockedOnPieceColor, (PieceID)lockedOnPieceID, location);

                    }

                    uint8 pieceLocation = board.ReadPieceLocation(lockedOnPieceColor, (PieceID)lockedOnPieceID);
                    Vec2 thing = Game::ConvertPieceLocationToCoordinates(pieceLocation);
                    LOG(thing.x << " " << thing.y);

                    ECS::ECS& ecs = ECS::g_ECSManager.GetECS();
                    TransformComponent& transformComponent = ecs.GetComponent<TransformComponent>(lockedOnEntity);
                    transformComponent.x = thing.x;
                    transformComponent.y = thing.y;
                    ecs.AddComponent<TransformComponent>(lockedOnEntity,
                                                         TransformComponent(transformComponent));
                }
                isMouseDown = false;
                lockedOnEntity = none;

            }
        }

        m_Texture->Bind(0);
        Renderer::Submit(*m_ShaderProgram, m_VAO.get(), transform);

        Sozin::RenderingSystem::RenderEntities(m_ShaderProgram.get());
    }


    void GameManager::Shutdown()
    {
        TRACE("Shutting down Game Manager");
        m_UpdateListener.StopListening();

        m_ShaderProgram.reset(nullptr);
        m_IndexBuffer.reset(nullptr);
        m_VertexBuffer.reset(nullptr);
        m_VAO.reset(nullptr);
        m_Texture.reset(nullptr);
    }


    GameManager::GameManager()
    {
        TRACE("Constructing Game Manager");
    }


    GameManager::~GameManager()
    {
        TRACE("Destroying Game Manager");
    }

}