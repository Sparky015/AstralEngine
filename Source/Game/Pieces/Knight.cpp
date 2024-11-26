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
    }

    void Knight::GenerateMoves(const Board& board, const uint8_t pieceLocation, const PieceColor pieceColor)
    {

    }

} // Game