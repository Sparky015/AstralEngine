/**
* @file ChessMove.h
* @author Andrew Fagan
* @date 12/15/2024
*/

#pragma once

#include "Common/ChessPieceTypes.h"

namespace Game {

    enum class MoveType : bool
    {
        REGULAR,
        TAKE
    };


    struct ChessMove
    {
        ChessMove() : targetLocation(EMPTY), movingPieceColor(PieceColor::BLACK), movingPieceID(PIECE_1), moveType(MoveType::REGULAR) {}
        ChessMove(PieceID movingPieceID, PieceColor movingPieceColor, SquareLocation targetLocation, MoveType moveType)
            : targetLocation(targetLocation), movingPieceColor(movingPieceColor), movingPieceID(movingPieceID), moveType(moveType) {}

        SquareLocation targetLocation;
        PieceColor movingPieceColor;
        PieceID movingPieceID;
        MoveType moveType;

        [[nodiscard]] constexpr bool IsValid() const { return targetLocation.IsOnBoard(); }
    };

}