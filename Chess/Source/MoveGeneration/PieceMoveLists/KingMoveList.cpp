//
// Created by Andrew Fagan on 11/18/24.
//

#include "KingMoveList.h"

namespace Game {

    KingMoveList::KingMoveList()
    {
        // Reserve the max amount of moves a king can make in the worst case
        m_RegularMoves.ReserveSpace(8);
        m_AttackingMoves.ReserveSpace(8);
    }

    void KingMoveList::GenerateMoves(const ChessBoard& board, const uint8_t pieceLocation, const PieceColor pieceColor)
    {
        // Check if the king can castle
        KingCastleRights castleRights = board.GetCastleRights(pieceColor);
        // TODO: Invert the checking of empty squares depending on color
        if (castleRights.QueenSide)
        {
            // Check if the squares to the left of the king are empty
            if (board.ReadSquareType(pieceLocation - 1) == PieceType::NONE &&
                board.ReadSquareType(pieceLocation - 2) == PieceType::NONE &&
                board.ReadSquareType(pieceLocation - 3) == PieceType::NONE)
            {
                m_RegularMoves.AddMove(pieceLocation - 2);
            }
        }

        if (castleRights.KingSide)
        {
            // Check if the squares to the right of the king are empty
            if (board.ReadSquareType(pieceLocation + 1) == PieceType::NONE &&
                board.ReadSquareType(pieceLocation + 2) == PieceType::NONE)
            {
                m_RegularMoves.AddMove(pieceLocation + 2);
            }

        }

        int8 directionMultiplier = (pieceColor == PieceColor::WHITE ? 1 : -1);
        int8 moveStep;
        uint8 moveLocation;
        std::array<int8, 8> directions = {UP, RIGHT, DOWN, LEFT, DIAGONAL_UP_LEFT, DIAGONAL_UP_RIGHT, DIAGONAL_DOWN_RIGHT, DIAGONAL_DOWN_LEFT};
        for (int8 direction : directions)
        {
            moveStep = direction * directionMultiplier;
            moveLocation = pieceLocation + moveStep;
            if (!IsMoveWithinBounds(pieceLocation, moveStep)) { continue; }
            if (board.ReadSquareType(moveLocation) == PieceType::NONE)
            {
                m_RegularMoves.AddMove(moveLocation);
            }
            else if (board.ReadSquareColor(moveLocation) != pieceColor)
            {
                m_AttackingMoves.AddMove(moveLocation);
            }
        }

    }

}
