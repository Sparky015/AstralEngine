//
// Created by Andrew Fagan on 11/18/24.
//

#pragma once

#include "pch.h"
#include "Game/PieceInfo.h"
#include "Game/Pieces/ChessPiece.h"

namespace Game {

    class Rook : public ChessPiece
    {
    public:
        Rook();

        void GenerateMoves(const uint8 pieceLocation, const PieceColor pieceColor) override;
        PieceType GetType() override { return PieceType::ROOK; }

        [[nodiscard]] inline const std::vector<uint8>& GetRegularMoves() const override { return m_RegularMoves; }
        [[nodiscard]] inline const std::vector<uint8>& GetAttackingMoves() const override { return m_AttackingMoves; }

    private:

        std::vector<uint8> m_RegularMoves;
        std::vector<uint8> m_AttackingMoves;
    };

} // Game
