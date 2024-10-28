/**
* @file Board.cpp
* @author Andrew Fagan
* @date 10/20/2024
*/

#include "Board.h"




Board::Board(const bool isDefaultBoard)
{
//    if (isDefaultBoard)
//    {
//        m_BlackPieceLocations = std::array<uint8, 16>({A7, B7, C7, D7, E7, F7, G7, H7, A8, B8, C8, D8, E8, F8, G8, H8});
//        m_WhitePieceLocations = std::array<uint8, 16>({A2, B2, C2, D2, E2, F2, G2, H2, A1, B1, C1, D1, E1, F1, G1, H1});
//    }
//    else
//    {
//        m_BlackPieceLocations.fill(EMPTY);
//        m_WhitePieceLocations.fill(EMPTY);
//    }
//
//    // First, we populate every square as empty.
//
//    m_Board.fill({NONE,PieceType::NONE});
//
//
//
//    // Next, we take the location of each piece and populate that square with the piece's info
//
//    for (uint8 pieceID = 0; pieceID < m_BlackPieceLocations.size(); pieceID++)
//    {
//        uint8 pieceLocation = m_BlackPieceLocations[pieceID];
//        m_Board[pieceLocation].color = BLACK;
//        m_Board[pieceLocation].type = ConvertPieceIDToPieceType(static_cast<PieceID>(pieceID));
//    }
//
//    for (uint8 pieceID = 0; pieceID < m_WhitePieceLocations.size(); pieceID++)
//    {
//        uint8 pieceLocation = m_WhitePieceLocations[pieceID];
//        m_Board[pieceLocation].color = WHITE;
//        m_Board[pieceLocation].type = ConvertPieceIDToPieceType(static_cast<PieceID>(pieceID));
//    }

}

Board::Board(const std::string& FEN)
{

}


void Board::MovePiece(const PieceColor color, const PieceID pieceID, const uint8_t targetBoardLocation)
{
    if (targetBoardLocation < 0 || targetBoardLocation > 63)
    {
        throw std::out_of_range("targetBoardLocation is not between 0 and 63");
    }

    if (color == PieceColor::WHITE)
    {
        m_WhitePieceLocations[pieceID] = targetBoardLocation;
    }
    else
    {
        m_BlackPieceLocations[pieceID] = targetBoardLocation;
    }
}


void Board::TakePiece(const PieceColor color, const PieceID pieceID, const uint8_t targetBoardLocation)
{

}


void Board::PromotePawn(const PieceColor color, const PieceID pieceIndex, const PieceType promotionType) const
{

}


uint8 Board::ReadPieceLocation(const PieceColor color, const PieceID pieceID) const
{
    if (color == PieceColor::WHITE)
    {
        return m_WhitePieceLocations[pieceID];
    }
    else
    {
        return m_BlackPieceLocations[pieceID];
    }
}


void Board::WritePieceLocation(const PieceColor color, const PieceID pieceID, const uint8_t boardLocation)
{
    if (color == PieceColor::WHITE)
    {
        m_WhitePieceLocations[pieceID] = boardLocation;
    }
    else
    {
        m_BlackPieceLocations[pieceID] = boardLocation;
    }
}




PieceType Board::InternalBoardRepresentation::ReadType(uint8_t squareLocation)
{
    // If the square location is an even number, then read the type of the first square in the TwoSquare object at
    // the index of squareLocation divided by two, else read the type of the second square in the TwoSquare object.
    uint8 halfOfSquareLocation = squareLocation >> 1;
    return (squareLocation % 2 == 0) ? m_Board[halfOfSquareLocation].ReadSquareOneType() : m_Board[halfOfSquareLocation].ReadSquareTwoType();
}

PieceColor Board::InternalBoardRepresentation::ReadColor(uint8_t squareLocation)
{
    // If the square location is an even number, then read the color of the first square in the TwoSquare object at
    // the index of squareLocation divided by two, else read the color of the second square in the TwoSquare object.
    uint8 halfOfSquareLocation = squareLocation >> 1;
    return (squareLocation % 2 == 0) ? m_Board[halfOfSquareLocation].ReadColorOne() : m_Board[halfOfSquareLocation].ReadColorTwo();;
}

Board::InternalBoardRepresentation::InternalBoardRepresentation(std::array<TwoSquares, 32> m_Board) : m_Board{m_Board}
{}

Board::TwoSquares::TwoSquares(PieceType pieceTypeOne, PieceColor pieceColorOne, PieceType pieceTypeTwo,
                              PieceColor pieceColorTwo)
{
    uint8 colorOneShifted = (uint8)pieceColorOne << 7;
    uint8 typeOneShifted = (uint8)pieceTypeOne << 4;
    uint8 colorTwoShifted = (uint8)pieceColorTwo << 3;
    m_Data = colorOneShifted | typeOneShifted | colorTwoShifted | (uint8)pieceTypeTwo;
}

PieceType Board::TwoSquares::ReadSquareOneType()
{
    uint8 typeOneShifted = m_Data >> 4 & (0b00001111);
    return (PieceType)typeOneShifted;
}

PieceColor Board::TwoSquares::ReadColorOne()
{
    uint8 typeOneShifted = m_Data >> 7;
    return (PieceColor)typeOneShifted;
}


PieceType Board::TwoSquares::ReadSquareTwoType()
{
    uint8 typeOneShifted = m_Data & (0b00000111);
    return (PieceType)typeOneShifted;
}

PieceColor Board::TwoSquares::ReadColorTwo()
{
    uint8 typeOneShifted = m_Data >> 3 & (0b00000001);
    return (PieceColor)typeOneShifted;
}

void Board::TwoSquares::WriteSquareOneType(PieceType type)
{
    m_Data &= 0b10001111;
    m_Data |= (uint8)type << 4;
}

void Board::TwoSquares::WriteSquareTwoType(PieceType type)
{
    m_Data &= 0b11111000;
    m_Data |= (uint8)type;
}

void Board::TwoSquares::WriteSquareOneColor(PieceColor color)
{
    m_Data &= 0b01111111;
    m_Data |= (uint8)color << 7;
}

void Board::TwoSquares::WriteSquareTwoColor(PieceColor color)
{
    m_Data &= 0b11110111;
    m_Data |= (uint8)color << 3;
}