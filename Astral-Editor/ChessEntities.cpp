//
// Created by Andrew Fagan on 12/11/24.
//

#include "ChessEntities.h"

#include "ECS/SceneManager.h"
#include "Asset/AssetManager.h"
#include "Renderer/RHI/Resources/Texture.h"

#include "Core/Engine.h"
#include "ECS/Components/SpriteComponent.h"
#include "ECS/Components/TransformComponent.h"
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

Astral::Entity ChessEntities::m_Mesh;


void ChessEntities::InitEntities()
{
    Astral::ECS& ecs = Astral::Engine::Get().GetSceneManager().GetECS();

    // m_ChessBoard = ecs.CreateEntity("Chessboard");
    //
    // m_BlackPawn1 = ecs.CreateEntity("BlackPawn1");
    // m_BlackPawn2 = ecs.CreateEntity("BlackPawn2");
    // m_BlackPawn3 = ecs.CreateEntity("BlackPawn3");
    // m_BlackPawn4 = ecs.CreateEntity("BlackPawn4");
    // m_BlackPawn5 = ecs.CreateEntity("BlackPawn5");
    // m_BlackPawn6 = ecs.CreateEntity("BlackPawn6");
    // m_BlackPawn7 = ecs.CreateEntity("BlackPawn7");
    // m_BlackPawn8 = ecs.CreateEntity("BlackPawn8");
    // m_BlackRook1 = ecs.CreateEntity("BlackRook1");
    // m_BlackRook2 = ecs.CreateEntity("BlackRook2");
    // m_BlackKnight1 = ecs.CreateEntity("BlackKnight1");
    // m_BlackKnight2 = ecs.CreateEntity("BlackKnight2");
    // m_BlackBishop1 = ecs.CreateEntity("BlackBishop1");
    // m_BlackBishop2 = ecs.CreateEntity("BlackBishop2");
    // m_BlackQueen = ecs.CreateEntity("BlackQueen");
    // m_BlackKing = ecs.CreateEntity("BlackKing");
    //
    // m_WhitePawn1 = ecs.CreateEntity("WhitePawn1");
    // m_WhitePawn2 = ecs.CreateEntity("WhitePawn2");
    // m_WhitePawn3 = ecs.CreateEntity("WhitePawn3");
    // m_WhitePawn4 = ecs.CreateEntity("WhitePawn4");
    // m_WhitePawn5 = ecs.CreateEntity("WhitePawn5");
    // m_WhitePawn6 = ecs.CreateEntity("WhitePawn6");
    // m_WhitePawn7 = ecs.CreateEntity("WhitePawn7");
    // m_WhitePawn8 = ecs.CreateEntity("WhitePawn8");
    // m_WhiteRook1 = ecs.CreateEntity("WhiteRook1");
    // m_WhiteRook2 = ecs.CreateEntity("WhiteRook2");
    // m_WhiteKnight1 = ecs.CreateEntity("WhiteKnight1");
    // m_WhiteKnight2 = ecs.CreateEntity("WhiteKnight2");
    // m_WhiteBishop1 = ecs.CreateEntity("WhiteBishop1");
    // m_WhiteBishop2 = ecs.CreateEntity("WhiteBishop2");
    // m_WhiteQueen = ecs.CreateEntity("WhiteQueen");
    // m_WhiteKing = ecs.CreateEntity("WhiteKing");

    // Transform Components
    //
    // ecs.AddComponent(m_ChessBoard,   TransformComponent({0.0f, 0.0f, -3.0f}, {800, 800, 800}));
    // ecs.AddComponent(m_BlackPawn1,   TransformComponent(-350, 250, 100, 100));
    // ecs.AddComponent(m_BlackPawn2,   TransformComponent(-250, 250, 100, 100));
    // ecs.AddComponent(m_BlackPawn3,   TransformComponent(-150, 250, 100, 100));
    // ecs.AddComponent(m_BlackPawn4,   TransformComponent(-50, 250, 100, 100));
    // ecs.AddComponent(m_BlackPawn5,   TransformComponent(50, 250, 100, 100));
    // ecs.AddComponent(m_BlackPawn6,   TransformComponent(150, 250, 100, 100));
    // ecs.AddComponent(m_BlackPawn7,   TransformComponent(250, 250, 100, 100));
    // ecs.AddComponent(m_BlackPawn8,   TransformComponent(350, 250, 100, 100));
    // ecs.AddComponent(m_BlackRook1,   TransformComponent(-350, 350, 100, 100));
    // ecs.AddComponent(m_BlackRook2,   TransformComponent(350, 350, 100, 100));
    // ecs.AddComponent(m_BlackKnight1, TransformComponent(-250, 350, 100, 100));
    // ecs.AddComponent(m_BlackKnight2, TransformComponent(250, 350, 100, 100));
    // ecs.AddComponent(m_BlackBishop1, TransformComponent(-150, 350, 100, 100));
    // ecs.AddComponent(m_BlackBishop2, TransformComponent(150, 350, 100, 100));
    // ecs.AddComponent(m_BlackQueen,   TransformComponent(-50, 350, 100, 100));
    // ecs.AddComponent(m_BlackKing,    TransformComponent(50, 350, 100, 100));
    // ecs.AddComponent(m_WhitePawn1,   TransformComponent(-350, -250, 100, 100));
    // ecs.AddComponent(m_WhitePawn2,   TransformComponent(-250, -250, 100, 100));
    // ecs.AddComponent(m_WhitePawn3,   TransformComponent(-150, -250, 100, 100));
    // ecs.AddComponent(m_WhitePawn4,   TransformComponent(-50, -250, 100, 100));
    // ecs.AddComponent(m_WhitePawn5,   TransformComponent(50, -250, 100, 100));
    // ecs.AddComponent(m_WhitePawn6,   TransformComponent(150, -250, 100, 100));
    // ecs.AddComponent(m_WhitePawn7,   TransformComponent(250, -250, 100, 100));
    // ecs.AddComponent(m_WhitePawn8,   TransformComponent(350, -250, 100, 100));
    // ecs.AddComponent(m_WhiteRook1,   TransformComponent(-350, -350, 100, 100));
    // ecs.AddComponent(m_WhiteRook2,   TransformComponent(350, -350, 100, 100));
    // ecs.AddComponent(m_WhiteKnight1, TransformComponent(-250, -350, 100, 100));
    // ecs.AddComponent(m_WhiteKnight2, TransformComponent(250, -350, 100, 100));
    // ecs.AddComponent(m_WhiteBishop1, TransformComponent(-150, -350, 100, 100));
    // ecs.AddComponent(m_WhiteBishop2, TransformComponent(150, -350, 100, 100));
    // ecs.AddComponent(m_WhiteQueen,   TransformComponent(-50, -350, 100, 100));
    // ecs.AddComponent(m_WhiteKing,    TransformComponent(50, -350, 100, 100));


    Astral::AssetRegistry& registry = Astral::Engine::Get().GetAssetManager().GetRegistry();
    // registry.SetAssetDirectoryPath(CHESS_ASSET_DIR);


    // Astral::Ref<Astral::Material> chessboardMaterial = registry.CreateAsset<Astral::Material>("chessboard.astmat");
    //
    // Astral::Ref<Astral::Material> blackPawnMaterial = registry.CreateAsset<Astral::Material>("black_pawn.astmat");
    // Astral::Ref<Astral::Material> blackRookMaterial = registry.CreateAsset<Astral::Material>("black_rook.astmat");
    // Astral::Ref<Astral::Material> blackKnightMaterial = registry.CreateAsset<Astral::Material>("black_knight.astmat");
    // Astral::Ref<Astral::Material> blackBishopMaterial = registry.CreateAsset<Astral::Material>("black_bishop.astmat");
    // Astral::Ref<Astral::Material> blackQueenMaterial = registry.CreateAsset<Astral::Material>("black_queen.astmat");
    // Astral::Ref<Astral::Material> blackKingMaterial = registry.CreateAsset<Astral::Material>("black_king.astmat");
    //
    // Astral::Ref<Astral::Material> whitePawnMaterial = registry.CreateAsset<Astral::Material>("white_pawn.astmat");
    // Astral::Ref<Astral::Material> whiteRookMaterial = registry.CreateAsset<Astral::Material>("white_rook.astmat");
    // Astral::Ref<Astral::Material> whiteKnightMaterial = registry.CreateAsset<Astral::Material>("white_knight.astmat");
    // Astral::Ref<Astral::Material> whiteBishopMaterial = registry.CreateAsset<Astral::Material>("white_bishop.astmat");
    // Astral::Ref<Astral::Material> whiteQueenMaterial = registry.CreateAsset<Astral::Material>("white_queen.astmat");
    // Astral::Ref<Astral::Material> whiteKingMaterial = registry.CreateAsset<Astral::Material>("white_king.astmat");
    //
    // Astral::Ref<Astral::Mesh> mesh = registry.GetAsset<Astral::Mesh>("Models/WaterBendingGauntlet.fbx");
    // Astral::Ref<Astral::Material> material = registry.GetAsset<Astral::Material>("Models/WaterBendingGauntlet.astmat");

    // m_Mesh = ecs.CreateEntity("Mesh");
    // ecs.AddComponent(m_Mesh, TransformComponent({0,0,500}, {500, 500, 500}));
    // ecs.AddComponent(m_Mesh, Astral::MeshComponent(material, mesh));
    //
    // // Sprite Componen
    // ecs.AddComponent(m_ChessBoard, Astral::SpriteComponent(chessboardMaterial));
    //
    // // Black Pieces
    // ecs.AddComponent(m_BlackPawn1, Astral::SpriteComponent(blackPawnMaterial));
    // ecs.AddComponent(m_BlackPawn2, Astral::SpriteComponent(blackPawnMaterial));
    // ecs.AddComponent(m_BlackPawn3, Astral::SpriteComponent(blackPawnMaterial));
    // ecs.AddComponent(m_BlackPawn4, Astral::SpriteComponent(blackPawnMaterial));
    // ecs.AddComponent(m_BlackPawn5, Astral::SpriteComponent(blackPawnMaterial));
    // ecs.AddComponent(m_BlackPawn6, Astral::SpriteComponent(blackPawnMaterial));
    // ecs.AddComponent(m_BlackPawn7, Astral::SpriteComponent(blackPawnMaterial));
    // ecs.AddComponent(m_BlackPawn8, Astral::SpriteComponent(blackPawnMaterial));
    // ecs.AddComponent(m_BlackRook1, Astral::SpriteComponent(blackRookMaterial));
    // ecs.AddComponent(m_BlackRook2, Astral::SpriteComponent(blackRookMaterial));
    // ecs.AddComponent(m_BlackKnight1, Astral::SpriteComponent(blackKnightMaterial));
    // ecs.AddComponent(m_BlackKnight2, Astral::SpriteComponent(blackKnightMaterial));
    // ecs.AddComponent(m_BlackBishop1, Astral::SpriteComponent(blackBishopMaterial));
    // ecs.AddComponent(m_BlackBishop2, Astral::SpriteComponent(blackBishopMaterial));
    // ecs.AddComponent(m_BlackQueen, Astral::SpriteComponent(blackQueenMaterial));
    // ecs.AddComponent(m_BlackKing, Astral::SpriteComponent(blackKingMaterial));
    //
    // // White Pieces
    // ecs.AddComponent(m_WhitePawn1, Astral::SpriteComponent(whitePawnMaterial));
    // ecs.AddComponent(m_WhitePawn2, Astral::SpriteComponent(whitePawnMaterial));
    // ecs.AddComponent(m_WhitePawn3, Astral::SpriteComponent(whitePawnMaterial));
    // ecs.AddComponent(m_WhitePawn4, Astral::SpriteComponent(whitePawnMaterial));
    // ecs.AddComponent(m_WhitePawn5, Astral::SpriteComponent(whitePawnMaterial));
    // ecs.AddComponent(m_WhitePawn6, Astral::SpriteComponent(whitePawnMaterial));
    // ecs.AddComponent(m_WhitePawn7, Astral::SpriteComponent(whitePawnMaterial));
    // ecs.AddComponent(m_WhitePawn8, Astral::SpriteComponent(whitePawnMaterial));
    // ecs.AddComponent(m_WhiteRook1, Astral::SpriteComponent(whiteRookMaterial));
    // ecs.AddComponent(m_WhiteRook2, Astral::SpriteComponent(whiteRookMaterial));
    // ecs.AddComponent(m_WhiteKnight1, Astral::SpriteComponent(whiteKnightMaterial));
    // ecs.AddComponent(m_WhiteKnight2, Astral::SpriteComponent(whiteKnightMaterial));
    // ecs.AddComponent(m_WhiteBishop1, Astral::SpriteComponent(whiteBishopMaterial));
    // ecs.AddComponent(m_WhiteBishop2, Astral::SpriteComponent(whiteBishopMaterial));
    // ecs.AddComponent(m_WhiteQueen, Astral::SpriteComponent(whiteQueenMaterial));
    // ecs.AddComponent(m_WhiteKing, Astral::SpriteComponent(whiteKingMaterial));
}


void ChessEntities::DestroyEntities()
{
    Astral::ECS& ecs = Astral::Engine::Get().GetSceneManager().GetECS();

    // ecs.DeleteEntity(m_ChessBoard);
    //
    // ecs.DeleteEntity(m_BlackPawn1);
    // ecs.DeleteEntity(m_BlackPawn2);
    // ecs.DeleteEntity(m_BlackPawn3);
    // ecs.DeleteEntity(m_BlackPawn4);
    // ecs.DeleteEntity(m_BlackPawn5);
    // ecs.DeleteEntity(m_BlackPawn6);
    // ecs.DeleteEntity(m_BlackPawn7);
    // ecs.DeleteEntity(m_BlackPawn8);
    // ecs.DeleteEntity(m_BlackRook1);
    // ecs.DeleteEntity(m_BlackRook2);
    // ecs.DeleteEntity(m_BlackKnight1);
    // ecs.DeleteEntity(m_BlackKnight2);
    // ecs.DeleteEntity(m_BlackBishop1);
    // ecs.DeleteEntity(m_BlackBishop2);
    // ecs.DeleteEntity(m_BlackQueen);
    // ecs.DeleteEntity(m_BlackKing);
    //
    // ecs.DeleteEntity(m_WhitePawn1);
    // ecs.DeleteEntity(m_WhitePawn2);
    // ecs.DeleteEntity(m_WhitePawn3);
    // ecs.DeleteEntity(m_WhitePawn4);
    // ecs.DeleteEntity(m_WhitePawn5);
    // ecs.DeleteEntity(m_WhitePawn6);
    // ecs.DeleteEntity(m_WhitePawn7);
    // ecs.DeleteEntity(m_WhitePawn8);
    // ecs.DeleteEntity(m_WhiteRook1);
    // ecs.DeleteEntity(m_WhiteRook2);
    // ecs.DeleteEntity(m_WhiteKnight1);
    // ecs.DeleteEntity(m_WhiteKnight2);
    // ecs.DeleteEntity(m_WhiteBishop1);
    // ecs.DeleteEntity(m_WhiteBishop2);
    // ecs.DeleteEntity(m_WhiteQueen);
    // ecs.DeleteEntity(m_WhiteKing);
}


