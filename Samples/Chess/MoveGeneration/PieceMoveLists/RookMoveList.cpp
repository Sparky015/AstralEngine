//
// Created by Andrew Fagan on 11/18/24.
//

#include "RookMoveList.h"

#include "MoveGeneration/MoveListGenerationUtils.h"

namespace Game {

    RookMoveList::RookMoveList()
    {
        // Reserve the max amount of moves a knight can make in the worst case
        m_RegularMoves.ReserveSpace(MAX_NUMBER_OF_REGULAR_MOVES);
        m_AttackingMoves.ReserveSpace(MAX_NUMBER_OF_ATTACKING_MOVES);
    }

    void RookMoveList::GenerateMoves(const Chessboard& board, const SquareLocation pieceLocation, const PieceColor pieceColor)
    {
        // Clear existing stored moves
        m_RegularMoves.Clear();
        m_AttackingMoves.Clear();

        int8 directionMultiplier = (pieceColor.IsWhite() ? 1 : -1);
        int8 moveStep;
        SquareLocation moveLocation;
        SquareLocation currentPieceLocation;

        // Checking each direction that the rook can go in
        constexpr std::array<int8, 4> directions = {UP, RIGHT, DOWN, LEFT};
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

        ASSERT(m_RegularMoves.Size() <= MAX_NUMBER_OF_REGULAR_MOVES, "Too many regular moves generated for rook!");
        ASSERT(m_AttackingMoves.Size() <= MAX_NUMBER_OF_ATTACKING_MOVES, "Too many attacking moves generated for rook!");
    }

} // Game