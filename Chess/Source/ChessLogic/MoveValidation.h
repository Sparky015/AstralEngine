/**
* @file MoveValidation.h
* @author Andrew Fagan
* @date 12/14/2024
*/

#pragma once

#include "../ChessPieceTypes.h"
#include "ChessBoard/ChessBoard.h"
#include "MoveGeneration/BoardMoveList.h"

namespace Game {


    enum class MoveType : bool
    {
        REGULAR,
        TAKE
    };

    struct ChessMove
    {
        ChessMove() : movingPieceColor(PieceColor::BLACK), movingPieceID(PIECE_1), moveType(MoveType::REGULAR), originatingLocation(255),
                      targetLocation(255) {}


        SquareLocation originatingLocation;
        SquareLocation targetLocation;
        PieceColor movingPieceColor;
        PieceID movingPieceID;
        MoveType moveType;

        inline bool IsValid() const { return originatingLocation == 255 && targetLocation == 255; }
    };


    bool IsMoveValid(const ChessBoard& chessBoard, const BoardMoveList& boardMoveList, ChessMove chessMove);

}
