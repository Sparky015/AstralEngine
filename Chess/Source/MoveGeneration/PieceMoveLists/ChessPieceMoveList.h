//
// Created by Andrew Fagan on 11/18/24.
//

#pragma once

#include "ChessPieceTypes.h"
#include "ChessBoard/ChessBoard.h"
#include "MoveGeneration/MoveListGenerationUtils.h"

namespace Game {

    class ChessPieceMoveList
    {
    public:
        ChessPieceMoveList() = default;
        virtual ~ChessPieceMoveList() = default;

        virtual void GenerateMoves(const ChessBoard& board, const uint8 pieceLocation, const PieceColor pieceColor) = 0;
        virtual PieceType GetType() = 0;

        [[nodiscard]] virtual const std::vector<uint8>& GetRegularMoves() const = 0;
        [[nodiscard]] virtual const std::vector<uint8>& GetAttackingMoves() const = 0;
    };


    enum BoardTransversal
    {
        UP = -8,
        DOWN = 8,
        LEFT = -1,
        RIGHT = 1,
        DIAGONAL_UP_RIGHT = -9,
        DIAGONAL_UP_LEFT = -7,
        DIAGONAL_DOWN_RIGHT = 9,
        DIAGONAL_DOWN_LEFT = 7,


        // These directions are just for the knight
        UP_LEFT = -17,
        UP_RIGHT = -15,
        DOWN_LEFT = 15,
        DOWN_RIGHT = 17,
        LEFT_UP = -10,
        LEFT_DOWN = 6,
        RIGHT_UP = -6,
        RIGHT_DOWN = 10
    };

}

