//
// Created by Andrew Fagan on 11/18/24.
//

#pragma once

#include "pch.h"
#include "Game/PieceInfo.h"

namespace Game {

    class Knight
    {
    public:
        void GenerateMoves(const uint8 pieceLocation, const PieceColor pieceColor);
        [[nodiscard]] inline const std::array<uint8, 8>& GetRegularMoves() const { return m_RegularMoves; }
        [[nodiscard]] inline const std::array<uint8, 8>& GetAttackingMoves() const { return m_AttackingMoves; }

    private:
        std::array<uint8, 8> m_RegularMoves;
        std::array<uint8, 8> m_AttackingMoves;
    };

} // Game
