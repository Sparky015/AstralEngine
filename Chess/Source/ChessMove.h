/**
* @file ChessMove.h
* @author Andrew Fagan
* @date 12/15/2024
*/

#pragma once

#include "ChessPieceTypes.h"

namespace Game {

    enum class MoveType : bool
    {
        REGULAR,
        TAKE
    };


    struct ChessMove
    {
        ChessMove() : movingPieceColor(PieceColor::BLACK), movingPieceID(PIECE_1), moveType(MoveType::REGULAR), targetLocation(255) {}
        ChessMove(PieceID movingPieceID, PieceColor movingPieceColor, SquareLocation targetLocation, MoveType moveType)
            : targetLocation(targetLocation), movingPieceColor(movingPieceColor), movingPieceID(movingPieceID), moveType(moveType) {}

        SquareLocation targetLocation;
        PieceColor movingPieceColor;
        PieceID movingPieceID;
        MoveType moveType;

        inline bool IsValid() const { return targetLocation == 255; }
    };

}