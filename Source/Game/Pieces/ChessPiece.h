//
// Created by Andrew Fagan on 11/18/24.
//

#pragma once

#include "Game/PieceInfo.h"
#include "Game/Board/Board.h"

namespace Game {

    class ChessPiece
    {
    public:
        ChessPiece() = default;
        virtual ~ChessPiece() = default;

        virtual void GenerateMoves(const Board& board, const uint8 pieceLocation, const PieceColor pieceColor);
        virtual PieceType GetType();

        [[nodiscard]] virtual const std::vector<uint8>& GetRegularMoves() const;
        [[nodiscard]] virtual const std::vector<uint8>& GetAttackingMoves() const;
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
        DOWN_LEFT = 17,
        DOWN_RIGHT = 15,
        LEFT_UP = -6,
        LEFT_DOWN = -10,
        RIGHT_UP = 6,
        RIGHT_DOWN = 10
    };

}

