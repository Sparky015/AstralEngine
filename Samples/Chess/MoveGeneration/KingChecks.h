//
// Created by Andrew Fagan on 12/23/24.
//

#pragma once


#include "BoardMoveList.h"
#include "Common/ChessPieceTypes.h"


namespace Game {

    bool IsKingInCheck(const Chessboard& chessboard, const BoardMoveList& boardMoveList, PieceColor pieceColor);

    bool CanPieceAttackKing(const Chessboard& chessboard, const BoardMoveList& boardMoveList, PieceColor pieceColor, PieceID pieceID);

}