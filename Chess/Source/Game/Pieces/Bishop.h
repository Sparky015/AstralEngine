//
// Created by Andrew Fagan on 11/18/24.
//

#pragma once

#include "pch.h"
#include "../PieceInfo.h"
#include "ChessPiece.h"

namespace Game {

    class Bishop : public ChessPiece
    {
    public:
        Bishop();

        void GenerateMoves(const Board& board, const uint8 pieceLocation, const PieceColor pieceColor) override;
        PieceType GetType() override { return PieceType::BISHOP; }

        [[nodiscard]] inline const std::vector<uint8>& GetRegularMoves() const override { return m_RegularMoves; }
        [[nodiscard]] inline const std::vector<uint8>& GetAttackingMoves() const override { return m_AttackingMoves; }

    private:
        std::vector<uint8> m_RegularMoves;
        std::vector<uint8> m_AttackingMoves;

//        std::array<uint8, 14> m_RegularMoves;
//        std::array<uint8, 4> m_AttackingMoves;
    };

} // Game
