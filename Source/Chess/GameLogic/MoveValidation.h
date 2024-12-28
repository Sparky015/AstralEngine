/**
* @file MoveValidation.h
* @author Andrew Fagan
* @date 12/14/2024
*/

#pragma once

#include "../Chessboard/Chessboard.h"
#include "../MoveGeneration/BoardMoveList.h"

namespace Game {

    bool IsMoveValid(const Chessboard& chessBoard, const BoardMoveList& boardMoveList, ChessMove chessMove);

}
