//
// Created by Andrew Fagan on 11/18/24.
//

#include "KnightMoveList.h"

#include "MoveGeneration/MoveListGenerationUtils.h"

namespace Game {

    KnightMoveList::KnightMoveList()
    {
        // Reserve the max amount of moves a knight can make in the worst case
        m_RegularMoves.ReserveSpace(8);
        m_AttackingMoves.ReserveSpace(8);
    }

    void KnightMoveList::GenerateMoves(const ChessBoard& board, const SquareLocation pieceLocation, const PieceColor pieceColor)
    {
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
    }

} // Game