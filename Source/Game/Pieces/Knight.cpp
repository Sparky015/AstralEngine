//
// Created by Andrew Fagan on 11/18/24.
//

#include "Knight.h"

namespace Game {

    Knight::Knight()
    {
        // Reserve the max amount of moves a knight can make in the worst case
        m_RegularMoves.reserve(8);
        m_AttackingMoves.reserve(8);

        // Fill each vector with EMPTY locations because 0 is a valid location on the board.
        std::fill(m_RegularMoves.begin(), m_RegularMoves.end(), EMPTY);
        std::fill(m_AttackingMoves.begin(), m_AttackingMoves.end(), EMPTY);
    }

    void Knight::GenerateMoves(const Board& board, const uint8_t pieceLocation, const PieceColor pieceColor)
    {

    }

} // Game