//
// Created by Andrew Fagan on 11/18/24.
//

#pragma once

#include "pch.h"
#include "Common/ChessPieceTypes.h"
#include "ChessPieceMoveList.h"

namespace Game {

    class KnightMoveList : public ChessPieceMoveList
    {
    public:
        KnightMoveList();

        void GenerateMoves(const Chessboard& board, SquareLocation pieceLocation, const PieceColor pieceColor) override;
        PieceType GetType() override { return PieceType::KNIGHT; }

        [[nodiscard]] inline const PieceMoveList& GetRegularMoves() const override { return m_RegularMoves; }
        [[nodiscard]] inline const PieceMoveList& GetAttackingMoves() const override { return m_AttackingMoves; }

    private:
        static constexpr uint8 MAX_NUMBER_OF_REGULAR_MOVES = 8;
        static constexpr uint8 MAX_NUMBER_OF_ATTACKING_MOVES = 8;

        PieceMoveList m_RegularMoves;
        PieceMoveList m_AttackingMoves;
    };

} // Game
