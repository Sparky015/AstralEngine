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
    static Astral::Entity GetEntity(SquareLocation location);


private:

    static Astral::Entity m_BlackPawn1;
    static Astral::Entity m_BlackPawn2;
    static Astral::Entity m_BlackPawn3;
    static Astral::Entity m_BlackPawn4;
    static Astral::Entity m_BlackPawn5;
    static Astral::Entity m_BlackPawn6;
    static Astral::Entity m_BlackPawn7;
    static Astral::Entity m_BlackPawn8;
    static Astral::Entity m_BlackRook1;
    static Astral::Entity m_BlackRook2;
    static Astral::Entity m_BlackKnight1;
    static Astral::Entity m_BlackKnight2;
    static Astral::Entity m_BlackBishop1;
    static Astral::Entity m_BlackBishop2;
    static Astral::Entity m_BlackQueen;
    static Astral::Entity m_BlackKing;

    static Astral::Entity m_WhitePawn1;
    static Astral::Entity m_WhitePawn2;
    static Astral::Entity m_WhitePawn3;
    static Astral::Entity m_WhitePawn4;
    static Astral::Entity m_WhitePawn5;
    static Astral::Entity m_WhitePawn6;
    static Astral::Entity m_WhitePawn7;
    static Astral::Entity m_WhitePawn8;
    static Astral::Entity m_WhiteRook1;
    static Astral::Entity m_WhiteRook2;
    static Astral::Entity m_WhiteKnight1;
    static Astral::Entity m_WhiteKnight2;
    static Astral::Entity m_WhiteBishop1;
    static Astral::Entity m_WhiteBishop2;
    static Astral::Entity m_WhiteQueen;
    static Astral::Entity m_WhiteKing;

    static Astral::Entity m_ChessBoard;

};
