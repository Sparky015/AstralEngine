//
// Created by Andrew Fagan on 11/18/24.
//

#pragma once

#include "pch.h"
#include "../../GameState/ChessPieceTypes.h"
#include "ChessPieceMoveList.h"
#include "../../Chessboard/Chessboard.h"

namespace Game {

    class BishopMoveList : public ChessPieceMoveList
    {
    public:
        BishopMoveList();

        void GenerateMoves(const Chessboard& board, SquareLocation pieceLocation, const PieceColor pieceColor) override;
        PieceType GetType() override { return PieceType::BISHOP; }

        [[nodiscard]] inline const PieceMoveList& GetRegularMoves() const override { return m_RegularMoves; }
        [[nodiscard]] inline const PieceMoveList& GetAttackingMoves() const override { return m_AttackingMoves; }

    private:
        PieceMoveList m_RegularMoves;
        PieceMoveList m_AttackingMoves;

//        std::array<uint8, 14> m_RegularMoves;
//        std::array<uint8, 4> m_AttackingMoves;
    };

} // Game
