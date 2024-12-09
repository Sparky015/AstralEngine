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
#include "../../ThirdPartyLibraries/GLAD/glad/glad.h"
#include "Renderer/RendererCommands.h"
#include "Input/Conversions.h"

namespace Renderer {

    void RenderStuff::InitEntities()
    {

        ECS::ECS& ecs = ECS::g_ECSManager.GetECS();

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

        m_ChessBoard = ecs.AddEntity();

    // Add all black pawns
        ecs.AddComponent(m_BlackPawn1, TransformComponent(50, 650));
        ecs.AddComponent(m_BlackPawn1, SpriteComponent(Texture::CreateTexture(std::string(ROOT_DIR) + "/Chess/Source/Resources/black_pawn.png")));
        ecs.AddComponent(m_BlackPawn2, TransformComponent(150, 650));
        ecs.AddComponent(m_BlackPawn2, SpriteComponent(Texture::CreateTexture(std::string(ROOT_DIR) + "/Chess/Source/Resources/black_pawn.png")));
        ecs.AddComponent(m_BlackPawn3, TransformComponent(250, 650));
        ecs.AddComponent(m_BlackPawn3, SpriteComponent(Texture::CreateTexture(std::string(ROOT_DIR) + "/Chess/Source/Resources/black_pawn.png")));
        ecs.AddComponent(m_BlackPawn4, TransformComponent(350, 650));
        ecs.AddComponent(m_BlackPawn4, SpriteComponent(Texture::CreateTexture(std::string(ROOT_DIR) + "/Chess/Source/Resources/black_pawn.png")));
        ecs.AddComponent(m_BlackPawn5, TransformComponent(450, 650));
        ecs.AddComponent(m_BlackPawn5, SpriteComponent(Texture::CreateTexture(std::string(ROOT_DIR) + "/Chess/Source/Resources/black_pawn.png")));
        ecs.AddComponent(m_BlackPawn6, TransformComponent(550, 650));
        ecs.AddComponent(m_BlackPawn6, SpriteComponent(Texture::CreateTexture(std::string(ROOT_DIR) + "/Chess/Source/Resources/black_pawn.png")));
        ecs.AddComponent(m_BlackPawn7, TransformComponent(650, 650));
        ecs.AddComponent(m_BlackPawn7, SpriteComponent(Texture::CreateTexture(std::string(ROOT_DIR) + "/Chess/Source/Resources/black_pawn.png")));
        ecs.AddComponent(m_BlackPawn8, TransformComponent(750, 650));
        ecs.AddComponent(m_BlackPawn8, SpriteComponent(Texture::CreateTexture(std::string(ROOT_DIR) + "/Chess/Source/Resources/black_pawn.png")));

        ecs.AddComponent(m_BlackRook1, TransformComponent(50, 750));
        ecs.AddComponent(m_BlackRook1, SpriteComponent(Texture::CreateTexture(std::string(ROOT_DIR) + "/Chess/Source/Resources/black_rook.png")));
        ecs.AddComponent(m_BlackRook2, TransformComponent(750, 750));
        ecs.AddComponent(m_BlackRook2, SpriteComponent(Texture::CreateTexture(std::string(ROOT_DIR) + "/Chess/Source/Resources/black_rook.png")));

        ecs.AddComponent(m_BlackKnight1, TransformComponent(150, 750));
        ecs.AddComponent(m_BlackKnight1, SpriteComponent(Texture::CreateTexture(std::string(ROOT_DIR) + "/Chess/Source/Resources/black_knight.png")));
        ecs.AddComponent(m_BlackKnight2, TransformComponent(650, 750));
        ecs.AddComponent(m_BlackKnight2, SpriteComponent(Texture::CreateTexture(std::string(ROOT_DIR) + "/Chess/Source/Resources/black_knight.png")));

        ecs.AddComponent(m_BlackBishop1, TransformComponent(250, 750));
        ecs.AddComponent(m_BlackBishop1, SpriteComponent(Texture::CreateTexture(std::string(ROOT_DIR) + "/Chess/Source/Resources/black_bishop.png")));
        ecs.AddComponent(m_BlackBishop2, TransformComponent(550, 750));
        ecs.AddComponent(m_BlackBishop2, SpriteComponent(Texture::CreateTexture(std::string(ROOT_DIR) + "/Chess/Source/Resources/black_bishop.png")));

        ecs.AddComponent(m_BlackQueen, TransformComponent(350, 750));
        ecs.AddComponent(m_BlackQueen, SpriteComponent(Texture::CreateTexture(std::string(ROOT_DIR) + "/Chess/Source/Resources/black_queen.png")));

        ecs.AddComponent(m_BlackKing, TransformComponent(450, 750));
        ecs.AddComponent(m_BlackKing, SpriteComponent(Texture::CreateTexture(std::string(ROOT_DIR) + "/Chess/Source/Resources/black_king.png")));

        // Add all white pawns
        ecs.AddComponent(m_WhitePawn1, TransformComponent(50, 150));
        ecs.AddComponent(m_WhitePawn1, SpriteComponent(Texture::CreateTexture(std::string(ROOT_DIR) + "/Chess/Source/Resources/white_pawn.png")));
        ecs.AddComponent(m_WhitePawn2, TransformComponent(150, 150));
        ecs.AddComponent(m_WhitePawn2, SpriteComponent(Texture::CreateTexture(std::string(ROOT_DIR) + "/Chess/Source/Resources/me_white_pawn.png")));
        ecs.AddComponent(m_WhitePawn3, TransformComponent(250, 150));
        ecs.AddComponent(m_WhitePawn3, SpriteComponent(Texture::CreateTexture(std::string(ROOT_DIR) + "/Chess/Source/Resources/white_pawn.png")));
        ecs.AddComponent(m_WhitePawn4, TransformComponent(350, 150));
        ecs.AddComponent(m_WhitePawn4, SpriteComponent(Texture::CreateTexture(std::string(ROOT_DIR) + "/Chess/Source/Resources/white_pawn.png")));
        ecs.AddComponent(m_WhitePawn5, TransformComponent(450, 150));
        ecs.AddComponent(m_WhitePawn5, SpriteComponent(Texture::CreateTexture(std::string(ROOT_DIR) + "/Chess/Source/Resources/white_pawn.png")));
        ecs.AddComponent(m_WhitePawn6, TransformComponent(550, 150));
        ecs.AddComponent(m_WhitePawn6, SpriteComponent(Texture::CreateTexture(std::string(ROOT_DIR) + "/Chess/Source/Resources/white_pawn.png")));
        ecs.AddComponent(m_WhitePawn7, TransformComponent(650, 150));
        ecs.AddComponent(m_WhitePawn7, SpriteComponent(Texture::CreateTexture(std::string(ROOT_DIR) + "/Chess/Source/Resources/white_pawn.png")));
        ecs.AddComponent(m_WhitePawn8, TransformComponent(750, 150));
        ecs.AddComponent(m_WhitePawn8, SpriteComponent(Texture::CreateTexture(std::string(ROOT_DIR) + "/Chess/Source/Resources/white_pawn.png")));

        ecs.AddComponent(m_WhiteRook1, TransformComponent(50, 50));
        ecs.AddComponent(m_WhiteRook1, SpriteComponent(Texture::CreateTexture(std::string(ROOT_DIR) + "/Chess/Source/Resources/white_rook.png")));
        ecs.AddComponent(m_WhiteRook2, TransformComponent(750, 50));
        ecs.AddComponent(m_WhiteRook2, SpriteComponent(Texture::CreateTexture(std::string(ROOT_DIR) + "/Chess/Source/Resources/white_rook.png")));

        ecs.AddComponent(m_WhiteKnight1, TransformComponent(150, 50));
        ecs.AddComponent(m_WhiteKnight1, SpriteComponent(Texture::CreateTexture(std::string(ROOT_DIR) + "/Chess/Source/Resources/white_knight.png")));
        ecs.AddComponent(m_WhiteKnight2, TransformComponent(650, 50));
        ecs.AddComponent(m_WhiteKnight2, SpriteComponent(Texture::CreateTexture(std::string(ROOT_DIR) + "/Chess/Source/Resources/white_knight.png")));

        ecs.AddComponent(m_WhiteBishop1, TransformComponent(250, 50));
        ecs.AddComponent(m_WhiteBishop1, SpriteComponent(Texture::CreateTexture(std::string(ROOT_DIR) + "/Chess/Source/Resources/white_bishop.png")));
        ecs.AddComponent(m_WhiteBishop2, TransformComponent(550, 50));
        ecs.AddComponent(m_WhiteBishop2, SpriteComponent(Texture::CreateTexture(std::string(ROOT_DIR) + "/Chess/Source/Resources/white_bishop.png")));

        ecs.AddComponent(m_WhiteQueen, TransformComponent(350, 50));
        ecs.AddComponent(m_WhiteQueen, SpriteComponent(Texture::CreateTexture(std::string(ROOT_DIR) + "/Chess/Source/Resources/white_queen.png")));

        ecs.AddComponent(m_WhiteKing, TransformComponent(450, 50));
        ecs.AddComponent(m_WhiteKing, SpriteComponent(Texture::CreateTexture(std::string(ROOT_DIR) + "/Chess/Source/Resources/white_king.png")));
    }


    void RenderStuff::SubmitEntitySystem(ECS::Entity entity)
    {
        ECS::ECS& ecs = ECS::g_ECSManager.GetECS();

        TransformComponent pieceTransform = ecs.GetComponent<TransformComponent>(entity);
        SpriteComponent pieceSprite = ecs.GetComponent<SpriteComponent>(entity);
        if (!pieceSprite.isUsed) { return;}
        Vec3 piecePosition = Vec3(pieceTransform.x, pieceTransform.y, 1.0f);
        Mat4 transform = CreateTransform(piecePosition);
        pieceSprite.texture->Bind(0);
        Renderer::Submit(*m_ShaderProgram, m_VAO.get(), transform);
    }


    Mat4 RenderStuff::CreateTransform(Vec3 position)
    {
        Window::Window& window = Window::g_WindowManager.GetWindow();
        static const Mat4 pieceScale = glm::scale(Mat4(1.0f), Vec3(0.25f, .25f, .25f));

        Vec3 normalizedPosition = Vec3(1.0f);
        normalizedPosition.x = position.x / window.GetWidth() * 2 - 1;
        normalizedPosition.y = position.y / window.GetHeight() * 2 - 1;

        Mat4 transform = glm::translate(Mat4(1.0f), normalizedPosition) * pieceScale;
        return transform;
    }


    Mat4 RenderStuff::GetMouseTransform()
    {
        Window::Window& window = Window::g_WindowManager.GetWindow();

        Mat4 pieceScale = glm::scale(Mat4(1.0f), Vec3(0.25f, .25f, .25f));
        Vec3 position = Vec3(1.0f);
        position.x = InputState::MousePositionX();
        position.y = InputState::MousePositionY();
        position.x = position.x / window.GetWidth() * 2 - 1;
        position.y = position.y / window.GetHeight() * 2 - 1;

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

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        InitEntities();
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
            ECS::g_ECSManager.GetECS().AddComponent<TransformComponent>(lockedOnEntity, TransformComponent(InputState::MousePositionX(), InputState::MousePositionY()));
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
                        static int numOfMeMoves = 0;
                        if (lockedOnPieceID == PIECE_2 && lockedOnPieceColor == PieceColor::WHITE)
                        {
                            numOfMeMoves++;
                            ECS::ECS& ecs = ECS::g_ECSManager.GetECS();
                            switch (numOfMeMoves)
                            {
                                case 1: ecs.AddComponent<SpriteComponent>
                                            (lockedOnEntity, SpriteComponent(Texture::CreateTexture(
                                                    std::string(ROOT_DIR) + "/Chess/Source/Resources/me_white_knight.png")));
                                    break;
                                    case 2: case 3: ecs.AddComponent<SpriteComponent>
                                            (lockedOnEntity, SpriteComponent(Texture::CreateTexture(
                                                    std::string(ROOT_DIR) + "/Chess/Source/Resources/me_white_bishop.png")));
                                    break;
                                case 4: ecs.AddComponent<SpriteComponent>
                                            (lockedOnEntity, SpriteComponent(Texture::CreateTexture(
                                                    std::string(ROOT_DIR) + "/Chess/Source/Resources/me_white_queen.png")));
                                    break;
                                case 5: ecs.AddComponent<SpriteComponent>
                                            (lockedOnEntity, SpriteComponent(Texture::CreateTexture(
                                                    std::string(ROOT_DIR) + "/Chess/Source/Resources/me_white_pawn.png")));
                                        numOfMeMoves = 0;
                                    break;
                            }
                        }
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
//                    LOG((int)pieceLocation);
                    Vec2 thing = Game::ConvertPieceLocationToCoordinates(pieceLocation);
                    LOG(thing.x << " " << thing.y);
//                    LOG(Game::ConvertIntToChessNotation(Game::ConvertCoordinatesToPieceLocation(thing)));
                    ECS::g_ECSManager.GetECS().AddComponent<TransformComponent>(lockedOnEntity,
                                                                                TransformComponent(thing.x, thing.y));
                }
                isMouseDown = false;
                lockedOnEntity = none;

            }
        }

        TransformComponent transformComponent = ECS::g_ECSManager.GetECS().GetComponent<TransformComponent>(m_WhiteKing);
        transform = CreateTransform(Vec3(transformComponent.x, transformComponent.y, 1));


        m_ShaderProgram->SetUniform("u_Transform", transform);

        Mat4 chessboardTransform = glm::translate(Mat4(1.0f), Vec3(0.0f)) * glm::scale(Mat4(1.0f), Vec3(2.0f));

        static Texture* chessboardTexture = Texture::CreateTexture(std::string(ROOT_DIR) + "/Chess/Source/Resources/art_chessboard.png");
        chessboardTexture->Bind(0);

        Renderer::Submit(*m_ShaderProgram, m_VAO.get(), chessboardTransform);

//        m_Texture->Bind(0);
//        Renderer::Submit(*m_ShaderProgram, m_VAO.get(), transform);

        SubmitEntitySystem(m_BlackPawn1);
        SubmitEntitySystem(m_BlackPawn2);
        SubmitEntitySystem(m_BlackPawn3);
        SubmitEntitySystem(m_BlackPawn4);
        SubmitEntitySystem(m_BlackPawn5);
        SubmitEntitySystem(m_BlackPawn6);
        SubmitEntitySystem(m_BlackPawn7);
        SubmitEntitySystem(m_BlackPawn8);
        SubmitEntitySystem(m_BlackRook1);
        SubmitEntitySystem(m_BlackRook2);
        SubmitEntitySystem(m_BlackKnight1);
        SubmitEntitySystem(m_BlackKnight2);
        SubmitEntitySystem(m_BlackBishop1);
        SubmitEntitySystem(m_BlackBishop2);
        SubmitEntitySystem(m_BlackQueen);
        SubmitEntitySystem(m_BlackKing);

        SubmitEntitySystem(m_WhitePawn1);
        SubmitEntitySystem(m_WhitePawn2);
        SubmitEntitySystem(m_WhitePawn3);
        SubmitEntitySystem(m_WhitePawn4);
        SubmitEntitySystem(m_WhitePawn5);
        SubmitEntitySystem(m_WhitePawn6);
        SubmitEntitySystem(m_WhitePawn7);
        SubmitEntitySystem(m_WhitePawn8);
        SubmitEntitySystem(m_WhiteRook1);
        SubmitEntitySystem(m_WhiteRook2);
        SubmitEntitySystem(m_WhiteKnight1);
        SubmitEntitySystem(m_WhiteKnight2);
        SubmitEntitySystem(m_WhiteBishop1);
        SubmitEntitySystem(m_WhiteBishop2);
        SubmitEntitySystem(m_WhiteQueen);
        SubmitEntitySystem(m_WhiteKing);
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