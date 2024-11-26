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
    }

    void Queen::GenerateMoves(const Board& board, const uint8_t pieceLocation, const PieceColor pieccColor)
    {

    }

}
