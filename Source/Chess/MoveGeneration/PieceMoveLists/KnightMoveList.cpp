//
// Created by Andrew Fagan on 11/18/24.
//

#include "KnightMoveList.h"

#include "MoveGeneration/MoveListGenerationUtils.h"

namespace Game {

    KnightMoveList::KnightMoveList()
    {
        // Reserve the max amount of moves a knight can make in the worst case
        m_RegularMoves.ReserveSpace(MAX_NUMBER_OF_REGULAR_MOVES);
        m_AttackingMoves.ReserveSpace(MAX_NUMBER_OF_ATTACKING_MOVES);
    }

    void KnightMoveList::GenerateMoves(const Chessboard& board, const SquareLocation pieceLocation, const PieceColor pieceColor)
    {
        // Clear existing stored moves
        m_RegularMoves.Clear();
        m_AttackingMoves.Clear();

        // Clear existing stored moves
        m_RegularMoves.Clear();
        m_AttackingMoves.Clear();

        int8 directionMultiplier = (pieceColor.IsWhite() ? 1 : -1);
        int8 moveStep;
        SquareLocation moveLocation;
        std::array<int8, 8> directions = {UP_LEFT, UP_RIGHT, RIGHT_UP, RIGHT_DOWN, DOWN_RIGHT, DOWN_LEFT, LEFT_DOWN, LEFT_UP};
        for (int8 direction : directions)
        {
            moveStep = direction * directionMultiplier;
            moveLocation = pieceLocation + moveStep;
            if (!IsMoveWithinBounds(pieceLocation, moveStep)) { continue; }
            if (board.GetSquareType(moveLocation) == PieceType::NONE)
            {
                m_RegularMoves.AddMove(moveLocation);
            }
            else if (board.GetSquareColor(moveLocation) != pieceColor)
            {
                m_AttackingMoves.AddMove(moveLocation);
            }
        }

        ASSERT(m_RegularMoves.Size() <= MAX_NUMBER_OF_REGULAR_MOVES, "Too many regular moves generated for knight!");
        ASSERT(m_AttackingMoves.Size() <= MAX_NUMBER_OF_ATTACKING_MOVES, "Too many attacking moves generated for knight!");
    }

} // Game