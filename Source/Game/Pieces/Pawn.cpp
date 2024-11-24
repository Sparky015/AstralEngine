//
// Created by Andrew Fagan on 11/18/24.
//

#include "Pawn.h"

namespace Game {

    Pawn::Pawn()
    {
        // Reserve the max amount of moves a pawn can make in the worst case
        m_RegularMoves.reserve(2);
        m_AttackingMoves.reserve(2);

        // Fill each vector with EMPTY locations because 0 is a valid location on the board.
        std::fill(m_RegularMoves.begin(), m_RegularMoves.end(), EMPTY);
        std::fill(m_AttackingMoves.begin(), m_AttackingMoves.end(), EMPTY);
    }

    void Pawn::GenerateMoves(const Board& board, const uint8_t pieceLocation, const PieceColor pieceColor)
    {
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

    }

} // Game