//
// Created by Andrew Fagan on 11/18/24.
//

#include "PawnMoveList.h"

namespace Game {

    PawnMoveList::PawnMoveList()
    {
        // Reserve the max amount of moves a pawn can make in the worst case
        m_RegularMoves.reserve(2);
        m_AttackingMoves.reserve(2);
    }

    void PawnMoveList::GenerateMoves(const ChessBoard& board, const uint8_t pieceLocation, const PieceColor pieceColor)
    {
        // Check if an en passant attack move is possible
        LastMoveBuffer lmb = board.GetLastMoveBuffer();
        if (lmb.MovingPieceType == PieceType::PAWN)
        {
            if (pieceColor == PieceColor::WHITE)
            {
                if (pieceLocation >= A5 && pieceLocation <= H5)
                {
                    if (lmb.FinalPieceLocation == pieceLocation - 1)
                    {
                        m_AttackingMoves.push_back(pieceLocation - 9);
                    }
                    else if (lmb.FinalPieceLocation == pieceLocation + 1)
                    {
                        m_AttackingMoves.push_back(pieceLocation - 7);
                    }
                }
            }
            else
            {
                if (pieceLocation >= A4 && pieceLocation <= H4)
                {
                    if (lmb.FinalPieceLocation == pieceLocation - 1)
                    {
                        m_AttackingMoves.push_back(pieceLocation + 7);
                    }
                    else if (lmb.FinalPieceLocation == pieceLocation + 1)
                    {
                        m_AttackingMoves.push_back(pieceLocation + 9);
                    }
                }
            }
        }

        int8 directionMultiplier = (pieceColor == PieceColor::WHITE ? 1 : -1);

        // Checking the regular pawn move
        int8 moveStep = UP * directionMultiplier;
        uint8 moveLocation = pieceLocation + moveStep;
        if (board.ReadSquareType(moveLocation) == PieceType::NONE && IsMoveWithinBounds(moveLocation, moveStep))
        {
            m_RegularMoves.push_back(moveLocation);
        }

        // Checking the attack pawn moves
        std::array<int8, 2> directions = {UP + LEFT, UP + RIGHT};
        for (int8 direction : directions)
        {
            moveStep = direction * directionMultiplier;
            moveLocation = pieceLocation + moveStep;
            if (board.ReadSquareType(moveLocation) != PieceType::NONE
                && IsMoveWithinBounds(pieceLocation, moveStep)
                && board.ReadSquareColor(moveLocation) != pieceColor)
            {
                m_AttackingMoves.push_back(moveLocation);
            }
        }

        // Check if pawn can do a double step move
        if (pieceColor == PieceColor::WHITE)
        {
            if (pieceLocation >= A2 && pieceLocation <= H2)
            {
                if (board.ReadSquareType(pieceLocation - 16) == PieceType::NONE &&
                    board.ReadSquareType(pieceLocation - 8) == PieceType::NONE)
                {
                    m_RegularMoves.push_back(pieceLocation - 16);
                }
            }
        }
        else
        {
            if (pieceLocation >= A7 && pieceLocation <= H7)
            {
                if (board.ReadSquareType(pieceLocation + 16) == PieceType::NONE &&
                    board.ReadSquareType(pieceLocation + 8) == PieceType::NONE)
                {
                    m_RegularMoves.push_back(pieceLocation + 16);
                }
            }
        }

    }

} // Game