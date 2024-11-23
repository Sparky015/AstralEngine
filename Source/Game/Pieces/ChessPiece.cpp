//
// Created by Andrew Fagan on 11/21/24.
//

#include "ChessPiece.h"

namespace Game {

    void ChessPiece::GenerateMoves(const Board& board, const uint8_t pieceLocation, const PieceColor pieceColor)
    {
        ERROR("This should not be called.")
    }


    PieceType ChessPiece::GetType()
    {
        ERROR("This should not be called.");
        return PieceType::NONE;
    };

    const std::vector<uint8>& ChessPiece::GetRegularMoves() const
    {
        ERROR("This should not be called.");
        static std::vector<uint8> empty;
        return empty;
    }

    const std::vector<uint8>& ChessPiece::GetAttackingMoves() const
    {
        ERROR("This should not be called.");
        static std::vector<uint8> empty;
        return empty;
    }

}