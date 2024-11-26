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
    }

    void Rook::GenerateMoves(const Board& board, const uint8_t pieceLocation, const PieceColor pieceColor)
    {

    }

} // Game