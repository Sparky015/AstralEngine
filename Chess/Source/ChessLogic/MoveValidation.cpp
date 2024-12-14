/**
* @file MoveValidation.cpp
* @author Andrew Fagan
* @date 12/14/2024
*/
#include "MoveValidation.h"

#include "MoveGeneration/BoardMoveList.h"


namespace Game {

    bool IsMoveValid(const ChessBoard& chessBoard, ChessMove chessMove)
    {
        ASSERT(!chessMove.IsValid(), "Invalid move given!");

        const BoardMoveList& boardMoveList = chessBoard.GetMoveList();

        boardMoveList.
    }

}
