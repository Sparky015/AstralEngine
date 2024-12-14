//
// Created by Andrew Fagan on 11/18/24.
//

#include "KnightMoveList.h"

namespace Game {

    KnightMoveList::KnightMoveList()
    {
        // Reserve the max amount of moves a knight can make in the worst case
        m_RegularMoves.reserve(8);
        m_AttackingMoves.reserve(8);
    }

    void KnightMoveList::GenerateMoves(const ChessBoard& board, const uint8_t pieceLocation, const PieceColor pieceColor)
    {
        int8 directionMultiplier = (pieceColor == PieceColor::WHITE ? 1 : -1);
        int8 moveStep;
        uint8 moveLocation;
        std::array<int8, 8> directions = {UP_LEFT, UP_RIGHT, RIGHT_UP, RIGHT_DOWN, DOWN_RIGHT, DOWN_LEFT, LEFT_DOWN, LEFT_UP};
        for (int8 direction : directions)
        {
            moveStep = direction * directionMultiplier;
            moveLocation = pieceLocation + moveStep;
            if (!IsMoveWithinBounds(pieceLocation, moveStep)) { continue; }
            if (board.ReadSquareType(moveLocation) == PieceType::NONE)
            {
                m_RegularMoves.push_back(moveLocation);
            }
            else if (board.ReadSquareColor(moveLocation) != pieceColor)
            {
                m_AttackingMoves.push_back(moveLocation);
            }
        }
    }

} // Game