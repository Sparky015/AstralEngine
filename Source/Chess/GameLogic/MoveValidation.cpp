/**
* @file MoveValidation.cpp
* @author Andrew Fagan
* @date 12/14/2024
*/
#include "MoveValidation.h"

#include "../MoveGeneration/BoardMoveList.h"


namespace Game {

    bool IsMoveValid(const ChessBoard& chessBoard, const BoardMoveList& boardMoveList, ChessMove chessMove)
    {
        ASSERT(!chessMove.IsValid(), "Invalid move given!");

        if (chessBoard.GetActiveColor() != chessMove.movingPieceColor) { return false; }

        if (chessMove.moveType == MoveType::REGULAR)
        {
            const PieceMoveList& pieceMoveList = boardMoveList.GetRegularMoves(chessMove.movingPieceID, chessMove.movingPieceColor);
            return pieceMoveList.Contains(chessMove);
        }
        else
        {
            const PieceMoveList& pieceMoveList = boardMoveList.GetAttackingMoves(chessMove.movingPieceID, chessMove.movingPieceColor);
            return pieceMoveList.Contains(chessMove);
        }
    }

}
