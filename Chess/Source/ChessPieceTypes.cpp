//
// Created by Andrew Fagan on 10/26/24.
//

#include "ChessPieceTypes.h"

// ece(PieceID pieceID, PieceColor pieceColor)
//{
//    // Memory Layout: CTTTDDDD where C = Color, T = type, D = ID
//    uint8 colorShifted = (uint8)pieceColor << 7;
//    uint8 typeShifted = (uint8)ConvertPieceIDToPieceType(pieceID) << 4;
//    m_Data = colorShifted | typeShifted | (uint8)pieceID;
//}
//
//
//PieceColor Piece::ReadColor()
//{
//    uint8 colorShifted = m_Data >> 7;
//    return (PieceColor)colorShifted;
//}
//
//
//PieceID Piece::ReadPieceID()
//{
//    uint8 idShifted = m_Data & (0b00001111);
//    return (PieceID)idShifted;
//}
//
//
//PieceType Piece::ReadType()
//{
//    uint8 typeShifted = m_Data >> 4;
//    typeShifted &= 0b00000111;
//    return (PieceType)typeShifted;
//}
//
//
//void Piece::SetColor(PieceColor pieceColor)
//{
//    m_Data &= 0b01111111; // Clear the color bit
//    uint8 colorShifted = pieceColor << 7;
//    m_Data |= colorShifted;
//}
//
//
//void Piece::SetPieceID(PieceID pieceID)
//{
//    m_Data &= 0b11110000; // Clear the ID bits
//    m_Data |= pieceID;
//}
//
//
//void Piece::SetType(PieceType pieceType)
//{
//    m_Data &= 0b10001111; // Clear the type bits
//    uint8 typeShifted = (uint8)pieceType << 4;
//    m_Data |= typeShifted;
//}
//
//uint8 Piece::ReadInternalData() const
//{
//    return m_Data;
//}


