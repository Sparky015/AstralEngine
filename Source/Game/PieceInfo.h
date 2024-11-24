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
 * @enum This identifies the type of a piece.
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