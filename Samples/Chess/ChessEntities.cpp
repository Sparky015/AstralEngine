//
// Created by Andrew Fagan on 12/11/24.
//

#include "ChessEntities.h"

#include <ChessModule.h>

#include "ECS/ECSManager.h"
#include "Asset/AssetManager.h"
#include "Renderer/RHI/Resources/Texture.h"
#include "Chessboard/Chessboard.h"
#include "Chessboard/ChessboardManager.h"
#include "Core/Engine.h"
#include "ECS/Components/Sprite.h"
#include "ECS/Components/Transform.h"
#include "Renderer/Common/Material.h"
#include "Renderer/Common/Mesh.h"


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


void ChessEntities::InitEntities(const Astral::Mesh& mesh, const Astral::Material& partialMaterial)
{
    Astral::ECS& ecs = Astral::Engine::Get().GetECSManager().GetECS();

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

    ecs.AddComponent(m_ChessBoard,   TransformComponent(Vec3(400.0f), Vec3(2.0f)));
    ecs.AddComponent(m_BlackPawn1,   TransformComponent(50, 150, .25, .25));
    ecs.AddComponent(m_BlackPawn2,   TransformComponent(150, 150, .25, .25));
    ecs.AddComponent(m_BlackPawn3,   TransformComponent(250, 150, .25, .25));
    ecs.AddComponent(m_BlackPawn4,   TransformComponent(350, 150, .25, .25));
    ecs.AddComponent(m_BlackPawn5,   TransformComponent(450, 150, .25, .25));
    ecs.AddComponent(m_BlackPawn6,   TransformComponent(550, 150, .25, .25));
    ecs.AddComponent(m_BlackPawn7,   TransformComponent(650, 150, .25, .25));
    ecs.AddComponent(m_BlackPawn8,   TransformComponent(750, 150, .25, .25));
    ecs.AddComponent(m_BlackRook1,   TransformComponent(50, 50, .25, .25));
    ecs.AddComponent(m_BlackRook2,   TransformComponent(750, 50, .25, .25));
    ecs.AddComponent(m_BlackKnight1, TransformComponent(150, 50, .25, .25));
    ecs.AddComponent(m_BlackKnight2, TransformComponent(650, 50, .25, .25));
    ecs.AddComponent(m_BlackBishop1, TransformComponent(250, 50, .25, .25));
    ecs.AddComponent(m_BlackBishop2, TransformComponent(550, 50, .25, .25));
    ecs.AddComponent(m_BlackQueen,   TransformComponent(350, 50, .25, .25));
    ecs.AddComponent(m_BlackKing,    TransformComponent(450, 50, .25, .25));
    ecs.AddComponent(m_WhitePawn1,   TransformComponent(50, 650, .25, .25));
    ecs.AddComponent(m_WhitePawn2,   TransformComponent(150, 650, .25, .25));
    ecs.AddComponent(m_WhitePawn3,   TransformComponent(250, 650, .25, .25));
    ecs.AddComponent(m_WhitePawn4,   TransformComponent(350, 650, .25, .25));
    ecs.AddComponent(m_WhitePawn5,   TransformComponent(450, 650, .25, .25));
    ecs.AddComponent(m_WhitePawn6,   TransformComponent(550, 650, .25, .25));
    ecs.AddComponent(m_WhitePawn7,   TransformComponent(650, 650, .25, .25));
    ecs.AddComponent(m_WhitePawn8,   TransformComponent(750, 650, .25, .25));
    ecs.AddComponent(m_WhiteRook1,   TransformComponent(50, 750, .25, .25));
    ecs.AddComponent(m_WhiteRook2,   TransformComponent(750, 750, .25, .25));
    ecs.AddComponent(m_WhiteKnight1, TransformComponent(150, 750, .25, .25));
    ecs.AddComponent(m_WhiteKnight2, TransformComponent(650, 750, .25, .25));
    ecs.AddComponent(m_WhiteBishop1, TransformComponent(250, 750, .25, .25));
    ecs.AddComponent(m_WhiteBishop2, TransformComponent(550, 750, .25, .25));
    ecs.AddComponent(m_WhiteQueen,   TransformComponent(350, 750, .25, .25));
    ecs.AddComponent(m_WhiteKing,    TransformComponent(450, 750, .25, .25));

    Astral::AssetRegistry& registry = Astral::g_AssetManager.GetRegistry();
    registry.SetAssetDirectoryPath(CHESS_ASSET_DIR);

    Astral::AssetID chessboardTextureID = registry.CreateAsset<Astral::Texture>("chessboard.png");

    Astral::AssetID blackPawnTextureID = registry.CreateAsset<Astral::Texture>("black_pawn.png");
    Astral::AssetID blackRookTextureID = registry.CreateAsset<Astral::Texture>("black_rook.png");
    Astral::AssetID blackKnightTextureID = registry.CreateAsset<Astral::Texture>("black_knight.png");
    Astral::AssetID blackBishopTextureID = registry.CreateAsset<Astral::Texture>("black_bishop.png");
    Astral::AssetID blackQueenTextureID = registry.CreateAsset<Astral::Texture>("black_queen.png");
    Astral::AssetID blackKingTextureID = registry.CreateAsset<Astral::Texture>("black_king.png");

    Astral::AssetID whitePawnTextureID = registry.CreateAsset<Astral::Texture>("white_pawn.png");
    Astral::AssetID whiteRookTextureID = registry.CreateAsset<Astral::Texture>("white_rook.png");
    Astral::AssetID whiteKnightTextureID = registry.CreateAsset<Astral::Texture>("white_knight.png");
    Astral::AssetID whiteBishopTextureID = registry.CreateAsset<Astral::Texture>("white_bishop.png");
    Astral::AssetID whiteQueenTextureID = registry.CreateAsset<Astral::Texture>("white_queen.png");
    Astral::AssetID whiteKingTextureID = registry.CreateAsset<Astral::Texture>("white_king.png");


    Astral::Material chessboardMaterial = partialMaterial;
    chessboardMaterial.TextureUniform = Astral::Texture::CreateTexture(std::string(CHESS_ASSET_DIR) + "chessboard.png");

    Astral::Material blackPawnMaterial = partialMaterial;
    blackPawnMaterial.TextureUniform = Astral::Texture::CreateTexture(std::string(CHESS_ASSET_DIR) + "black_pawn.png");

    Astral::Material blackRookMaterial = partialMaterial;
    blackRookMaterial.TextureUniform = Astral::Texture::CreateTexture(std::string(CHESS_ASSET_DIR) + "black_rook.png");

    Astral::Material blackKnightMaterial = partialMaterial;
    blackKnightMaterial.TextureUniform = Astral::Texture::CreateTexture(std::string(CHESS_ASSET_DIR) + "black_knight.png");

    Astral::Material blackBishopMaterial = partialMaterial;
    blackBishopMaterial.TextureUniform = Astral::Texture::CreateTexture(std::string(CHESS_ASSET_DIR) + "black_bishop.png");

    Astral::Material blackQueenMaterial = partialMaterial;
    blackQueenMaterial.TextureUniform = Astral::Texture::CreateTexture(std::string(CHESS_ASSET_DIR) + "black_queen.png");

    Astral::Material blackKingMaterial = partialMaterial;
    blackKingMaterial.TextureUniform = Astral::Texture::CreateTexture(std::string(CHESS_ASSET_DIR) + "black_king.png");

    Astral::Material whitePawnMaterial = partialMaterial;
    whitePawnMaterial.TextureUniform = Astral::Texture::CreateTexture(std::string(CHESS_ASSET_DIR) + "white_pawn.png");

    Astral::Material whiteRookMaterial = partialMaterial;
    whiteRookMaterial.TextureUniform = Astral::Texture::CreateTexture(std::string(CHESS_ASSET_DIR) + "white_rook.png");

    Astral::Material whiteKnightMaterial = partialMaterial;
    whiteKnightMaterial.TextureUniform = Astral::Texture::CreateTexture(std::string(CHESS_ASSET_DIR) + "white_knight.png");

    Astral::Material whiteBishopMaterial = partialMaterial;
    whiteBishopMaterial.TextureUniform = Astral::Texture::CreateTexture(std::string(CHESS_ASSET_DIR) + "white_bishop.png");

    Astral::Material whiteQueenMaterial = partialMaterial;
    whiteQueenMaterial.TextureUniform = Astral::Texture::CreateTexture(std::string(CHESS_ASSET_DIR) + "white_queen.png");

    Astral::Material whiteKingMaterial = partialMaterial;
    whiteKingMaterial.TextureUniform = Astral::Texture::CreateTexture(std::string(CHESS_ASSET_DIR) + "white_king.png");

    // Sprite Components
    ecs.AddComponent(m_ChessBoard, SpriteComponent(chessboardMaterial, mesh));

    // Black Pieces
    ecs.AddComponent(m_BlackPawn1, SpriteComponent(blackPawnMaterial, mesh));
    ecs.AddComponent(m_BlackPawn2, SpriteComponent(blackPawnMaterial, mesh));
    ecs.AddComponent(m_BlackPawn3, SpriteComponent(blackPawnMaterial, mesh));
    ecs.AddComponent(m_BlackPawn4, SpriteComponent(blackPawnMaterial, mesh));
    ecs.AddComponent(m_BlackPawn5, SpriteComponent(blackPawnMaterial, mesh));
    ecs.AddComponent(m_BlackPawn6, SpriteComponent(blackPawnMaterial, mesh));
    ecs.AddComponent(m_BlackPawn7, SpriteComponent(blackPawnMaterial, mesh));
    ecs.AddComponent(m_BlackPawn8, SpriteComponent(blackPawnMaterial, mesh));
    ecs.AddComponent(m_BlackRook1, SpriteComponent(blackRookMaterial, mesh));
    ecs.AddComponent(m_BlackRook2, SpriteComponent(blackRookMaterial, mesh));
    ecs.AddComponent(m_BlackKnight1, SpriteComponent(blackKnightMaterial, mesh));
    ecs.AddComponent(m_BlackKnight2, SpriteComponent(blackKnightMaterial, mesh));
    ecs.AddComponent(m_BlackBishop1, SpriteComponent(blackBishopMaterial, mesh));
    ecs.AddComponent(m_BlackBishop2, SpriteComponent(blackBishopMaterial, mesh));
    ecs.AddComponent(m_BlackQueen, SpriteComponent(blackQueenMaterial, mesh));
    ecs.AddComponent(m_BlackKing, SpriteComponent(blackKingMaterial, mesh));

    // White Pieces
    ecs.AddComponent(m_WhitePawn1, SpriteComponent(whitePawnMaterial, mesh));
    ecs.AddComponent(m_WhitePawn2, SpriteComponent(whitePawnMaterial, mesh));
    ecs.AddComponent(m_WhitePawn3, SpriteComponent(whitePawnMaterial, mesh));
    ecs.AddComponent(m_WhitePawn4, SpriteComponent(whitePawnMaterial, mesh));
    ecs.AddComponent(m_WhitePawn5, SpriteComponent(whitePawnMaterial, mesh));
    ecs.AddComponent(m_WhitePawn6, SpriteComponent(whitePawnMaterial, mesh));
    ecs.AddComponent(m_WhitePawn7, SpriteComponent(whitePawnMaterial, mesh));
    ecs.AddComponent(m_WhitePawn8, SpriteComponent(whitePawnMaterial, mesh));
    ecs.AddComponent(m_WhiteRook1, SpriteComponent(whiteRookMaterial, mesh));
    ecs.AddComponent(m_WhiteRook2, SpriteComponent(whiteRookMaterial, mesh));
    ecs.AddComponent(m_WhiteKnight1, SpriteComponent(whiteKnightMaterial, mesh));
    ecs.AddComponent(m_WhiteKnight2, SpriteComponent(whiteKnightMaterial, mesh));
    ecs.AddComponent(m_WhiteBishop1, SpriteComponent(whiteBishopMaterial, mesh));
    ecs.AddComponent(m_WhiteBishop2, SpriteComponent(whiteBishopMaterial, mesh));
    ecs.AddComponent(m_WhiteQueen, SpriteComponent(whiteQueenMaterial, mesh));
    ecs.AddComponent(m_WhiteKing, SpriteComponent(whiteKingMaterial, mesh));
}


void ChessEntities::DestroyEntities()
{
    Astral::ECS& ecs = Astral::Engine::Get().GetECSManager().GetECS();

    ecs.DeleteEntity(m_ChessBoard);

    ecs.DeleteEntity(m_BlackPawn1);
    ecs.DeleteEntity(m_BlackPawn2);
    ecs.DeleteEntity(m_BlackPawn3);
    ecs.DeleteEntity(m_BlackPawn4);
    ecs.DeleteEntity(m_BlackPawn5);
    ecs.DeleteEntity(m_BlackPawn6);
    ecs.DeleteEntity(m_BlackPawn7);
    ecs.DeleteEntity(m_BlackPawn8);
    ecs.DeleteEntity(m_BlackRook1);
    ecs.DeleteEntity(m_BlackRook2);
    ecs.DeleteEntity(m_BlackKnight1);
    ecs.DeleteEntity(m_BlackKnight2);
    ecs.DeleteEntity(m_BlackBishop1);
    ecs.DeleteEntity(m_BlackBishop2);
    ecs.DeleteEntity(m_BlackQueen);
    ecs.DeleteEntity(m_BlackKing);

    ecs.DeleteEntity(m_WhitePawn1);
    ecs.DeleteEntity(m_WhitePawn2);
    ecs.DeleteEntity(m_WhitePawn3);
    ecs.DeleteEntity(m_WhitePawn4);
    ecs.DeleteEntity(m_WhitePawn5);
    ecs.DeleteEntity(m_WhitePawn6);
    ecs.DeleteEntity(m_WhitePawn7);
    ecs.DeleteEntity(m_WhitePawn8);
    ecs.DeleteEntity(m_WhiteRook1);
    ecs.DeleteEntity(m_WhiteRook2);
    ecs.DeleteEntity(m_WhiteKnight1);
    ecs.DeleteEntity(m_WhiteKnight2);
    ecs.DeleteEntity(m_WhiteBishop1);
    ecs.DeleteEntity(m_WhiteBishop2);
    ecs.DeleteEntity(m_WhiteQueen);
    ecs.DeleteEntity(m_WhiteKing);
}


Astral::Entity ChessEntities::GetEntity(SquareLocation location)
{
    Game::Chessboard& board = ChessModule::Get().GetBoardManager().GetBoard();

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
