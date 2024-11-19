//
// Created by Andrew Fagan on 11/18/24.
//

#pragma once

#include "pch.h"
#include "Game/PieceInfo.h"
#include "Game/Pieces/ChessPiece.h"

namespace Game {

    class Pawn : public ChessPiece
    {
    public:
        void GenerateMoves(const uint8 pieceLocation, const PieceColor pieceColor) override;
        [[nodiscard]] inline const std::array<uint8, 2>& GetRegularMoves() const { return m_RegularMoves; }
        [[nodiscard]] inline const std::array<uint8, 2>& GetAttackingMoves() const { return m_AttackingMoves; }

    private:
        std::array<uint8, 2> m_RegularMoves;
        std::array<uint8, 2> m_AttackingMoves;
    };

} // Game
