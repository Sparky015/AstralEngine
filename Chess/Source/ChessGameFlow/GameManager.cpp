//
// Created by Andrew Fagan on 11/28/24.
//

#include "GameManager.h"

#include "ECS/ECSManager.h"
#include "Window/Window.h"
#include "Input/InputState.h"
#include "ChessBoard/ChessBoard.h"
#include "ChessBoard/ChessBoardManager.h"
#include "glm/ext/matrix_transform.hpp"
#include "Renderer/RendererCommands.h"
#include "Utils/ChessConversions.h"

#include "ECS/Systems/RenderingSystem.h"
#include "ChessGameState/ChessEntities.h"
#include "ChessLogic/MoveValidation.h"
#include "Window/WindowManager.h"

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

        InitializeRenderingComponents();
        ChessEntities::InitEntities(m_VAO.get());
    }


    void GameManager::Update()
    {
        // Clear screen
        RendererCommand::Clear();

        Game::ChessBoard& board = Game::g_BoardManager.GetBoard();


        // enum PieceTrackingState
        static bool isMouseDown;


        // struct PieceTracked
        // --------------------------------------------------------------------------------------------------------
        static uint8 lockedOnPieceID = 255;
        static PieceColor lockedOnPieceColor;
        static ECS::Entity lockedOnEntity;
        // --------------------------------------------------------------------------------------------------------
        static ECS::Entity none = ECS::g_ECSManager.GetECS().AddEntity();


        // onMouseClick
        // --------------------------------------------------------------------------------------------------------
        // Detect mouse clicks and track piece
        if (InputState::IsKeyDown(Keycode::KEY_LEFT_CLICK))
        {
            uint8 location = Game::ConvertCoordinatesToPieceLocation({InputState::MousePositionX(), InputState::MousePositionY()});
            if (!isMouseDown)
            {
                if (board.ReadSquareType(location) != PieceType::NONE)
                {
                    // Get info of piece clicked on
                    lockedOnEntity = ChessEntities::GetEntity(location);
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


            // Place the piece being tracked onto the mouse coordinates
            TransformComponent& transformComponent = ecs.GetComponent<TransformComponent>(lockedOnEntity);
            transformComponent.x = InputState::MousePositionX();
            transformComponent.y = InputState::MousePositionY();
            ecs.AddComponent<TransformComponent>(lockedOnEntity,
                                                 TransformComponent(transformComponent));
        } // --------------------------------------------------------------------------------------------------------


        // onMouseRelease
        // --------------------------------------------------------------------------------------------------------
        else
        {
            if (isMouseDown)
            {
                // On mouse release
                if (lockedOnPieceID != 255)
                {



                    // --------------------------------------------------------------------------------------------------------
                    // Find the square the mouse is over
                    uint8 location = Game::ConvertCoordinatesToPieceLocation({InputState::MousePositionX(), InputState::MousePositionY()});
                    if (board.ReadSquareType(location) == PieceType::NONE)
                    {
                        Game::ChessBoardManager& boardManager = Game::g_BoardManager;

                        // Move piece to the square the mouse was released over
                        ChessMove chessMove = ChessMove((PieceID)lockedOnPieceID, lockedOnPieceColor, location, MoveType::REGULAR);
                        if (IsMoveValid(boardManager.GetBoard(), boardManager.GetMoveList(), chessMove))
                        {
                            board.MovePiece(lockedOnPieceColor, (PieceID)lockedOnPieceID, location);
                            g_BoardManager.GetMoveList().GenerateMoves(board, board.GetActiveColor());
                        }
                    }
                    else if (board.ReadSquareColor(location) != lockedOnPieceColor)
                    {
                        ECS::ECS& ecs = ECS::g_ECSManager.GetECS();

                        // Perform take with piece to the square the mouse was released over
                        SpriteComponent& pieceSprite = ecs.GetComponent<SpriteComponent>(ChessEntities::GetEntity(location));
                        pieceSprite.isUsed = false;

                        Game::ChessBoardManager& boardManager = Game::g_BoardManager;
                        ChessMove chessMove = ChessMove((PieceID)lockedOnPieceID, lockedOnPieceColor, location, MoveType::TAKE);
                        if (IsMoveValid(boardManager.GetBoard(), boardManager.GetMoveList(), chessMove))
                        {
                            board.TakePiece(lockedOnPieceColor, (PieceID)lockedOnPieceID, location);
                            g_BoardManager.GetMoveList().GenerateMoves(board, board.GetActiveColor());
                        }

                    }
                    // --------------------------------------------------------------------------------------------------------



                    // Transform GetSquareTransform()
                    // --------------------------------------------------------------------------------------------------------
                    // Place the piece transform back to the square it was released over
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
                // --------------------------------------------------------------------------------------------------------


        // --------------------------------------------------------------------------------------------------------

                // Reset the mouse and entity
                isMouseDown = false;
                lockedOnEntity = none;

            }
        }

//        m_Texture->Bind(0);
//        Renderer::Submit(*m_ShaderProgram, m_VAO.get(), transform);

        // Render Entities
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


    void GameManager::InitializeRenderingComponents()
    {


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

//        m_Texture.reset(Texture::CreateTexture(std::string(SHADER_DIR) + "water.jpeg"));
//        m_Texture->Bind();

        m_ShaderProgram->SetTextureUniform("u_Texture", 0);

        m_IndexBuffer.reset(IndexBuffer::CreateIndexBuffer(indices, 6));
        m_VAO->SetIndexBuffer(m_IndexBuffer.get());

        RendererCommand::SetBlending(true);

    }



    struct GameManager::TrackedPiece
    {
        TrackedPiece() : pieceID(0), color(PieceColor::BLACK), entity(ECS::Entity()) {}

        uint8 pieceID;
        PieceColor color{PieceColor::BLACK};
        ECS::Entity entity;
    };


    void GameManager::TrackPiece(GameManager::TrackedPiece& outTrackedPiece)
    {

    }

}
