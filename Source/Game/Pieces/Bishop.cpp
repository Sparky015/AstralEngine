//
// Created by Andrew Fagan on 11/18/24.
//

#include "Bishop.h"

namespace Game {

    Bishop::Bishop()
    {
        // Reserve the max amount of moves a bishop can make in the worst case
        m_RegularMoves.reserve(14);
        m_AttackingMoves.reserve(4);
    }

    void Bishop::GenerateMoves(const Board& board, const uint8_t pieceLocation, const PieceColor pieceColor)
    {

    }

} // Game