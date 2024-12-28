//
// Created by Andrew Fagan on 12/23/24.
//

#include "Checks.h"

namespace Game {

    bool IsKingInCheck(const Chessboard& chessboard, const BoardMoveList& boardMoveList, PieceColor pieceColor)
    {
        for (uint8 pieceID = PIECE_1; pieceID != PIECE_16 + 1; pieceID++)
        {
            if (CanPieceAttackKing(chessboard, boardMoveList, pieceColor, (PieceID)pieceID)) { LOG("IT HAPPENED!"); return true; }
        }

        return false;
    }


    bool CanPieceAttackKing(const Chessboard& chessboard, const BoardMoveList& boardMoveList, PieceColor pieceColor, PieceID pieceID)
    {
        PieceID kingPieceID = chessboard.GetKingPieceID(pieceColor);
        SquareLocation kingLocation = chessboard.GetPieceLocation(kingPieceID, pieceColor);
        return boardMoveList.GetAttackingMoves(pieceID, pieceColor).Contains(kingLocation);
    }

}