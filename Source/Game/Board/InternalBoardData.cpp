//
// Created by Andrew Fagan on 11/18/24.
//

#include "InternalBoardData.h"

namespace Game {

/*********************************************
                TwoSquares
**********************************************/

    TwoSquares::TwoSquares(PieceType pieceTypeOne, PieceColor pieceColorOne, PieceType pieceTypeTwo,
                                  PieceColor pieceColorTwo)
    {
        uint8 colorOneShifted = (uint8) pieceColorOne << 7;
        uint8 typeOneShifted = (uint8) pieceTypeOne << 4;
        uint8 colorTwoShifted = (uint8) pieceColorTwo << 3;
        m_Data = colorOneShifted | typeOneShifted | colorTwoShifted | (uint8) pieceTypeTwo;
    }


    PieceType TwoSquares::ReadSquareType(bool isSecondSquare) const
    {
        if (isSecondSquare)
        {
            uint8 squareTwoTypeShifted = m_Data & (0b00000111);
            return (PieceType) squareTwoTypeShifted;
        }
        else
        {
            uint8 squareOneTypeShifted = m_Data >> 4 & (0b00000111);
            return (PieceType) squareOneTypeShifted;
        }
    }


    PieceColor TwoSquares::ReadSquareColor(bool isSecondSquare) const
    {
        if (isSecondSquare)
        {
            uint8 squareTwoColorShifted = m_Data >> 3 & (0b00000001);
            return (PieceColor) squareTwoColorShifted;
        }
        else
        {
            uint8 squareOneColorShifted = m_Data >> 7;
            return (PieceColor) squareOneColorShifted;
        }
    }


    void TwoSquares::WriteSquareType(PieceType type, bool isSecondSquare)
    {
        if (isSecondSquare)
        {
            m_Data &= 0b11111000;
            m_Data |= (uint8) type;
        }
        else
        {
            m_Data &= 0b10001111;
            m_Data |= (uint8) type << 4;
        }
    }


    void TwoSquares::WriteSquareColor(PieceColor color, bool isSecondSquare)
    {
        if (isSecondSquare)
        {
            m_Data &= 0b11110111;
            m_Data |= (uint8) color << 3;
        }
        else
        {
            m_Data &= 0b01111111;
            m_Data |= (uint8) color << 7;
        }

    }


    TwoSquares::TwoSquares() : m_Data{0b01100110}
    {}


/*********************************************
          InternalBoardRepresentation
**********************************************/

    InternalBoardRepresentation::InternalBoardRepresentation()
    {
        m_InternalBoard.fill(TwoSquares(PieceType::NONE, BLACK, PieceType::NONE, BLACK));
    }


    PieceType InternalBoardRepresentation::ReadSquareType(uint8_t squareLocation) const
    {
        if (squareLocation > 63)
        { throw std::out_of_range("squareLocation is not a valid location on the chess board"); }
        uint8 halfOfSquareLocation = squareLocation >> 1; // Divide by 2
        bool isSecondSquare = (squareLocation % 2); // Odd numbered squares work out to be the second square
        return m_InternalBoard[halfOfSquareLocation].ReadSquareType(isSecondSquare);
    }


    PieceColor InternalBoardRepresentation::ReadSquareColor(uint8_t squareLocation) const
    {
        if (squareLocation > 63)
        { throw std::out_of_range("squareLocation is not a valid location on the chess board"); }
        uint8 halfOfSquareLocation = squareLocation >> 1; // Divide by 2
        bool isSecondSquare = (squareLocation % 2); // Odd numbered squares work out to be the second square
        return m_InternalBoard[halfOfSquareLocation].ReadSquareColor(isSecondSquare);
    }


    void InternalBoardRepresentation::WriteSquareType(PieceType pieceType, uint8_t squareLocation)
    {
        if (squareLocation > 63)
        { throw std::out_of_range("squareLocation is not a valid location on the chess board"); }
        uint8 halfOfSquareLocation = squareLocation >> 1;  // Divide by 2
        bool isSecondSquare = (squareLocation % 2); // Odd numbered squares work out to be the second square
        m_InternalBoard[halfOfSquareLocation].WriteSquareType(pieceType, isSecondSquare);
    }


    void InternalBoardRepresentation::WriteSquareColor(PieceColor pieceColor, uint8_t squareLocation)
    {
        if (squareLocation > 63)
        { throw std::out_of_range("squareLocation is not a valid location on the chess board"); }
        uint8 halfOfSquareLocation = squareLocation >> 1;  // Divide by 2
        bool isSecondSquare = (squareLocation % 2); // Odd numbered squares work out to be the second square
        m_InternalBoard[halfOfSquareLocation].WriteSquareColor(pieceColor, isSecondSquare);
    }

}