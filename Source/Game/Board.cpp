/**
* @file Board.cpp
* @author Andrew Fagan
* @date 10/20/2024
*/

#include "Board.h"




Board::Board()
{
    m_BlackPieceLocations = std::array<uint8, 16>({A7, B7, C7, D7, E7, F7, G7, H7, A8, B8, C8, D8, E8, F8, G8, H8});
    m_WhitePieceLocations = std::array<uint8, 16>({A2, B2, C2, D2, E2, F2, G2, H2, A1, B1, C1, D1, E1, F1, G1, H1});



    // Next, we take the location of each piece and populate that square with the piece's info

    for (uint8 pieceID = 0; pieceID < m_BlackPieceLocations.size(); pieceID++)
    {
        uint8 pieceLocation = m_BlackPieceLocations[pieceID];
        m_Board.WriteColor(BLACK, pieceLocation);
        m_Board.WriteType(ConvertPieceIDToPieceType((PieceID)pieceID), pieceLocation);
    }

    for (uint8 pieceID = 0; pieceID < m_WhitePieceLocations.size(); pieceID++)
    {
        uint8 pieceLocation = m_WhitePieceLocations[pieceID];
        m_Board.WriteColor(WHITE, pieceLocation);
        m_Board.WriteType(ConvertPieceIDToPieceType((PieceID)pieceID), pieceLocation);
    }

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

    if (m_Board.ReadType(targetBoardLocation) != PieceType::NONE)
    {
        throw std::logic_error("targetBoardLocation already has a piece on the square");
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


void Board::ClearBoard()
{
    m_Board = InternalBoardRepresentation();
}


Board::TwoSquares::TwoSquares(PieceType pieceTypeOne, PieceColor pieceColorOne, PieceType pieceTypeTwo,
                              PieceColor pieceColorTwo)
{
    uint8 colorOneShifted = (uint8)pieceColorOne << 7;
    uint8 typeOneShifted = (uint8)pieceTypeOne << 4;
    uint8 colorTwoShifted = (uint8)pieceColorTwo << 3;
    m_Data = colorOneShifted | typeOneShifted | colorTwoShifted | (uint8)pieceTypeTwo;
}


PieceType Board::TwoSquares::ReadSquareType(bool isSecondSquare)
{
    if (isSecondSquare)
    {
        uint8 squareTwoTypeShifted = m_Data & (0b00000111);
        return (PieceType)squareTwoTypeShifted;
    }
    else
    {
        uint8 squareOneTypeShifted = m_Data >> 4 & (0b00000111);
        return (PieceType)squareOneTypeShifted;
    }
}


PieceColor Board::TwoSquares::ReadSquareColor(bool isSecondSquare)
{
    if (isSecondSquare)
    {
        uint8 squareTwoColorShifted = m_Data >> 3 & (0b00000001);
        return (PieceColor)squareTwoColorShifted;
    }
    else
    {
        uint8 squareOneColorShifted = m_Data >> 7;
        return (PieceColor)squareOneColorShifted;
    }
}


void Board::TwoSquares::WriteSquareType(PieceType type, bool isSecondSquare)
{
    if (isSecondSquare)
    {
        m_Data &= 0b11111000;
        m_Data |= (uint8)type;
    }
    else
    {
        m_Data &= 0b10001111;
        m_Data |= (uint8)type << 4;
    }
}


void Board::TwoSquares::WriteSquareColor(PieceColor color, bool isSecondSquare)
{
    if (isSecondSquare)
    {
        m_Data &= 0b11110111;
        m_Data |= (uint8)color << 3;
    }
    else
    {
        m_Data &= 0b01111111;
        m_Data |= (uint8)color << 7;
    }

}


Board::TwoSquares::TwoSquares() : m_Data{0b01100110} {}


Board::InternalBoardRepresentation::InternalBoardRepresentation()
{
    m_InternalBoard.fill(TwoSquares(PieceType::NONE, BLACK, PieceType::NONE, BLACK));
}


Board::InternalBoardRepresentation::InternalBoardRepresentation(std::array<TwoSquares, 32> m_Board) : m_InternalBoard{m_Board} {}


PieceType Board::InternalBoardRepresentation::ReadType(uint8_t squareLocation)
{
    uint8 halfOfSquareLocation = squareLocation >> 1; // Divide by 2
    bool isSecondSquare = (squareLocation % 2); // Odd numbered squares work out to be the second square
    return m_InternalBoard[halfOfSquareLocation].ReadSquareType(isSecondSquare);
}


PieceColor Board::InternalBoardRepresentation::ReadColor(uint8_t squareLocation)
{
    uint8 halfOfSquareLocation = squareLocation >> 1; // Divide by 2
    bool isSecondSquare = (squareLocation % 2); // Odd numbered squares work out to be the second square
    return m_InternalBoard[halfOfSquareLocation].ReadSquareColor(isSecondSquare);
}


void Board::InternalBoardRepresentation::WriteType(PieceType pieceType, uint8_t squareLocation)
{
    uint8 halfOfSquareLocation = squareLocation >> 1;  // Divide by 2
    bool isSecondSquare = (squareLocation % 2); // Odd numbered squares work out to be the second square
    m_InternalBoard[halfOfSquareLocation].WriteSquareType(pieceType, isSecondSquare);
}


void Board::InternalBoardRepresentation::WriteColor(PieceColor pieceColor, uint8_t squareLocation)
{
    uint8 halfOfSquareLocation = squareLocation >> 1;  // Divide by 2
    bool isSecondSquare = (squareLocation % 2); // Odd numbered squares work out to be the second square
    m_InternalBoard[halfOfSquareLocation].WriteSquareColor(pieceColor, isSecondSquare);
}






