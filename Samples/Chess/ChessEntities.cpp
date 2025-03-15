//
// Created by Andrew Fagan on 12/11/24.
//

#include "ChessEntities.h"

#include "ECS/ECSManager.h"
#include "Renderer/Primitives/Texture.h"
#include "Chessboard/Chessboard.h"
#include "Chessboard/ChessboardManager.h"
#include "ECS/Components/Sprite.h"
#include "ECS/Components/Transform.h"


Astral::Entity ChessEntities::m_BlackPawn1;
Astral::Entity ChessEntities::m_BlackPawn2;
Astral::Entity ChessEntities::m_BlackPawn3;
Astral::Entity ChessEntities::m_BlackPawn4;
Astral::Entity ChessEntities::m_BlackPawn5;
Astral::Entity ChessEntities::m_BlackPawn6;
Astral::Entity ChessEntities::m_BlackPawn7;
Astral::Entity ChessEntities::m_BlackPawn8;
Astral::Entity ChessEntities::m_BlackRook1;
Astral::Entity ChessEntities::m_BlackRook2;
Astral::Entity ChessEntities::m_BlackKnight1;
Astral::Entity ChessEntities::m_BlackKnight2;
Astral::Entity ChessEntities::m_BlackBishop1;
Astral::Entity ChessEntities::m_BlackBishop2;
Astral::Entity ChessEntities::m_BlackQueen;
Astral::Entity ChessEntities::m_BlackKing;

Astral::Entity ChessEntities::m_WhitePawn1;
Astral::Entity ChessEntities::m_WhitePawn2;
Astral::Entity ChessEntities::m_WhitePawn3;
Astral::Entity ChessEntities::m_WhitePawn4;
Astral::Entity ChessEntities::m_WhitePawn5;
Astral::Entity ChessEntities::m_WhitePawn6;
Astral::Entity ChessEntities::m_WhitePawn7;
Astral::Entity ChessEntities::m_WhitePawn8;
Astral::Entity ChessEntities::m_WhiteRook1;
Astral::Entity ChessEntities::m_WhiteRook2;
Astral::Entity ChessEntities::m_WhiteKnight1;
Astral::Entity ChessEntities::m_WhiteKnight2;
Astral::Entity ChessEntities::m_WhiteBishop1;
Astral::Entity ChessEntities::m_WhiteBishop2;
Astral::Entity ChessEntities::m_WhiteQueen;
Astral::Entity ChessEntities::m_WhiteKing;

Astral::Entity ChessEntities::m_ChessBoard;


void ChessEntities::InitEntities(Graphics::VertexArrayObject* vertexArrayObject)
{
    Astral::ECS& ecs = Astral::g_ECSManager.GetECS();

    m_ChessBoard = ecs.CreateEntity();

    m_BlackPawn1 = ecs.CreateEntity();
    m_BlackPawn2 = ecs.CreateEntity();
    m_BlackPawn3 = ecs.CreateEntity();
    m_BlackPawn4 = ecs.CreateEntity();
    m_BlackPawn5 = ecs.CreateEntity();
    m_BlackPawn6 = ecs.CreateEntity();
    m_BlackPawn7 = ecs.CreateEntity();
    m_BlackPawn8 = ecs.CreateEntity();
    m_BlackRook1 = ecs.CreateEntity();
    m_BlackRook2 = ecs.CreateEntity();
    m_BlackKnight1 = ecs.CreateEntity();
    m_BlackKnight2 = ecs.CreateEntity();
    m_BlackBishop1 = ecs.CreateEntity();
    m_BlackBishop2 = ecs.CreateEntity();
    m_BlackQueen = ecs.CreateEntity();
    m_BlackKing = ecs.CreateEntity();

    m_WhitePawn1 = ecs.CreateEntity();
    m_WhitePawn2 = ecs.CreateEntity();
    m_WhitePawn3 = ecs.CreateEntity();
    m_WhitePawn4 = ecs.CreateEntity();
    m_WhitePawn5 = ecs.CreateEntity();
    m_WhitePawn6 = ecs.CreateEntity();
    m_WhitePawn7 = ecs.CreateEntity();
    m_WhitePawn8 = ecs.CreateEntity();
    m_WhiteRook1 = ecs.CreateEntity();
    m_WhiteRook2 = ecs.CreateEntity();
    m_WhiteKnight1 = ecs.CreateEntity();
    m_WhiteKnight2 = ecs.CreateEntity();
    m_WhiteBishop1 = ecs.CreateEntity();
    m_WhiteBishop2 = ecs.CreateEntity();
    m_WhiteQueen = ecs.CreateEntity();
    m_WhiteKing = ecs.CreateEntity();


    // Transform Components

    m_ChessBoard.SetComponent(TransformComponent(Vec3(400.0f), Vec3(2.0f)));
    m_BlackPawn1.SetComponent(TransformComponent(50, 650, .25, .25));
    m_BlackPawn2.SetComponent(TransformComponent(150, 650, .25, .25));
    m_BlackPawn3.SetComponent(TransformComponent(250, 650, .25, .25));
    m_BlackPawn4.SetComponent(TransformComponent(350, 650, .25, .25));
    m_BlackPawn5.SetComponent(TransformComponent(450, 650, .25, .25));
    m_BlackPawn6.SetComponent(TransformComponent(550, 650, .25, .25));
    m_BlackPawn7.SetComponent(TransformComponent(650, 650, .25, .25));
    m_BlackPawn8.SetComponent(TransformComponent(750, 650, .25, .25));
    m_BlackRook1.SetComponent(TransformComponent(50, 750, .25, .25));
    m_BlackRook2.SetComponent(TransformComponent(750, 750, .25, .25));
    m_BlackKnight1.SetComponent(TransformComponent(150, 750, .25, .25));
    m_BlackKnight2.SetComponent(TransformComponent(650, 750, .25, .25));
    m_BlackBishop1.SetComponent(TransformComponent(250, 750, .25, .25));
    m_BlackBishop2.SetComponent(TransformComponent(550, 750, .25, .25));
    m_BlackQueen.SetComponent(TransformComponent(350, 750, .25, .25));
    m_BlackKing.SetComponent(TransformComponent(450, 750, .25, .25));
    m_WhitePawn1.SetComponent(TransformComponent(50, 150, .25, .25));
    m_WhitePawn2.SetComponent(TransformComponent(150, 150, .25, .25));
    m_WhitePawn3.SetComponent(TransformComponent(250, 150, .25, .25));
    m_WhitePawn4.SetComponent(TransformComponent(350, 150, .25, .25));
    m_WhitePawn5.SetComponent(TransformComponent(450, 150, .25, .25));
    m_WhitePawn6.SetComponent(TransformComponent(550, 150, .25, .25));
    m_WhitePawn7.SetComponent(TransformComponent(650, 150, .25, .25));
    m_WhitePawn8.SetComponent(TransformComponent(750, 150, .25, .25));
    m_WhiteRook1.SetComponent(TransformComponent(50, 50, .25, .25));
    m_WhiteRook2.SetComponent(TransformComponent(750, 50, .25, .25));
    m_WhiteKnight1.SetComponent(TransformComponent(150, 50, .25, .25));
    m_WhiteKnight2.SetComponent(TransformComponent(650, 50, .25, .25));
    m_WhiteBishop1.SetComponent(TransformComponent(250, 50, .25, .25));
    m_WhiteBishop2.SetComponent(TransformComponent(550, 50, .25, .25));
    m_WhiteQueen.SetComponent(TransformComponent(350, 50, .25, .25));
    m_WhiteKing.SetComponent(TransformComponent(450, 50, .25, .25));


    // Sprite Components
    m_ChessBoard.SetComponent(SpriteComponent(Graphics::Texture::CreateTexture(std::string(CHESS_ASSET_DIR) + "chessboard.png"), vertexArrayObject));
    m_BlackPawn1.SetComponent(SpriteComponent(Graphics::Texture::CreateTexture(std::string(CHESS_ASSET_DIR) + "black_pawn.png"), vertexArrayObject));
    m_BlackPawn2.SetComponent(SpriteComponent(Graphics::Texture::CreateTexture(std::string(CHESS_ASSET_DIR) + "black_pawn.png"), vertexArrayObject));
    m_BlackPawn3.SetComponent(SpriteComponent(Graphics::Texture::CreateTexture(std::string(CHESS_ASSET_DIR) + "black_pawn.png"), vertexArrayObject));
    m_BlackPawn4.SetComponent(SpriteComponent(Graphics::Texture::CreateTexture(std::string(CHESS_ASSET_DIR) + "black_pawn.png"), vertexArrayObject));
    m_BlackPawn5.SetComponent(SpriteComponent(Graphics::Texture::CreateTexture(std::string(CHESS_ASSET_DIR) + "black_pawn.png"), vertexArrayObject));
    m_BlackPawn6.SetComponent(SpriteComponent(Graphics::Texture::CreateTexture(std::string(CHESS_ASSET_DIR) + "black_pawn.png"), vertexArrayObject));
    m_BlackPawn7.SetComponent(SpriteComponent(Graphics::Texture::CreateTexture(std::string(CHESS_ASSET_DIR) + "black_pawn.png"), vertexArrayObject));
    m_BlackPawn8.SetComponent(SpriteComponent(Graphics::Texture::CreateTexture(std::string(CHESS_ASSET_DIR) + "black_pawn.png"), vertexArrayObject));
    m_BlackRook1.SetComponent(SpriteComponent(Graphics::Texture::CreateTexture(std::string(CHESS_ASSET_DIR) + "black_rook.png"), vertexArrayObject));
    m_BlackRook2.SetComponent(SpriteComponent(Graphics::Texture::CreateTexture(std::string(CHESS_ASSET_DIR) + "black_rook.png"), vertexArrayObject));
    m_BlackKnight1.SetComponent(SpriteComponent(Graphics::Texture::CreateTexture(std::string(CHESS_ASSET_DIR) + "black_knight.png"), vertexArrayObject));
    m_BlackKnight2.SetComponent(SpriteComponent(Graphics::Texture::CreateTexture(std::string(CHESS_ASSET_DIR) + "black_knight.png"), vertexArrayObject));
    m_BlackBishop1.SetComponent(SpriteComponent(Graphics::Texture::CreateTexture(std::string(CHESS_ASSET_DIR) + "black_bishop.png"), vertexArrayObject));
    m_BlackBishop2.SetComponent(SpriteComponent(Graphics::Texture::CreateTexture(std::string(CHESS_ASSET_DIR) + "black_bishop.png"), vertexArrayObject));
    m_BlackQueen.SetComponent(SpriteComponent(Graphics::Texture::CreateTexture(std::string(CHESS_ASSET_DIR) + "black_queen.png"), vertexArrayObject));
    m_BlackKing.SetComponent(SpriteComponent(Graphics::Texture::CreateTexture(std::string(CHESS_ASSET_DIR) + "black_king.png"), vertexArrayObject));
    m_WhitePawn1.SetComponent(SpriteComponent(Graphics::Texture::CreateTexture(std::string(CHESS_ASSET_DIR) + "white_pawn.png"), vertexArrayObject));
    m_WhitePawn2.SetComponent(SpriteComponent(Graphics::Texture::CreateTexture(std::string(CHESS_ASSET_DIR) + "white_pawn.png"), vertexArrayObject));
    m_WhitePawn3.SetComponent(SpriteComponent(Graphics::Texture::CreateTexture(std::string(CHESS_ASSET_DIR) + "white_pawn.png"), vertexArrayObject));
    m_WhitePawn4.SetComponent(SpriteComponent(Graphics::Texture::CreateTexture(std::string(CHESS_ASSET_DIR) + "white_pawn.png"), vertexArrayObject));
    m_WhitePawn5.SetComponent(SpriteComponent(Graphics::Texture::CreateTexture(std::string(CHESS_ASSET_DIR) + "white_pawn.png"), vertexArrayObject));
    m_WhitePawn6.SetComponent(SpriteComponent(Graphics::Texture::CreateTexture(std::string(CHESS_ASSET_DIR) + "white_pawn.png"), vertexArrayObject));
    m_WhitePawn7.SetComponent(SpriteComponent(Graphics::Texture::CreateTexture(std::string(CHESS_ASSET_DIR) + "white_pawn.png"), vertexArrayObject));
    m_WhitePawn8.SetComponent(SpriteComponent(Graphics::Texture::CreateTexture(std::string(CHESS_ASSET_DIR) + "white_pawn.png"), vertexArrayObject));
    m_WhiteRook1.SetComponent(SpriteComponent(Graphics::Texture::CreateTexture(std::string(CHESS_ASSET_DIR) + "white_rook.png"), vertexArrayObject));
    m_WhiteRook2.SetComponent(SpriteComponent(Graphics::Texture::CreateTexture(std::string(CHESS_ASSET_DIR) + "white_rook.png"), vertexArrayObject));
    m_WhiteKnight1.SetComponent(SpriteComponent(Graphics::Texture::CreateTexture(std::string(CHESS_ASSET_DIR) + "white_knight.png"), vertexArrayObject));
    m_WhiteKnight2.SetComponent(SpriteComponent(Graphics::Texture::CreateTexture(std::string(CHESS_ASSET_DIR) + "white_knight.png"), vertexArrayObject));
    m_WhiteBishop1.SetComponent(SpriteComponent(Graphics::Texture::CreateTexture(std::string(CHESS_ASSET_DIR) + "white_bishop.png"), vertexArrayObject));
    m_WhiteBishop2.SetComponent(SpriteComponent(Graphics::Texture::CreateTexture(std::string(CHESS_ASSET_DIR) + "white_bishop.png"), vertexArrayObject));
    m_WhiteQueen.SetComponent(SpriteComponent(Graphics::Texture::CreateTexture(std::string(CHESS_ASSET_DIR) + "white_queen.png"), vertexArrayObject));
    m_WhiteKing.SetComponent(SpriteComponent(Graphics::Texture::CreateTexture(std::string(CHESS_ASSET_DIR) + "white_king.png"), vertexArrayObject));
    m_ChessBoard.SetComponent(SpriteComponent(Graphics::Texture::CreateTexture(std::string(CHESS_ASSET_DIR) + "chessboard.png"), vertexArrayObject));}


void ChessEntities::DestroyEntities()
{
    Astral::ECS& ecs = Astral::g_ECSManager.GetECS();

    m_ChessBoard.Delete();

    m_BlackPawn1.Delete();
    m_BlackPawn2.Delete();
    m_BlackPawn3.Delete();
    m_BlackPawn4.Delete();
    m_BlackPawn5.Delete();
    m_BlackPawn6.Delete();
    m_BlackPawn7.Delete();
    m_BlackPawn8.Delete();
    m_BlackRook1.Delete();
    m_BlackRook2.Delete();
    m_BlackKnight1.Delete();
    m_BlackKnight2.Delete();
    m_BlackBishop1.Delete();
    m_BlackBishop2.Delete();
    m_BlackQueen.Delete();
    m_BlackKing.Delete();

    m_WhitePawn1.Delete();
    m_WhitePawn2.Delete();
    m_WhitePawn3.Delete();
    m_WhitePawn4.Delete();
    m_WhitePawn5.Delete();
    m_WhitePawn6.Delete();
    m_WhitePawn7.Delete();
    m_WhitePawn8.Delete();
    m_WhiteRook1.Delete();
    m_WhiteRook2.Delete();
    m_WhiteKnight1.Delete();
    m_WhiteKnight2.Delete();
    m_WhiteBishop1.Delete();
    m_WhiteBishop2.Delete();
    m_WhiteQueen.Delete();
    m_WhiteKing.Delete();
}


Astral::Entity ChessEntities::GetEntity(SquareLocation location)
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
