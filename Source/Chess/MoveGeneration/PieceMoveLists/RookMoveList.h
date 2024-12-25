//
// Created by Andrew Fagan on 11/18/24.
//

#pragma once

#include "pch.h"
#include "../../GameState/ChessPieceTypes.h"
#include "ChessPieceMoveList.h"
#include "../../ChessBoard/ChessBoard.h"

namespace Game {

    class RookMoveList : public ChessPieceMoveList
    {
    public:
        RookMoveList();

        void GenerateMoves(const ChessBoard& board, const SquareLocation pieceLocation, const PieceColor pieceColor) override;
        PieceType GetType() override { return PieceType::ROOK; }

        [[nodiscard]] inline const PieceMoveList& GetRegularMoves() const override { return m_RegularMoves; }
        [[nodiscard]] inline const PieceMoveList& GetAttackingMoves() const override { return m_AttackingMoves; }

    private:

        PieceMoveList m_RegularMoves;
        PieceMoveList m_AttackingMoves;
    };

} // Game
