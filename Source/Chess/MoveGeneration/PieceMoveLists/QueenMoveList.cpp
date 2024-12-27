//
// Created by Andrew Fagan on 11/18/24.
//

#include "QueenMoveList.h"

#include "MoveGeneration/MoveListGenerationUtils.h"

namespace Game {

    QueenMoveList::QueenMoveList()
    {
        // Reserve the max amount of moves a knight can make in the worst case
        m_RegularMoves.ReserveSpace(28);
        m_AttackingMoves.ReserveSpace(8);
    }

    void QueenMoveList::GenerateMoves(const ChessBoard& board, const SquareLocation pieceLocation, const PieceColor pieceColor)
    {
        int8 directionMultiplier = (pieceColor.IsWhite() ? 1 : -1);
        int8 moveStep;
        SquareLocation moveLocation;
        SquareLocation currentPieceLocation;

        // Checking each direction that the rook can go in
        std::array<int8, 8> directions = {UP, RIGHT, DOWN, LEFT, DIAGONAL_UP_LEFT, DIAGONAL_UP_RIGHT, DIAGONAL_DOWN_RIGHT, DIAGONAL_DOWN_LEFT};
        for (int8 direction : directions)
        {
            moveStep = direction * directionMultiplier;
            moveLocation = pieceLocation + moveStep;
            currentPieceLocation = pieceLocation;

            // Move onto next direction if the next move is out of bounds
            if (!IsMoveWithinBounds(pieceLocation, moveStep)) {continue;}

            // We keep adding moves to the vector if the next square is empty.
            while (IsMoveWithinBounds(currentPieceLocation, moveStep) &&
            board.GetSquareType(moveLocation) == PieceType::NONE)
            {
                m_RegularMoves.AddMove(moveLocation);
                currentPieceLocation = moveLocation;
                moveLocation += moveStep;
            }

            // The next move's square is not empty, therefore we check if we can attack it.
            if (IsMoveWithinBounds(currentPieceLocation, moveStep) &&
                board.GetSquareColor(moveLocation) != pieceColor)
            {
                m_AttackingMoves.AddMove(moveLocation);
            }

        }
    }

}
