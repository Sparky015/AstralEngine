/**
* @file ChessboardImGuiComponents.h
* @author Andrew Fagan
* @date 12/28/2024
*/

#pragma once

#include "Chessboard/Chessboard.h"
#include "MoveGeneration/PieceMoveList.h"

void PieceMoveListTableComponent(const char *tableName, const Game::Chessboard& chessboard, const Game::PieceMoveList& pieceMoveList);