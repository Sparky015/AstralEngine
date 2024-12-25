//
// Created by Andrew Fagan on 10/21/24.
//

#pragma once

#include "pch.h"

#include "PieceColor.h"
#include "SquareLocation.h"

/**
 * @enum PieceID This tracks the index of each piece in the Board's piece arrays.
 * @note The piece type for each piece ID can't be guaranteed to be constant during runtime, nor the same for each board.
*/
enum PieceID : uint8
{
    PIECE_1,
    PIECE_2,
    PIECE_3,
    PIECE_4,
    PIECE_5,
    PIECE_6,
    PIECE_7,
    PIECE_8,

    PIECE_9,
    PIECE_10,
    PIECE_11,
    PIECE_12,
    PIECE_13,
    PIECE_14,
    PIECE_15,
    PIECE_16
};

/**
 * @enum PieceType This identifies the type of a piece.
*/
enum class PieceType : uint8
{
    NONE,

    PAWN,
    BISHOP,
    KNIGHT,
    ROOK,
    QUEEN,
    KING
};




