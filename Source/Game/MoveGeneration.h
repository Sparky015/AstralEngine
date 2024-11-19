//
// Created by Andrew Fagan on 11/2/24.
//

#pragma once

#include "Game/Board/Board.h"
#include "Game/MoveList.h"

namespace Game {



    class MoveListGenerator
    {
    public:
        MoveListGenerator();
        ~MoveListGenerator() = default;

        void GenerateMoveList(MoveList* outMoveList, const Board* board, PieceColor pieceColor);

    private:
        void CalculatePawnMoveList(PieceID pieceID, PieceColor pieceColor, uint8 pieceLocation);
        void CalculateRookMoveList(PieceID pieceID, PieceColor pieceColor, uint8 pieceLocation);
        void CalculateBishopMoveList(PieceID pieceID, PieceColor pieceColor, uint8 pieceLocation);
        void CalculateKnightMoveList(PieceID pieceID, PieceColor pieceColor, uint8 pieceLocation);
        void CalculateQueenMoveList(PieceID pieceID, PieceColor pieceColor, uint8 pieceLocation);
        void CalculateKingMoveList(PieceID pieceID, PieceColor pieceColor, uint8 pieceLocation);

        MoveList* m_MoveList;
        const Board* m_Board;
    };

}