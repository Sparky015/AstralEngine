//
// Created by Andrew Fagan on 10/21/24.
//

#pragma once

#include "pch.h"


/**
 * @enum This tracks the index of each piece in the Board's piece arrays.
 * @note The names of each ID is what the piece is at the start of the game.
*/
enum PieceID : uint8
{
    //TODO: Look into changing this to a generic PIECE_1, PIECE_2, and so on because I can't encode the
    // Pawn types in this way because the usage should not be considering the type through the kind of piece ID alone.

    PAWN_1,
    PAWN_2,
    PAWN_3,
    PAWN_4,
    PAWN_5,
    PAWN_6,
    PAWN_7,
    PAWN_8,

    ROOK_1,
    KNIGHT_1,
    BISHOP_1,
    QUEEN,
    KING,
    BISHOP_2,
    KNIGHT_2,
    ROOK_2
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


PieceType ConvertPieceIDToPieceType(PieceID pieceID);

/**
* @struct Represents a chess piece
*/
struct Piece
{
    Piece(PieceID pieceID, PieceColor pieceColor);

    void SetColor(PieceColor pieceColor);
    void SetType(PieceType pieceType);
    void SetPieceID(PieceID pieceID);

    PieceColor ReadColor();
    PieceType ReadType();
    PieceID ReadPieceID();

private:
    uint8 m_Data; /// This is where the piece type, piece color, and piece ID is stored.

public:
    uint8 ReadInternalData() const; // For testing only
};