//
// Created by Andrew Fagan on 10/21/24.
//

#pragma once

#include "pch.h"


/**
 * @enum This tracks the index of each piece in the Board's piece arrays.
 * @note The names of each ID is what the piece is at the start of the game.
*/
enum class PieceID : uint8
{
    PAWN_1,
    PAWN_2,
    PAWN_3,
    PAWN_4,
    PAWN_5,
    PAWN_6,
    PAWN_7,
    PAWN_8,
    BISHOP_1,
    BISHOP_2,
    KNIGHT_1,
    KNIGHT_2,
    ROOK_1,
    ROOK_2,
    QUEEN,
    KING
};

/**
 * @enum This identifies the type of a piece.
*/
enum class PieceType : uint8
{
    PAWN,
    BISHOP,
    KNIGHT,
    ROOK,
    QUEEN,
    KING
};

/**
* @enum This identifies the color of a piece
*/
enum class PieceColor : bool
{
    BLACK,
    WHITE
};
