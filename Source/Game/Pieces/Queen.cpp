//
// Created by Andrew Fagan on 11/18/24.
//

#include "Queen.h"

namespace Game {

    Queen::Queen()
    {
        // Reserve the max amount of moves a knight can make in the worst case
        m_RegularMoves.reserve(28);
        m_AttackingMoves.reserve(8);

        // Fill each vector with EMPTY locations because 0 is a valid location on the board.
        std::fill(m_RegularMoves.begin(), m_RegularMoves.end(), EMPTY);
        std::fill(m_AttackingMoves.begin(), m_AttackingMoves.end(), EMPTY);
    }

    void Queen::GenerateMoves(const uint8_t pieceLocation, const PieceColor pieccColor)
    {

    }

}
