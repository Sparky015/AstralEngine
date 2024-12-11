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

    void Queen::GenerateMoves(const Board& board, const uint8_t pieceLocation, const PieceColor pieceColor)
    {
        int8 directionMultiplier = (pieceColor == PieceColor::WHITE ? 1 : -1);
        int8 moveStep;
        uint8 moveLocation;
        uint8 currentPieceLocation;

        // Checking each direction that the rook can go in
        std::array<int8, 8> directions = {UP, RIGHT, DOWN, LEFT, DIAGONAL_UP_LEFT, DIAGONAL_UP_RIGHT, DIAGONAL_DOWN_RIGHT, DIAGONAL_DOWN_LEFT};
        for (int8 direction : directions)
        {
            moveStep = direction * directionMultiplier;
            moveLocation = pieceLocation + moveStep;
            currentPieceLocation = pieceLocation;

            // Move onto next direction if the next move is out of bounds
            if (!IsWithinBounds(pieceLocation, moveStep)) {continue;}

            // We keep adding moves to the vector if the next square is empty.
            while (IsWithinBounds(currentPieceLocation, moveStep) &&
            board.ReadSquareType(moveLocation) == PieceType::NONE)
            {
                m_RegularMoves.push_back(moveLocation);
                currentPieceLocation = moveLocation;
                moveLocation += moveStep;
            }

            // The next move's square is not empty, therefore we check if we can attack it.
            if (IsWithinBounds(currentPieceLocation, moveStep) &&
                board.ReadSquareColor(moveLocation) != pieceColor)
            {
                m_AttackingMoves.push_back(moveLocation);
            }

        }
    }

}
