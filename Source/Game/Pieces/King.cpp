//
// Created by Andrew Fagan on 11/18/24.
//

#include "King.h"

namespace Game {

    King::King()
    {
        // Reserve the max amount of moves a king can make in the worst case
        m_RegularMoves.reserve(8);
        m_AttackingMoves.reserve(8);
    }

    void King::GenerateMoves(const Board& board, const uint8_t pieceLocation, const PieceColor pieceColor)
    {
        // Check if the king can castle
        KingCastleRights castleRights = board.GetCastleRights(pieceColor);
        // TODO: Invert the checking of empty squares depending on color
        if (castleRights.KingSide)
        {
            // Check if the squares to the left of the king are empty
            if (board.ReadSquareType(pieceLocation - 1) == PieceType::NONE &&
                board.ReadSquareType(pieceLocation - 2) == PieceType::NONE &&
                board.ReadSquareType(pieceLocation - 3) == PieceType::NONE)
            {
                m_RegularMoves.push_back(pieceLocation - 2);
            }
        }

        if (castleRights.QueenSide)
        {
            // Check if the squares to the right of the king are empty
            if (board.ReadSquareType(pieceLocation + 1) == PieceType::NONE &&
                board.ReadSquareType(pieceLocation + 2) == PieceType::NONE)
            {
                m_RegularMoves.push_back(pieceLocation + 2);
            }

        }


    }

}
