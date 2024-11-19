//
// Created by Andrew Fagan on 10/26/24.
//

#include "PieceInfo.h"

// TODO: Remove this function and replace it with the use of the Piece struct. I can't assume the Piece IDs will match the
// PieceTypes due to promotions. Therefore, the types could change, but I would still assume it to be the first type.
PieceType ConvertPieceIDToPieceType(PieceID pieceID)
{
    switch (pieceID)
    {
        case PAWN_1: case PAWN_2: case PAWN_3: case PAWN_4: case PAWN_5: case PAWN_6: case PAWN_7: case PAWN_8:
            return PieceType::PAWN;
        case ROOK_1: case ROOK_2:
            return PieceType::ROOK;
        case BISHOP_1: case BISHOP_2:
            return PieceType::BISHOP;
        case KNIGHT_1: case KNIGHT_2:
            return PieceType::KNIGHT;
        case KING:
            return PieceType::KING;
        case QUEEN:
            return PieceType::QUEEN;
    }
}


Piece::Piece(PieceID pieceID, PieceColor pieceColor)
{
    // Memory Layout: CTTTDDDD where C = Color, T = type, D = ID
    uint8 colorShifted = (uint8)pieceColor << 7;
    uint8 typeShifted = (uint8)ConvertPieceIDToPieceType(pieceID) << 4;
    m_Data = colorShifted | typeShifted | (uint8)pieceID;
}


PieceColor Piece::ReadColor()
{
    uint8 colorShifted = m_Data >> 7;
    return (PieceColor)colorShifted;
}


PieceID Piece::ReadPieceID()
{
    uint8 idShifted = m_Data & (0b00001111);
    return (PieceID)idShifted;
}


PieceType Piece::ReadType()
{
    uint8 typeShifted = m_Data >> 4;
    typeShifted &= 0b00000111;
    return (PieceType)typeShifted;
}


void Piece::SetColor(PieceColor pieceColor)
{
    m_Data &= 0b01111111; // Clear the color bit
    uint8 colorShifted = pieceColor << 7;
    m_Data |= colorShifted;
}


void Piece::SetPieceID(PieceID pieceID)
{
    m_Data &= 0b11110000; // Clear the ID bits
    m_Data |= pieceID;
}


void Piece::SetType(PieceType pieceType)
{
    m_Data &= 0b10001111; // Clear the type bits
    uint8 typeShifted = (uint8)pieceType << 4;
    m_Data |= typeShifted;
}

uint8 Piece::ReadInternalData() const
{
    return m_Data;
}


