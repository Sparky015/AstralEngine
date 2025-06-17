//
// Created by Andrew Fagan on 12/11/24.
//

#include "ChessEntities.h"

#include "ECS/ECSManager.h"
#include "Asset/AssetManager.h"
#include "Renderer/RHI/Resources/Texture.h"

#include "Core/Engine.h"
#include "ECS/Components/Sprite.h"
#include "ECS/Components/Transform.h"
#include "Renderer/Common/Material.h"
#include "Renderer/Common/Mesh.h"
#include "Asset/AssetManager.h"


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


void ChessEntities::InitEntities(const Astral::Mesh& mesh)
{
    Astral::ECS& ecs = Astral::Engine::Get().GetECSManager().GetECS();

    m_ChessBoard = ecs.CreateEntity("Chessboard");

    m_BlackPawn1 = ecs.CreateEntity("BlackPawn1");
    m_BlackPawn2 = ecs.CreateEntity("BlackPawn2");
    m_BlackPawn3 = ecs.CreateEntity("BlackPawn3");
    m_BlackPawn4 = ecs.CreateEntity("BlackPawn4");
    m_BlackPawn5 = ecs.CreateEntity("BlackPawn5");
    m_BlackPawn6 = ecs.CreateEntity("BlackPawn6");
    m_BlackPawn7 = ecs.CreateEntity("BlackPawn7");
    m_BlackPawn8 = ecs.CreateEntity("BlackPawn8");
    m_BlackRook1 = ecs.CreateEntity("BlackRook1");
    m_BlackRook2 = ecs.CreateEntity("BlackRook2");
    m_BlackKnight1 = ecs.CreateEntity("BlackKnight1");
    m_BlackKnight2 = ecs.CreateEntity("BlackKnight2");
    m_BlackBishop1 = ecs.CreateEntity("BlackBishop1");
    m_BlackBishop2 = ecs.CreateEntity("BlackBishop2");
    m_BlackQueen = ecs.CreateEntity("BlackQueen");
    m_BlackKing = ecs.CreateEntity("BlackKing");

    m_WhitePawn1 = ecs.CreateEntity("WhitePawn1");
    m_WhitePawn2 = ecs.CreateEntity("WhitePawn2");
    m_WhitePawn3 = ecs.CreateEntity("WhitePawn3");
    m_WhitePawn4 = ecs.CreateEntity("WhitePawn4");
    m_WhitePawn5 = ecs.CreateEntity("WhitePawn5");
    m_WhitePawn6 = ecs.CreateEntity("WhitePawn6");
    m_WhitePawn7 = ecs.CreateEntity("WhitePawn7");
    m_WhitePawn8 = ecs.CreateEntity("WhitePawn8");
    m_WhiteRook1 = ecs.CreateEntity("WhiteRook1");
    m_WhiteRook2 = ecs.CreateEntity("WhiteRook2");
    m_WhiteKnight1 = ecs.CreateEntity("WhiteKnight1");
    m_WhiteKnight2 = ecs.CreateEntity("WhiteKnight2");
    m_WhiteBishop1 = ecs.CreateEntity("WhiteBishop1");
    m_WhiteBishop2 = ecs.CreateEntity("WhiteBishop2");
    m_WhiteQueen = ecs.CreateEntity("WhiteQueen");
    m_WhiteKing = ecs.CreateEntity("WhiteKing");


    // Transform Components

    ecs.AddComponent(m_ChessBoard,   TransformComponent({0.0f, 0.0f, 0.0f}, {800, 800, 800}));
    ecs.AddComponent(m_BlackPawn1,   TransformComponent(-350, -250, 100, 100));
    ecs.AddComponent(m_BlackPawn2,   TransformComponent(-250, -250, 100, 100));
    ecs.AddComponent(m_BlackPawn3,   TransformComponent(-150, -250, 100, 100));
    ecs.AddComponent(m_BlackPawn4,   TransformComponent(-50, -250, 100, 100));
    ecs.AddComponent(m_BlackPawn5,   TransformComponent(50, -250, 100, 100));
    ecs.AddComponent(m_BlackPawn6,   TransformComponent(150, -250, 100, 100));
    ecs.AddComponent(m_BlackPawn7,   TransformComponent(250, -250, 100, 100));
    ecs.AddComponent(m_BlackPawn8,   TransformComponent(350, -250, 100, 100));
    ecs.AddComponent(m_BlackRook1,   TransformComponent(-350, -350, 100, 100));
    ecs.AddComponent(m_BlackRook2,   TransformComponent(350, -350, 100, 100));
    ecs.AddComponent(m_BlackKnight1, TransformComponent(-250, -350, 100, 100));
    ecs.AddComponent(m_BlackKnight2, TransformComponent(250, -350, 100, 100));
    ecs.AddComponent(m_BlackBishop1, TransformComponent(-150, -350, 100, 100));
    ecs.AddComponent(m_BlackBishop2, TransformComponent(150, -350, 100, 100));
    ecs.AddComponent(m_BlackQueen,   TransformComponent(-50, -350, 100, 100));
    ecs.AddComponent(m_BlackKing,    TransformComponent(50, -350, 100, 100));
    ecs.AddComponent(m_WhitePawn1,   TransformComponent(-350, 250, 100, 100));
    ecs.AddComponent(m_WhitePawn2,   TransformComponent(-250, 250, 100, 100));
    ecs.AddComponent(m_WhitePawn3,   TransformComponent(-150, 250, 100, 100));
    ecs.AddComponent(m_WhitePawn4,   TransformComponent(-50, 250, 100, 100));
    ecs.AddComponent(m_WhitePawn5,   TransformComponent(50, 250, 100, 100));
    ecs.AddComponent(m_WhitePawn6,   TransformComponent(150, 250, 100, 100));
    ecs.AddComponent(m_WhitePawn7,   TransformComponent(250, 250, 100, 100));
    ecs.AddComponent(m_WhitePawn8,   TransformComponent(350, 250, 100, 100));
    ecs.AddComponent(m_WhiteRook1,   TransformComponent(-350, 350, 100, 100));
    ecs.AddComponent(m_WhiteRook2,   TransformComponent(350, 350, 100, 100));
    ecs.AddComponent(m_WhiteKnight1, TransformComponent(-250, 350, 100, 100));
    ecs.AddComponent(m_WhiteKnight2, TransformComponent(250, 350, 100, 100));
    ecs.AddComponent(m_WhiteBishop1, TransformComponent(-150, 350, 100, 100));
    ecs.AddComponent(m_WhiteBishop2, TransformComponent(150, 350, 100, 100));
    ecs.AddComponent(m_WhiteQueen,   TransformComponent(-50, 350, 100, 100));
    ecs.AddComponent(m_WhiteKing,    TransformComponent(50, 350, 100, 100));


    Astral::AssetRegistry& registry = Astral::Engine::Get().GetAssetManager().GetRegistry();
    registry.SetAssetDirectoryPath(CHESS_ASSET_DIR);

    Astral::AssetID chessboardMaterialID = registry.CreateAsset<Astral::Material>("chessboard.astmat");

    Astral::AssetID blackPawnMaterialID = registry.CreateAsset<Astral::Material>("black_pawn.astmat");
    Astral::AssetID blackRookMaterialID = registry.CreateAsset<Astral::Material>("black_rook.astmat");
    Astral::AssetID blackKnightMaterialID = registry.CreateAsset<Astral::Material>("black_knight.astmat");
    Astral::AssetID blackBishopMaterialID = registry.CreateAsset<Astral::Material>("black_bishop.astmat");
    Astral::AssetID blackQueenMaterialID = registry.CreateAsset<Astral::Material>("black_queen.astmat");
    Astral::AssetID blackKingMaterialID = registry.CreateAsset<Astral::Material>("black_king.astmat");

    Astral::AssetID whitePawnMaterialID = registry.CreateAsset<Astral::Material>("white_pawn.astmat");
    Astral::AssetID whiteRookMaterialID = registry.CreateAsset<Astral::Material>("white_rook.astmat");
    Astral::AssetID whiteKnightMaterialID = registry.CreateAsset<Astral::Material>("white_knight.astmat");
    Astral::AssetID whiteBishopMaterialID = registry.CreateAsset<Astral::Material>("white_bishop.astmat");
    Astral::AssetID whiteQueenMaterialID = registry.CreateAsset<Astral::Material>("white_queen.astmat");
    Astral::AssetID whiteKingMaterialID = registry.CreateAsset<Astral::Material>("white_king.astmat");


    // Sprite Components
    ecs.AddComponent(m_ChessBoard, SpriteComponent(chessboardMaterialID, mesh));

    // Black Pieces
    ecs.AddComponent(m_BlackPawn1, SpriteComponent(blackPawnMaterialID, mesh));
    ecs.AddComponent(m_BlackPawn2, SpriteComponent(blackPawnMaterialID, mesh));
    ecs.AddComponent(m_BlackPawn3, SpriteComponent(blackPawnMaterialID, mesh));
    ecs.AddComponent(m_BlackPawn4, SpriteComponent(blackPawnMaterialID, mesh));
    ecs.AddComponent(m_BlackPawn5, SpriteComponent(blackPawnMaterialID, mesh));
    ecs.AddComponent(m_BlackPawn6, SpriteComponent(blackPawnMaterialID, mesh));
    ecs.AddComponent(m_BlackPawn7, SpriteComponent(blackPawnMaterialID, mesh));
    ecs.AddComponent(m_BlackPawn8, SpriteComponent(blackPawnMaterialID, mesh));
    ecs.AddComponent(m_BlackRook1, SpriteComponent(blackRookMaterialID, mesh));
    ecs.AddComponent(m_BlackRook2, SpriteComponent(blackRookMaterialID, mesh));
    ecs.AddComponent(m_BlackKnight1, SpriteComponent(blackKnightMaterialID, mesh));
    ecs.AddComponent(m_BlackKnight2, SpriteComponent(blackKnightMaterialID, mesh));
    ecs.AddComponent(m_BlackBishop1, SpriteComponent(blackBishopMaterialID, mesh));
    ecs.AddComponent(m_BlackBishop2, SpriteComponent(blackBishopMaterialID, mesh));
    ecs.AddComponent(m_BlackQueen, SpriteComponent(blackQueenMaterialID, mesh));
    ecs.AddComponent(m_BlackKing, SpriteComponent(blackKingMaterialID, mesh));

    // White Pieces
    ecs.AddComponent(m_WhitePawn1, SpriteComponent(whitePawnMaterialID, mesh));
    ecs.AddComponent(m_WhitePawn2, SpriteComponent(whitePawnMaterialID, mesh));
    ecs.AddComponent(m_WhitePawn3, SpriteComponent(whitePawnMaterialID, mesh));
    ecs.AddComponent(m_WhitePawn4, SpriteComponent(whitePawnMaterialID, mesh));
    ecs.AddComponent(m_WhitePawn5, SpriteComponent(whitePawnMaterialID, mesh));
    ecs.AddComponent(m_WhitePawn6, SpriteComponent(whitePawnMaterialID, mesh));
    ecs.AddComponent(m_WhitePawn7, SpriteComponent(whitePawnMaterialID, mesh));
    ecs.AddComponent(m_WhitePawn8, SpriteComponent(whitePawnMaterialID, mesh));
    ecs.AddComponent(m_WhiteRook1, SpriteComponent(whiteRookMaterialID, mesh));
    ecs.AddComponent(m_WhiteRook2, SpriteComponent(whiteRookMaterialID, mesh));
    ecs.AddComponent(m_WhiteKnight1, SpriteComponent(whiteKnightMaterialID, mesh));
    ecs.AddComponent(m_WhiteKnight2, SpriteComponent(whiteKnightMaterialID, mesh));
    ecs.AddComponent(m_WhiteBishop1, SpriteComponent(whiteBishopMaterialID, mesh));
    ecs.AddComponent(m_WhiteBishop2, SpriteComponent(whiteBishopMaterialID, mesh));
    ecs.AddComponent(m_WhiteQueen, SpriteComponent(whiteQueenMaterialID, mesh));
    ecs.AddComponent(m_WhiteKing, SpriteComponent(whiteKingMaterialID, mesh));
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


