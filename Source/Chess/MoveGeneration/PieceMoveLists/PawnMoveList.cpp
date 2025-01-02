//
// Created by Andrew Fagan on 11/18/24.
//

#include "PawnMoveList.h"

#include "MoveGeneration/MoveListGenerationUtils.h"

namespace Game {

    PawnMoveList::PawnMoveList()
    {
        // Reserve the max amount of moves a pawn can make in the worst case
        m_RegularMoves.ReserveSpace(MAX_NUMBER_OF_REGULAR_MOVES);
        m_AttackingMoves.ReserveSpace(MAX_NUMBER_OF_ATTACKING_MOVES);
    }

    void PawnMoveList::GenerateMoves(const Chessboard& board, const SquareLocation pieceLocation, const PieceColor pieceColor)
    {
        // Clear existing stored moves
        m_RegularMoves.Clear();
        m_AttackingMoves.Clear();

        // Check if an en passant attack move is possible
        PreviousMoveData lmb = board.GetPreviousMoveData();
        if (lmb.MovingPieceType == PieceType::PAWN)
        {
            if (pieceColor.IsWhite())
            {
                if (pieceLocation.IsInSameRow(A5))
                {
                    if (lmb.FinalPieceLocation == pieceLocation - 1)
                    {
                        m_AttackingMoves.AddMove(pieceLocation - 9);
                    }
                    else if (lmb.FinalPieceLocation == pieceLocation + 1)
                    {
                        m_AttackingMoves.AddMove(pieceLocation - 7);
                    }
                }
            }
            else
            {
                if (pieceLocation.IsInSameRow(A4))
                {
                    if (lmb.FinalPieceLocation == pieceLocation - 1)
                    {
                        m_AttackingMoves.AddMove(pieceLocation + 7);
                    }
                    else if (lmb.FinalPieceLocation == pieceLocation + 1)
                    {
                        m_AttackingMoves.AddMove(pieceLocation + 9);
                    }
                }
            }
        }

        int8 directionMultiplier = (pieceColor.IsWhite() ? 1 : -1);

        // Checking the regular pawn move
        int8 moveStep = UP * directionMultiplier;
        SquareLocation moveLocation = pieceLocation + moveStep;
        if (board.GetSquareType(moveLocation) == PieceType::NONE && IsMoveWithinBounds(moveLocation, moveStep))
        {
            m_RegularMoves.AddMove(moveLocation);
        }

        // Checking the attack pawn moves
        std::array<int8, 2> directions = {UP + LEFT, UP + RIGHT};
        for (int8 direction : directions)
        {
            moveStep = direction * directionMultiplier;
            moveLocation = pieceLocation + moveStep;
            if (board.GetSquareType(moveLocation) != PieceType::NONE
                && IsMoveWithinBounds(pieceLocation, moveStep)
                && board.GetSquareColor(moveLocation) != pieceColor)
            {
                m_AttackingMoves.AddMove(moveLocation);
            }
        }

        // Check if pawn can do a double step move
        if (pieceColor.IsWhite())
        {
            if (pieceLocation.IsInSameRow(A2))
            {
                if (board.GetSquareType(pieceLocation - 16) == PieceType::NONE &&
                    board.GetSquareType(pieceLocation - 8) == PieceType::NONE)
                {
                    m_RegularMoves.AddMove(pieceLocation - 16);
                }
            }
        }
        else
        {
            if (pieceLocation.IsInSameRow(A7))
            {
                if (board.GetSquareType(pieceLocation + 16) == PieceType::NONE &&
                    board.GetSquareType(pieceLocation + 8) == PieceType::NONE)
                {
                    m_RegularMoves.AddMove(pieceLocation + 16);
                }
            }
        }

        ASSERT(m_RegularMoves.Size() <= MAX_NUMBER_OF_REGULAR_MOVES, "Too many regular moves generated for pawn!");
        ASSERT(m_AttackingMoves.Size() <= MAX_NUMBER_OF_ATTACKING_MOVES, "Too many attacking moves generated for pawn!");
    }

} // Game