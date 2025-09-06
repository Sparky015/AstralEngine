/**
* @file ChessboardComponents.h
* @author Andrew Fagan
* @date 12/28/2024
*/

#pragma once

#include "Chessboard/Chessboard.h"
#include "MoveGeneration/BoardMoveList.h"

void PieceMoveListTableComponent(const Game::Chessboard& chessboard, const Game::BoardMoveList& boardMoveList);
