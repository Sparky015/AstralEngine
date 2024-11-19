//
// Created by Andrew Fagan on 10/21/24.
//

#pragma once

#include "pch.h"


/**
 * @enum This tracks the index of each piece in the Board's piece arrays.
 * @note The piece type for each piece ID can't be guaranteed to be constant during runtime, nor the same for each board.
*/
enum PieceID : uint8
{
    PIECE_1, // [TYPE USING STANDARD BOARD CONSTRUCTOR]
    PIECE_2, // [PAWN]
    PIECE_3, // [PAWN]
    PIECE_4, // [PAWN]
    PIECE_5, // [PAWN]
    PIECE_6, // [PAWN]
    PIECE_7, // [PAWN]
    PIECE_8, // [PAWN]

    PIECE_9, // [ROOK]
    PIECE_10, // [KNIGHT]
    PIECE_11, // [BISHOP]
    PIECE_12, // [QUEEN]
    PIECE_13, // [KING]
    PIECE_14, // [BISHOP]
    PIECE_15, // [KNIGHT]
    PIECE_16, // [ROOK]
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
    KING,

    NONE
};

/**
* @enum This identifies the color of a piece
*/
enum PieceColor : bool
{
    BLACK,
    WHITE
};


///**
//* @struct Represents a chess piece
//*/
//struct Piece
//{
//    Piece(PieceID pieceID, PieceColor pieceColor);
//
//    void SetColor(PieceColor pieceColor);
//    void SetType(PieceType pieceType);
//    void SetPieceID(PieceID pieceID);
//
//    PieceColor ReadColor();
//    PieceType ReadType();
//    PieceID ReadPieceID();
//
//private:
//    uint8 m_Data; /// This is where the piece type, piece color, and piece ID is stored.
//
//public:
//    uint8 ReadInternalData() const; // For testing only
//};