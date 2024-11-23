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

}

