//
// Created by Andrew Fagan on 11/18/24.
//

#include "KingMoveList.h"

#include "MoveGeneration/MoveListGenerationUtils.h"

namespace Game {

    KingMoveList::KingMoveList()
    {
        // Reserve the max amount of moves a king can make in the worst case
        m_RegularMoves.ReserveSpace(MAX_NUMBER_OF_REGULAR_MOVES);
        m_AttackingMoves.ReserveSpace(MAX_NUMBER_OF_ATTACKING_MOVES);
    }

    void KingMoveList::GenerateMoves(const Chessboard& board, const SquareLocation pieceLocation, const PieceColor pieceColor)
    {
        // Clear existing stored moves
        m_RegularMoves.Clear();
        m_AttackingMoves.Clear();

        // Check if the king can castle
        KingCastleRights castleRights = board.GetCastleRights(pieceColor);

        if (castleRights.QueenSide)
        {
            // Check if the squares to the left of the king are empty
            if (board.GetSquareType(pieceLocation - 1) == PieceType::NONE &&
                board.GetSquareType(pieceLocation - 2) == PieceType::NONE &&
                board.GetSquareType(pieceLocation - 3) == PieceType::NONE)
            {
                m_RegularMoves.AddMove(pieceLocation - 2);
            }
        }

        if (castleRights.KingSide)
        {
            // Check if the squares to the right of the king are empty
            if (board.GetSquareType(pieceLocation + 1) == PieceType::NONE &&
                board.GetSquareType(pieceLocation + 2) == PieceType::NONE)
            {
                m_RegularMoves.AddMove(pieceLocation + 2);
            }

        }

        int8 directionMultiplier = (pieceColor.IsWhite() ? 1 : -1);
        std::array<int8, 8> directions = {UP, RIGHT, DOWN, LEFT, DIAGONAL_UP_LEFT, DIAGONAL_UP_RIGHT, DIAGONAL_DOWN_RIGHT, DIAGONAL_DOWN_LEFT};
        for (int8 direction : directions)
        {
            int8 moveStep = direction * directionMultiplier;
            SquareLocation moveLocation = pieceLocation + moveStep;
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

        ASSERT(m_RegularMoves.Size() <= MAX_NUMBER_OF_REGULAR_MOVES, "Too many regular moves generated for king!");
        ASSERT(m_AttackingMoves.Size() <= MAX_NUMBER_OF_ATTACKING_MOVES, "Too many attacking moves generated for king!");
    }

}
