//
// Created by Andrew Fagan on 12/9/24.
//

#include "RenderStuff.h"
#include "Window/Window.h"
#include "Input/InputState.h"
#include "Game/Board/Board.h"
#include "Game/Board/BoardManager.h"
#include "Window/WindowManager.h"
#include "glm/ext/matrix_transform.hpp"
#include "Renderer/RendererCommands.h"
#include "Conversions.h"

#include "ECS/Systems/RenderingSystem.h"

namespace Renderer {

    void RenderStuff::InitEntities()
    {

        ECS::ECS& ecs = ECS::g_ECSManager.GetECS();

        m_ChessBoard = ecs.AddEntity();

        m_BlackPawn1 = ecs.AddEntity();
        m_BlackPawn2 = ecs.AddEntity();
        m_BlackPawn3 = ecs.AddEntity();
        m_BlackPawn4 = ecs.AddEntity();
        m_BlackPawn5 = ecs.AddEntity();
        m_BlackPawn6 = ecs.AddEntity();
        m_BlackPawn7 = ecs.AddEntity();
        m_BlackPawn8 = ecs.AddEntity();
        m_BlackRook1 = ecs.AddEntity();
        m_BlackRook2 = ecs.AddEntity();
        m_BlackKnight1 = ecs.AddEntity();
        m_BlackKnight2 = ecs.AddEntity();
        m_BlackBishop1 = ecs.AddEntity();
        m_BlackBishop2 = ecs.AddEntity();
        m_BlackQueen = ecs.AddEntity();
        m_BlackKing = ecs.AddEntity();

        m_WhitePawn1 = ecs.AddEntity();
        m_WhitePawn2 = ecs.AddEntity();
        m_WhitePawn3 = ecs.AddEntity();
        m_WhitePawn4 = ecs.AddEntity();
        m_WhitePawn5 = ecs.AddEntity();
        m_WhitePawn6 = ecs.AddEntity();
        m_WhitePawn7 = ecs.AddEntity();
        m_WhitePawn8 = ecs.AddEntity();
        m_WhiteRook1 = ecs.AddEntity();
        m_WhiteRook2 = ecs.AddEntity();
        m_WhiteKnight1 = ecs.AddEntity();
        m_WhiteKnight2 = ecs.AddEntity();
        m_WhiteBishop1 = ecs.AddEntity();
        m_WhiteBishop2 = ecs.AddEntity();
        m_WhiteQueen = ecs.AddEntity();
        m_WhiteKing = ecs.AddEntity();


        ecs.AddComponent(m_ChessBoard, TransformComponent(Vec3(400.0f), Vec3(2.0f)));
        ecs.AddComponent(m_ChessBoard, SpriteComponent(Texture::CreateTexture(std::string(ROOT_DIR) + "/Chess/Source/Resources/chessboard.png"), m_VAO.get()));

        ecs.AddComponent(m_BlackPawn1, TransformComponent(50, 650, .25, .25));
        ecs.AddComponent(m_BlackPawn1, SpriteComponent(Texture::CreateTexture(std::string(ROOT_DIR) + "/Chess/Source/Resources/black_pawn.png"), m_VAO.get()));
        ecs.AddComponent(m_BlackPawn2, TransformComponent(150, 650, .25, .25));
        ecs.AddComponent(m_BlackPawn2, SpriteComponent(Texture::CreateTexture(std::string(ROOT_DIR) + "/Chess/Source/Resources/black_pawn.png"), m_VAO.get()));
        ecs.AddComponent(m_BlackPawn3, TransformComponent(250, 650, .25, .25));
        ecs.AddComponent(m_BlackPawn3, SpriteComponent(Texture::CreateTexture(std::string(ROOT_DIR) + "/Chess/Source/Resources/black_pawn.png"), m_VAO.get()));
        ecs.AddComponent(m_BlackPawn4, TransformComponent(350, 650, .25, .25));
        ecs.AddComponent(m_BlackPawn4, SpriteComponent(Texture::CreateTexture(std::string(ROOT_DIR) + "/Chess/Source/Resources/black_pawn.png"), m_VAO.get()));
        ecs.AddComponent(m_BlackPawn5, TransformComponent(450, 650, .25, .25));
        ecs.AddComponent(m_BlackPawn5, SpriteComponent(Texture::CreateTexture(std::string(ROOT_DIR) + "/Chess/Source/Resources/black_pawn.png"), m_VAO.get()));
        ecs.AddComponent(m_BlackPawn6, TransformComponent(550, 650, .25, .25));
        ecs.AddComponent(m_BlackPawn6, SpriteComponent(Texture::CreateTexture(std::string(ROOT_DIR) + "/Chess/Source/Resources/black_pawn.png"), m_VAO.get()));
        ecs.AddComponent(m_BlackPawn7, TransformComponent(650, 650, .25, .25));
        ecs.AddComponent(m_BlackPawn7, SpriteComponent(Texture::CreateTexture(std::string(ROOT_DIR) + "/Chess/Source/Resources/black_pawn.png"), m_VAO.get()));
        ecs.AddComponent(m_BlackPawn8, TransformComponent(750, 650, .25, .25));
        ecs.AddComponent(m_BlackPawn8, SpriteComponent(Texture::CreateTexture(std::string(ROOT_DIR) + "/Chess/Source/Resources/black_pawn.png"), m_VAO.get()));

        ecs.AddComponent(m_BlackRook1, TransformComponent(50, 750, .25, .25));
        ecs.AddComponent(m_BlackRook1, SpriteComponent(Texture::CreateTexture(std::string(ROOT_DIR) + "/Chess/Source/Resources/black_rook.png"), m_VAO.get()));
        ecs.AddComponent(m_BlackRook2, TransformComponent(750, 750, .25, .25));
        ecs.AddComponent(m_BlackRook2, SpriteComponent(Texture::CreateTexture(std::string(ROOT_DIR) + "/Chess/Source/Resources/black_rook.png"), m_VAO.get()));

        ecs.AddComponent(m_BlackKnight1, TransformComponent(150, 750, .25, .25));
        ecs.AddComponent(m_BlackKnight1, SpriteComponent(Texture::CreateTexture(std::string(ROOT_DIR) + "/Chess/Source/Resources/black_knight.png"), m_VAO.get()));
        ecs.AddComponent(m_BlackKnight2, TransformComponent(650, 750, .25, .25));
        ecs.AddComponent(m_BlackKnight2, SpriteComponent(Texture::CreateTexture(std::string(ROOT_DIR) + "/Chess/Source/Resources/black_knight.png"), m_VAO.get()));

        ecs.AddComponent(m_BlackBishop1, TransformComponent(250, 750, .25, .25));
        ecs.AddComponent(m_BlackBishop1, SpriteComponent(Texture::CreateTexture(std::string(ROOT_DIR) + "/Chess/Source/Resources/black_bishop.png"), m_VAO.get()));
        ecs.AddComponent(m_BlackBishop2, TransformComponent(550, 750, .25, .25));
        ecs.AddComponent(m_BlackBishop2, SpriteComponent(Texture::CreateTexture(std::string(ROOT_DIR) + "/Chess/Source/Resources/black_bishop.png"), m_VAO.get()));

        ecs.AddComponent(m_BlackQueen, TransformComponent(350, 750, .25, .25));
        ecs.AddComponent(m_BlackQueen, SpriteComponent(Texture::CreateTexture(std::string(ROOT_DIR) + "/Chess/Source/Resources/black_queen.png"), m_VAO.get()));

        ecs.AddComponent(m_BlackKing, TransformComponent(450, 750, .25, .25));
        ecs.AddComponent(m_BlackKing, SpriteComponent(Texture::CreateTexture(std::string(ROOT_DIR) + "/Chess/Source/Resources/black_king.png"), m_VAO.get()));

        ecs.AddComponent(m_WhitePawn1, TransformComponent(50, 150, .25, .25));
        ecs.AddComponent(m_WhitePawn1, SpriteComponent(Texture::CreateTexture(std::string(ROOT_DIR) + "/Chess/Source/Resources/white_pawn.png"), m_VAO.get()));
        ecs.AddComponent(m_WhitePawn2, TransformComponent(150, 150, .25, .25));
        ecs.AddComponent(m_WhitePawn2, SpriteComponent(Texture::CreateTexture(std::string(ROOT_DIR) + "/Chess/Source/Resources/white_pawn.png"), m_VAO.get()));
        ecs.AddComponent(m_WhitePawn3, TransformComponent(250, 150, .25, .25));
        ecs.AddComponent(m_WhitePawn3, SpriteComponent(Texture::CreateTexture(std::string(ROOT_DIR) + "/Chess/Source/Resources/white_pawn.png"), m_VAO.get()));
        ecs.AddComponent(m_WhitePawn4, TransformComponent(350, 150, .25, .25));
        ecs.AddComponent(m_WhitePawn4, SpriteComponent(Texture::CreateTexture(std::string(ROOT_DIR) + "/Chess/Source/Resources/white_pawn.png"), m_VAO.get()));
        ecs.AddComponent(m_WhitePawn5, TransformComponent(450, 150, .25, .25));
        ecs.AddComponent(m_WhitePawn5, SpriteComponent(Texture::CreateTexture(std::string(ROOT_DIR) + "/Chess/Source/Resources/white_pawn.png"), m_VAO.get()));
        ecs.AddComponent(m_WhitePawn6, TransformComponent(550, 150, .25, .25));
        ecs.AddComponent(m_WhitePawn6, SpriteComponent(Texture::CreateTexture(std::string(ROOT_DIR) + "/Chess/Source/Resources/white_pawn.png"), m_VAO.get()));
        ecs.AddComponent(m_WhitePawn7, TransformComponent(650, 150, .25, .25));
        ecs.AddComponent(m_WhitePawn7, SpriteComponent(Texture::CreateTexture(std::string(ROOT_DIR) + "/Chess/Source/Resources/white_pawn.png"), m_VAO.get()));
        ecs.AddComponent(m_WhitePawn8, TransformComponent(750, 150, .25, .25));
        ecs.AddComponent(m_WhitePawn8, SpriteComponent(Texture::CreateTexture(std::string(ROOT_DIR) + "/Chess/Source/Resources/white_pawn.png"), m_VAO.get()));

        ecs.AddComponent(m_WhiteRook1, TransformComponent(50, 50, .25, .25));
        ecs.AddComponent(m_WhiteRook1, SpriteComponent(Texture::CreateTexture(std::string(ROOT_DIR) + "/Chess/Source/Resources/white_rook.png"), m_VAO.get()));
        ecs.AddComponent(m_WhiteRook2, TransformComponent(750, 50, .25, .25));
        ecs.AddComponent(m_WhiteRook2, SpriteComponent(Texture::CreateTexture(std::string(ROOT_DIR) + "/Chess/Source/Resources/white_rook.png"), m_VAO.get()));

        ecs.AddComponent(m_WhiteKnight1, TransformComponent(150, 50, .25, .25));
        ecs.AddComponent(m_WhiteKnight1, SpriteComponent(Texture::CreateTexture(std::string(ROOT_DIR) + "/Chess/Source/Resources/white_knight.png"), m_VAO.get()));
        ecs.AddComponent(m_WhiteKnight2, TransformComponent(650, 50, .25, .25));
        ecs.AddComponent(m_WhiteKnight2, SpriteComponent(Texture::CreateTexture(std::string(ROOT_DIR) + "/Chess/Source/Resources/white_knight.png"), m_VAO.get()));

        ecs.AddComponent(m_WhiteBishop1, TransformComponent(250, 50, .25, .25));
        ecs.AddComponent(m_WhiteBishop1, SpriteComponent(Texture::CreateTexture(std::string(ROOT_DIR) + "/Chess/Source/Resources/white_bishop.png"), m_VAO.get()));
        ecs.AddComponent(m_WhiteBishop2, TransformComponent(550, 50, .25, .25));
        ecs.AddComponent(m_WhiteBishop2, SpriteComponent(Texture::CreateTexture(std::string(ROOT_DIR) + "/Chess/Source/Resources/white_bishop.png"), m_VAO.get()));

        ecs.AddComponent(m_WhiteQueen, TransformComponent(350, 50, .25, .25));
        ecs.AddComponent(m_WhiteQueen, SpriteComponent(Texture::CreateTexture(std::string(ROOT_DIR) + "/Chess/Source/Resources/white_queen.png"), m_VAO.get()));

        ecs.AddComponent(m_WhiteKing, TransformComponent(450, 50, .25, .25));
        ecs.AddComponent(m_WhiteKing, SpriteComponent(Texture::CreateTexture(std::string(ROOT_DIR) + "/Chess/Source/Resources/white_king.png"), m_VAO.get()));

        ecs.AddComponent(m_ChessBoard, TransformComponent(Vec3(400.0f), Vec3(2.0f)));
        ecs.AddComponent(m_ChessBoard, SpriteComponent(Texture::CreateTexture(std::string(ROOT_DIR) + "/Chess/Source/Resources/chessboard.png"), m_VAO.get()));
    }


    Mat4 RenderStuff::GetMouseTransform(Vec3 scale)
    {
        Window::Window& window = Window::g_WindowManager.GetWindow();

        Mat4 pieceScale = glm::scale(Mat4(1.0f), scale);
        Vec3 position = Vec3(1.0f);

        position.x = InputState::MousePositionX() / window.GetWidth() * 2 - 1;
        position.y = InputState::MousePositionY() / window.GetHeight() * 2 - 1;

        if (InputState::IsKeyDown(KEY_R))
        {
            position.x = .1;
            position.y = .1;
        }

        return glm::translate(Mat4(1.0f), position) * pieceScale;
    }


    ECS::Entity RenderStuff::GetEntity(uint8_t location)
    {
        Game::Board& board = Game::g_BoardManager.GetBoard();
        if (board.ReadSquareType(location) == PieceType::NONE)
        {
            return m_WhiteKing;
        }

        PieceColor color = board.ReadSquareColor(location);
        uint8 pieceID = board.ReadSquarePieceID(location);

        if (color == PieceColor::BLACK)
        {
            switch (pieceID)
            {
                case 0: return m_BlackPawn1;
                case 1: return m_BlackPawn2;
                case 2: return m_BlackPawn3;
                case 3: return m_BlackPawn4;
                case 4: return m_BlackPawn5;
                case 5: return m_BlackPawn6;
                case 6: return m_BlackPawn7;
                case 7: return m_BlackPawn8;
                case 8: return m_BlackRook1;
                case 9: return m_BlackKnight1;
                case 10: return m_BlackBishop1;
                case 11: return m_BlackQueen;
                case 12: return m_BlackKing;
                case 13: return m_BlackBishop2;
                case 14: return m_BlackKnight2;
                case 15: return m_BlackRook2;
                default: return m_WhiteKing;
            }
        }
        else
        {
            switch (pieceID)
            {
                case 0: return m_WhitePawn1;
                case 1: return m_WhitePawn2;
                case 2: return m_WhitePawn3;
                case 3: return m_WhitePawn4;
                case 4: return m_WhitePawn5;
                case 5: return m_WhitePawn6;
                case 6: return m_WhitePawn7;
                case 7: return m_WhitePawn8;
                case 8: return m_WhiteRook1;
                case 9: return m_WhiteKnight1;
                case 10: return m_WhiteBishop1;
                case 11: return m_WhiteQueen;
                case 12: return m_WhiteKing;
                case 13: return m_WhiteBishop2;
                case 14: return m_WhiteKnight2;
                case 15: return m_WhiteRook2;
                default: return m_WhiteKing;
            }
        }
    }


    void RenderStuff::Init()
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

        m_Texture.reset(Texture::CreateTexture(std::string(SHADER_DIR) + "water.jpeg"));
        m_Texture->Bind();

        m_ShaderProgram->SetTextureUniform("u_Texture", 0);

        m_IndexBuffer.reset(IndexBuffer::CreateIndexBuffer(indices, 6));
        m_VAO->SetIndexBuffer(m_IndexBuffer.get());

        InitEntities();

        RendererCommand::SetBlending(true);
    }


    void RenderStuff::Update()
    {RendererCommand::Clear();

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
                    lockedOnEntity = GetEntity(location);
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
            if (lockedOnEntity.GetID() != m_ChessBoard.GetID()) // this makes sure the chessboard doesn't move
            {
                TransformComponent &transformComponent = ecs.GetComponent<TransformComponent>(lockedOnEntity);
                transformComponent.x = InputState::MousePositionX();
                transformComponent.y = InputState::MousePositionY();
                ecs.AddComponent<TransformComponent>(lockedOnEntity,
                                                     TransformComponent(transformComponent));
            }
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

                        SpriteComponent& pieceSprite = ecs.GetComponent<SpriteComponent>(GetEntity(location));
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


//        Mat4 chessboardTransform = glm::translate(Mat4(1.0f), Vec3(0.0f)) * glm::scale(Mat4(1.0f), Vec3(2.0f));
//
//        static Texture* chessboardTexture = Texture::CreateTexture(std::string(ROOT_DIR) + "/Chess/Source/Resources/chessboard.png");
//        chessboardTexture->Bind(0);
//
//        Renderer::Submit(*m_ShaderProgram, m_VAO.get(), chessboardTransform);

        m_Texture->Bind(0);
        Renderer::Submit(*m_ShaderProgram, m_VAO.get(), transform);


        Sozin::RenderingSystem::RenderEntities(m_ShaderProgram.get());
    }


    void RenderStuff::Shutdown()
    {
        m_ShaderProgram.reset(nullptr);
        m_IndexBuffer.reset(nullptr);
        m_VertexBuffer.reset(nullptr);
        m_VAO.reset(nullptr);
        m_Texture.reset(nullptr);
    }


}