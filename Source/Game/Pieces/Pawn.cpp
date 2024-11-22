//
// Created by Andrew Fagan on 11/18/24.
//

#include "Pawn.h"

namespace Game {

    Pawn::Pawn()
    {
        // Reserve the max amount of moves a pawn can make in the worst case
        m_RegularMoves.reserve(2);
        m_AttackingMoves.reserve(2);

        // Fill each vector with EMPTY locations because 0 is a valid location on the board.
        std::fill(m_RegularMoves.begin(), m_RegularMoves.end(), EMPTY);
        std::fill(m_AttackingMoves.begin(), m_AttackingMoves.end(), EMPTY);
    }

    void Pawn::GenerateMoves(const uint8_t pieceLocation, const PieceColor pieceColor)
    {

    }

} // Game