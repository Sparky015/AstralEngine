//
// Created by Andrew Fagan on 11/18/24.
//

#pragma once

#include "Game/Board/Board.h"
#include "Game/Pieces/Pawn.h"
#include "Game/Pieces/Rook.h"
#include "Game/Pieces/Knight.h"
#include "Game/Pieces/Bishop.h"
#include "Game/Pieces/Queen.h"
#include "Game/Pieces/King.h"

namespace Game {

    // Before: 376 bytes with inheritance (ChessPiece base class)
    // After: 188 bytes without inheritance

    class MoveList
    {
    public:
        MoveList();
        ~MoveList() = default;

        Pawn& GetPawn(PieceID pieceID);
        Rook& GetRook(PieceID pieceID);
        Knight& GetKnight(PieceID pieceID);
        Bishop& GetBishop(PieceID pieceID);
        King& GetKing(PieceID pieceID);
        Queen& GetQueen(PieceID pieceID);

    private:

        Pawn m_Pawn1;
        Pawn m_Pawn2;
        Pawn m_Pawn3;
        Pawn m_Pawn4;
        Pawn m_Pawn5;
        Pawn m_Pawn6;
        Pawn m_Pawn7;
        Pawn m_Pawn8;
        Rook m_Rook1;
        Rook m_Rook2;
        Knight m_Knight1;
        Knight m_Knight2;
        Bishop m_Bishop1;
        Bishop m_Bishop2;
        King m_King;
        Queen m_Queen;
    };

} // Game
