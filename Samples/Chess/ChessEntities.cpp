//
// Created by Andrew Fagan on 12/11/24.
//

#include "ChessEntities.h"

#include "ECS/ECSManager.h"
#include "Renderer/Primitives/Texture.h"
#include "Chessboard/Chessboard.h"
#include "Chessboard/ChessboardManager.h"


ECS::Entity ChessEntities::m_BlackPawn1;
ECS::Entity ChessEntities::m_BlackPawn2;
ECS::Entity ChessEntities::m_BlackPawn3;
ECS::Entity ChessEntities::m_BlackPawn4;
ECS::Entity ChessEntities::m_BlackPawn5;
ECS::Entity ChessEntities::m_BlackPawn6;
ECS::Entity ChessEntities::m_BlackPawn7;
ECS::Entity ChessEntities::m_BlackPawn8;
ECS::Entity ChessEntities::m_BlackRook1;
ECS::Entity ChessEntities::m_BlackRook2;
ECS::Entity ChessEntities::m_BlackKnight1;
ECS::Entity ChessEntities::m_BlackKnight2;
ECS::Entity ChessEntities::m_BlackBishop1;
ECS::Entity ChessEntities::m_BlackBishop2;
ECS::Entity ChessEntities::m_BlackQueen;
ECS::Entity ChessEntities::m_BlackKing;

ECS::Entity ChessEntities::m_WhitePawn1;
ECS::Entity ChessEntities::m_WhitePawn2;
ECS::Entity ChessEntities::m_WhitePawn3;
ECS::Entity ChessEntities::m_WhitePawn4;
ECS::Entity ChessEntities::m_WhitePawn5;
ECS::Entity ChessEntities::m_WhitePawn6;
ECS::Entity ChessEntities::m_WhitePawn7;
ECS::Entity ChessEntities::m_WhitePawn8;
ECS::Entity ChessEntities::m_WhiteRook1;
ECS::Entity ChessEntities::m_WhiteRook2;
ECS::Entity ChessEntities::m_WhiteKnight1;
ECS::Entity ChessEntities::m_WhiteKnight2;
ECS::Entity ChessEntities::m_WhiteBishop1;
ECS::Entity ChessEntities::m_WhiteBishop2;
ECS::Entity ChessEntities::m_WhiteQueen;
ECS::Entity ChessEntities::m_WhiteKing;

ECS::Entity ChessEntities::m_ChessBoard;


void ChessEntities::InitEntities(Graphics::VertexArrayObject* vertexArrayObject)
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


    // Transform Components

    ecs.AddComponent(m_ChessBoard,   TransformComponent(Vec3(400.0f), Vec3(2.0f)));
    ecs.AddComponent(m_BlackPawn1,   TransformComponent(50, 650, .25, .25));
    ecs.AddComponent(m_BlackPawn2,   TransformComponent(150, 650, .25, .25));
    ecs.AddComponent(m_BlackPawn3,   TransformComponent(250, 650, .25, .25));
    ecs.AddComponent(m_BlackPawn4,   TransformComponent(350, 650, .25, .25));
    ecs.AddComponent(m_BlackPawn5,   TransformComponent(450, 650, .25, .25));
    ecs.AddComponent(m_BlackPawn6,   TransformComponent(550, 650, .25, .25));
    ecs.AddComponent(m_BlackPawn7,   TransformComponent(650, 650, .25, .25));
    ecs.AddComponent(m_BlackPawn8,   TransformComponent(750, 650, .25, .25));
    ecs.AddComponent(m_BlackRook1,   TransformComponent(50, 750, .25, .25));
    ecs.AddComponent(m_BlackRook2,   TransformComponent(750, 750, .25, .25));
    ecs.AddComponent(m_BlackKnight1, TransformComponent(150, 750, .25, .25));
    ecs.AddComponent(m_BlackKnight2, TransformComponent(650, 750, .25, .25));
    ecs.AddComponent(m_BlackBishop1, TransformComponent(250, 750, .25, .25));
    ecs.AddComponent(m_BlackBishop2, TransformComponent(550, 750, .25, .25));
    ecs.AddComponent(m_BlackQueen,   TransformComponent(350, 750, .25, .25));
    ecs.AddComponent(m_BlackKing,    TransformComponent(450, 750, .25, .25));
    ecs.AddComponent(m_WhitePawn1,   TransformComponent(50, 150, .25, .25));
    ecs.AddComponent(m_WhitePawn2,   TransformComponent(150, 150, .25, .25));
    ecs.AddComponent(m_WhitePawn3,   TransformComponent(250, 150, .25, .25));
    ecs.AddComponent(m_WhitePawn4,   TransformComponent(350, 150, .25, .25));
    ecs.AddComponent(m_WhitePawn5,   TransformComponent(450, 150, .25, .25));
    ecs.AddComponent(m_WhitePawn6,   TransformComponent(550, 150, .25, .25));
    ecs.AddComponent(m_WhitePawn7,   TransformComponent(650, 150, .25, .25));
    ecs.AddComponent(m_WhitePawn8,   TransformComponent(750, 150, .25, .25));
    ecs.AddComponent(m_WhiteRook1,   TransformComponent(50, 50, .25, .25));
    ecs.AddComponent(m_WhiteRook2,   TransformComponent(750, 50, .25, .25));
    ecs.AddComponent(m_WhiteKnight1, TransformComponent(150, 50, .25, .25));
    ecs.AddComponent(m_WhiteKnight2, TransformComponent(650, 50, .25, .25));
    ecs.AddComponent(m_WhiteBishop1, TransformComponent(250, 50, .25, .25));
    ecs.AddComponent(m_WhiteBishop2, TransformComponent(550, 50, .25, .25));
    ecs.AddComponent(m_WhiteQueen,   TransformComponent(350, 50, .25, .25));
    ecs.AddComponent(m_WhiteKing,    TransformComponent(450, 50, .25, .25));


    // Sprite Components
    ecs.AddComponent(m_ChessBoard, SpriteComponent(Graphics::Texture::CreateTexture(std::string(CHESS_ASSET_DIR) + "chessboard.png"), vertexArrayObject));
    ecs.AddComponent(m_BlackPawn1, SpriteComponent(Graphics::Texture::CreateTexture(std::string(CHESS_ASSET_DIR) + "black_pawn.png"), vertexArrayObject));
    ecs.AddComponent(m_BlackPawn2, SpriteComponent(Graphics::Texture::CreateTexture(std::string(CHESS_ASSET_DIR) + "black_pawn.png"), vertexArrayObject));
    ecs.AddComponent(m_BlackPawn3, SpriteComponent(Graphics::Texture::CreateTexture(std::string(CHESS_ASSET_DIR) + "black_pawn.png"), vertexArrayObject));
    ecs.AddComponent(m_BlackPawn4, SpriteComponent(Graphics::Texture::CreateTexture(std::string(CHESS_ASSET_DIR) + "black_pawn.png"), vertexArrayObject));
    ecs.AddComponent(m_BlackPawn5, SpriteComponent(Graphics::Texture::CreateTexture(std::string(CHESS_ASSET_DIR) + "black_pawn.png"), vertexArrayObject));
    ecs.AddComponent(m_BlackPawn6, SpriteComponent(Graphics::Texture::CreateTexture(std::string(CHESS_ASSET_DIR) + "black_pawn.png"), vertexArrayObject));
    ecs.AddComponent(m_BlackPawn7, SpriteComponent(Graphics::Texture::CreateTexture(std::string(CHESS_ASSET_DIR) + "black_pawn.png"), vertexArrayObject));
    ecs.AddComponent(m_BlackPawn8, SpriteComponent(Graphics::Texture::CreateTexture(std::string(CHESS_ASSET_DIR) + "black_pawn.png"), vertexArrayObject));
    ecs.AddComponent(m_BlackRook1, SpriteComponent(Graphics::Texture::CreateTexture(std::string(CHESS_ASSET_DIR) + "black_rook.png"), vertexArrayObject));
    ecs.AddComponent(m_BlackRook2, SpriteComponent(Graphics::Texture::CreateTexture(std::string(CHESS_ASSET_DIR) + "black_rook.png"), vertexArrayObject));
    ecs.AddComponent(m_BlackKnight1, SpriteComponent(Graphics::Texture::CreateTexture(std::string(CHESS_ASSET_DIR) + "black_knight.png"), vertexArrayObject));
    ecs.AddComponent(m_BlackKnight2, SpriteComponent(Graphics::Texture::CreateTexture(std::string(CHESS_ASSET_DIR) + "black_knight.png"), vertexArrayObject));
    ecs.AddComponent(m_BlackBishop1, SpriteComponent(Graphics::Texture::CreateTexture(std::string(CHESS_ASSET_DIR) + "black_bishop.png"), vertexArrayObject));
    ecs.AddComponent(m_BlackBishop2, SpriteComponent(Graphics::Texture::CreateTexture(std::string(CHESS_ASSET_DIR) + "black_bishop.png"), vertexArrayObject));
    ecs.AddComponent(m_BlackQueen, SpriteComponent(Graphics::Texture::CreateTexture(std::string(CHESS_ASSET_DIR) + "black_queen.png"), vertexArrayObject));
    ecs.AddComponent(m_BlackKing, SpriteComponent(Graphics::Texture::CreateTexture(std::string(CHESS_ASSET_DIR) + "black_king.png"), vertexArrayObject));
    ecs.AddComponent(m_WhitePawn1, SpriteComponent(Graphics::Texture::CreateTexture(std::string(CHESS_ASSET_DIR) + "white_pawn.png"), vertexArrayObject));
    ecs.AddComponent(m_WhitePawn2, SpriteComponent(Graphics::Texture::CreateTexture(std::string(CHESS_ASSET_DIR) + "white_pawn.png"), vertexArrayObject));
    ecs.AddComponent(m_WhitePawn3, SpriteComponent(Graphics::Texture::CreateTexture(std::string(CHESS_ASSET_DIR) + "white_pawn.png"), vertexArrayObject));
    ecs.AddComponent(m_WhitePawn4, SpriteComponent(Graphics::Texture::CreateTexture(std::string(CHESS_ASSET_DIR) + "white_pawn.png"), vertexArrayObject));
    ecs.AddComponent(m_WhitePawn5, SpriteComponent(Graphics::Texture::CreateTexture(std::string(CHESS_ASSET_DIR) + "white_pawn.png"), vertexArrayObject));
    ecs.AddComponent(m_WhitePawn6, SpriteComponent(Graphics::Texture::CreateTexture(std::string(CHESS_ASSET_DIR) + "white_pawn.png"), vertexArrayObject));
    ecs.AddComponent(m_WhitePawn7, SpriteComponent(Graphics::Texture::CreateTexture(std::string(CHESS_ASSET_DIR) + "white_pawn.png"), vertexArrayObject));
    ecs.AddComponent(m_WhitePawn8, SpriteComponent(Graphics::Texture::CreateTexture(std::string(CHESS_ASSET_DIR) + "white_pawn.png"), vertexArrayObject));
    ecs.AddComponent(m_WhiteRook1, SpriteComponent(Graphics::Texture::CreateTexture(std::string(CHESS_ASSET_DIR) + "white_rook.png"), vertexArrayObject));
    ecs.AddComponent(m_WhiteRook2, SpriteComponent(Graphics::Texture::CreateTexture(std::string(CHESS_ASSET_DIR) + "white_rook.png"), vertexArrayObject));
    ecs.AddComponent(m_WhiteKnight1, SpriteComponent(Graphics::Texture::CreateTexture(std::string(CHESS_ASSET_DIR) + "white_knight.png"), vertexArrayObject));
    ecs.AddComponent(m_WhiteKnight2, SpriteComponent(Graphics::Texture::CreateTexture(std::string(CHESS_ASSET_DIR) + "white_knight.png"), vertexArrayObject));
    ecs.AddComponent(m_WhiteBishop1, SpriteComponent(Graphics::Texture::CreateTexture(std::string(CHESS_ASSET_DIR) + "white_bishop.png"), vertexArrayObject));
    ecs.AddComponent(m_WhiteBishop2, SpriteComponent(Graphics::Texture::CreateTexture(std::string(CHESS_ASSET_DIR) + "white_bishop.png"), vertexArrayObject));
    ecs.AddComponent(m_WhiteQueen, SpriteComponent(Graphics::Texture::CreateTexture(std::string(CHESS_ASSET_DIR) + "white_queen.png"), vertexArrayObject));
    ecs.AddComponent(m_WhiteKing, SpriteComponent(Graphics::Texture::CreateTexture(std::string(CHESS_ASSET_DIR) + "white_king.png"), vertexArrayObject));
    ecs.AddComponent(m_ChessBoard, SpriteComponent(Graphics::Texture::CreateTexture(std::string(CHESS_ASSET_DIR) + "chessboard.png"), vertexArrayObject));}


void ChessEntities::DestroyEntities()
{
    ECS::ECS& ecs = ECS::g_ECSManager.GetECS();

    ecs.RemoveEntity(m_ChessBoard);

    ecs.RemoveEntity(m_BlackPawn1);
    ecs.RemoveEntity(m_BlackPawn2);
    ecs.RemoveEntity(m_BlackPawn3);
    ecs.RemoveEntity(m_BlackPawn4);
    ecs.RemoveEntity(m_BlackPawn5);
    ecs.RemoveEntity(m_BlackPawn6);
    ecs.RemoveEntity(m_BlackPawn7);
    ecs.RemoveEntity(m_BlackPawn8);
    ecs.RemoveEntity(m_BlackRook1);
    ecs.RemoveEntity(m_BlackRook2);
    ecs.RemoveEntity(m_BlackKnight1);
    ecs.RemoveEntity(m_BlackKnight2);
    ecs.RemoveEntity(m_BlackBishop1);
    ecs.RemoveEntity(m_BlackBishop2);
    ecs.RemoveEntity(m_BlackQueen);
    ecs.RemoveEntity(m_BlackKing);

    ecs.RemoveEntity(m_WhitePawn1);
    ecs.RemoveEntity(m_WhitePawn2);
    ecs.RemoveEntity(m_WhitePawn3);
    ecs.RemoveEntity(m_WhitePawn4);
    ecs.RemoveEntity(m_WhitePawn5);
    ecs.RemoveEntity(m_WhitePawn6);
    ecs.RemoveEntity(m_WhitePawn7);
    ecs.RemoveEntity(m_WhitePawn8);
    ecs.RemoveEntity(m_WhiteRook1);
    ecs.RemoveEntity(m_WhiteRook2);
    ecs.RemoveEntity(m_WhiteKnight1);
    ecs.RemoveEntity(m_WhiteKnight2);
    ecs.RemoveEntity(m_WhiteBishop1);
    ecs.RemoveEntity(m_WhiteBishop2);
    ecs.RemoveEntity(m_WhiteQueen);
    ecs.RemoveEntity(m_WhiteKing);
}


ECS::Entity ChessEntities::GetEntity(SquareLocation location)
{
    Game::Chessboard& board = Game::g_BoardManager.GetBoard();
    if (board.GetSquareType(location) == PieceType::NONE)
    {
        return m_WhiteKing;
    }

    PieceColor color = board.GetSquareColor(location);
    uint8 pieceID = board.GetSquarePieceID(location);

    if (color.IsBlack())
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
