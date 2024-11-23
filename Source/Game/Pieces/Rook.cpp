//
// Created by Andrew Fagan on 11/18/24.
//

#include "Rook.h"

namespace Game {

    Rook::Rook()
    {
        // Reserve the max amount of moves a knight can make in the worst case
        m_RegularMoves.reserve(14);
        m_AttackingMoves.reserve(4);

        // Fill each vector with EMPTY locations because 0 is a valid location on the board.
        std::fill(m_RegularMoves.begin(), m_RegularMoves.end(), EMPTY);
        std::fill(m_AttackingMoves.begin(), m_AttackingMoves.end(), EMPTY);
    }

    void Rook::GenerateMoves(const Board& board, const uint8_t pieceLocation, const PieceColor pieceColor)
    {

    }

} // Game