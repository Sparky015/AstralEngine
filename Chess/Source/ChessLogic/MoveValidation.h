/**
* @file MoveValidation.h
* @author Andrew Fagan
* @date 12/14/2024
*/

#pragma once

#include "ChessBoard/ChessBoard.h"
#include "MoveGeneration/BoardMoveList.h"

namespace Game {

    bool IsMoveValid(const ChessBoard& chessBoard, const BoardMoveList& boardMoveList, ChessMove chessMove);

}
