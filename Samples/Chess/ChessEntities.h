//
// Created by Andrew Fagan on 12/11/24.
//

#pragma once

#include "Common/ChessPieceTypes.h"
#include "ECS/Entity.h"
#include "Renderer/Primitives/VertexArrayObject.h"

class ChessEntities
{
public:
    static void InitEntities(Graphics::VertexArrayObject* vertexArrayObject);
    static void DestroyEntities();
    static ECS::Entity GetEntity(SquareLocation location);


private:

    static ECS::Entity m_BlackPawn1;
    static ECS::Entity m_BlackPawn2;
    static ECS::Entity m_BlackPawn3;
    static ECS::Entity m_BlackPawn4;
    static ECS::Entity m_BlackPawn5;
    static ECS::Entity m_BlackPawn6;
    static ECS::Entity m_BlackPawn7;
    static ECS::Entity m_BlackPawn8;
    static ECS::Entity m_BlackRook1;
    static ECS::Entity m_BlackRook2;
    static ECS::Entity m_BlackKnight1;
    static ECS::Entity m_BlackKnight2;
    static ECS::Entity m_BlackBishop1;
    static ECS::Entity m_BlackBishop2;
    static ECS::Entity m_BlackQueen;
    static ECS::Entity m_BlackKing;

    static ECS::Entity m_WhitePawn1;
    static ECS::Entity m_WhitePawn2;
    static ECS::Entity m_WhitePawn3;
    static ECS::Entity m_WhitePawn4;
    static ECS::Entity m_WhitePawn5;
    static ECS::Entity m_WhitePawn6;
    static ECS::Entity m_WhitePawn7;
    static ECS::Entity m_WhitePawn8;
    static ECS::Entity m_WhiteRook1;
    static ECS::Entity m_WhiteRook2;
    static ECS::Entity m_WhiteKnight1;
    static ECS::Entity m_WhiteKnight2;
    static ECS::Entity m_WhiteBishop1;
    static ECS::Entity m_WhiteBishop2;
    static ECS::Entity m_WhiteQueen;
    static ECS::Entity m_WhiteKing;

    static ECS::Entity m_ChessBoard;

};
