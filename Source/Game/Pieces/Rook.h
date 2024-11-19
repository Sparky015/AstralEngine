//
// Created by Andrew Fagan on 11/18/24.
//

#pragma once

#include "pch.h"
#include "Game/PieceInfo.h"

namespace Game {

    class Rook
    {
    public:
        void GenerateMoves(const uint8 pieceLocation, const PieceColor pieceColor);
        [[nodiscard]] inline const std::array<uint8, 14>& GetRegularMoves() const { return m_RegularMoves; }
        [[nodiscard]] inline const std::array<uint8, 4>& GetAttackingMoves() const { return m_AttackingMoves; }

    private:
        std::array<uint8, 14> m_RegularMoves;
        std::array<uint8, 4> m_AttackingMoves;
    };

} // Game
