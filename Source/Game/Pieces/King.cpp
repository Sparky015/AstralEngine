//
// Created by Andrew Fagan on 11/18/24.
//

#include "King.h"

namespace Game {

    King::King()
    {
        // Reserve the max amount of moves a king can make in the worst case
        m_RegularMoves.reserve(8);
        m_AttackingMoves.reserve(8);

        // Fill each vector with EMPTY locations because 0 is a valid location on the board.
        std::fill(m_RegularMoves.begin(), m_RegularMoves.end(), EMPTY);
        std::fill(m_AttackingMoves.begin(), m_AttackingMoves.end(), EMPTY);
    }

    void King::GenerateMoves(const uint8_t pieceLocation, const PieceColor pieceColor)
    {
        ChessPiece::GenerateMoves(pieceLocation, pieceColor);
    }

}
