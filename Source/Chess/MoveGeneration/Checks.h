//
// Created by Andrew Fagan on 12/23/24.
//

#pragma once

#include "pch.h"
#include "../GameState/ChessPieceTypes.h"
#include "BoardMoveList.h"


namespace Game {

    bool IsKingInCheck(const ChessBoard& chessboard, const BoardMoveList& boardMoveList, PieceColor pieceColor);

    bool CanPieceAttackKing(const ChessBoard& chessboard, const BoardMoveList& boardMoveList, PieceColor pieceColor, PieceID pieceID);

}