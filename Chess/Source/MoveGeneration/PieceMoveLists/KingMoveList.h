//
// Created by Andrew Fagan on 11/18/24.
//

#pragma once

#include "pch.h"
#include "ChessPieceTypes.h"
#include "ChessPieceMoveList.h"
#include "ChessBoard/ChessBoard.h"

namespace Game {

    class KingMoveList : public ChessPieceMoveList
    {
    public:
        KingMoveList();

        void GenerateMoves(const ChessBoard& board, const uint8 pieceLocation, const PieceColor pieceColor) override;
        PieceType GetType() override { return PieceType::KING; }

        [[nodiscard]] inline const PieceMoveList& GetRegularMoves() const override { return m_RegularMoves; }
        [[nodiscard]] inline const PieceMoveList& GetAttackingMoves() const override { return m_AttackingMoves; }

    private:
        PieceMoveList m_RegularMoves;
        PieceMoveList m_AttackingMoves;
    };

}

