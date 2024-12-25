//
// Created by Andrew Fagan on 11/18/24.
//

#include "InternalChessBoardData.h"

namespace Game {

/*********************************************
                TwoSquares
**********************************************/

    TwoSquares::TwoSquares(PieceType pieceTypeOne, PieceColor pieceColorOne, PieceType pieceTypeTwo,
                                  PieceColor pieceColorTwo)
    {
        uint8 colorOneShifted = static_cast<uint8>(pieceColorOne.GetRawValue()) << 7;
        uint8 typeOneShifted = static_cast<uint8>(pieceTypeOne) << 4;
        uint8 colorTwoShifted = static_cast<uint8>(pieceColorTwo.GetRawValue()) << 3;
        m_Data = colorOneShifted | typeOneShifted | colorTwoShifted | static_cast<uint8>(pieceTypeTwo);
    }


    PieceType TwoSquares::ReadSquareType(SquarePosition squarePosition) const
    {
        if (squarePosition == SECOND_SQUARE)
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


    PieceColor TwoSquares::ReadSquareColor(SquarePosition squarePosition) const
    {
        if (squarePosition == SECOND_SQUARE)
        {
            uint8 squareTwoColorShifted = m_Data >> 3 & (0b00000001);
            return PieceColor::FromRawValue(squareTwoColorShifted);
        }
        else
        {
            uint8 squareOneColorShifted = m_Data >> 7;
            return PieceColor::FromRawValue(squareOneColorShifted);
        }
    }


    void TwoSquares::WriteSquareType(PieceType type, SquarePosition squarePosition)
    {
        if (squarePosition == SECOND_SQUARE)
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


    void TwoSquares::WriteSquareColor(PieceColor color, SquarePosition squarePosition)
    {
        if (squarePosition == SECOND_SQUARE)
        {
            m_Data &= 0b11110111;
            m_Data |= static_cast<uint8>(color.GetRawValue()) << 3;
        }
        else
        {
            m_Data &= 0b01111111;
            m_Data |= static_cast<uint8>(color.GetRawValue()) << 7;
        }

    }


    TwoSquares::TwoSquares() : m_Data{0b01100110}
    {}


/*********************************************
          InternalBoardRepresentation
**********************************************/

    InternalBoardRepresentation::InternalBoardRepresentation()
    {
        m_InternalBoard.fill(TwoSquares(PieceType::NONE, PieceColor::BLACK, PieceType::NONE, PieceColor::BLACK));
    }


    PieceType InternalBoardRepresentation::ReadSquareType(SquareLocation squareLocation) const
    {
        ASSERT(squareLocation.IsOnBoard(), "squareLocation is not a valid location on the chess board");
        uint8 halfOfSquareLocation = squareLocation.GetRawValue() >> 1; // Divide by 2
        bool isSecondSquare = (squareLocation.GetRawValue() % 2); // Odd numbered squares work out to be the second square
        return m_InternalBoard[halfOfSquareLocation].ReadSquareType(SquarePosition(isSecondSquare));
    }


    PieceColor InternalBoardRepresentation::ReadSquareColor(SquareLocation squareLocation) const
    {
        ASSERT(squareLocation.IsOnBoard(), "squareLocation is not a valid location on the chess board");
        uint8 halfOfSquareLocation = squareLocation.GetRawValue() >> 1; // Divide by 2
        bool isSecondSquare = (squareLocation.GetRawValue() % 2); // Odd numbered squares work out to be the second square
        return m_InternalBoard[halfOfSquareLocation].ReadSquareColor(SquarePosition(isSecondSquare));
    }


    void InternalBoardRepresentation::WriteSquareType(PieceType pieceType, SquareLocation squareLocation)
    {
        ASSERT(squareLocation.IsOnBoard(), "squareLocation is not a valid location on the chess board");
        uint8 halfOfSquareLocation = squareLocation.GetRawValue() >> 1;  // Divide by 2
        bool isSecondSquare = (squareLocation.GetRawValue() % 2); // Odd numbered squares work out to be the second square
        m_InternalBoard[halfOfSquareLocation].WriteSquareType(pieceType, SquarePosition(isSecondSquare));
    }


    void InternalBoardRepresentation::WriteSquareColor(PieceColor pieceColor, SquareLocation squareLocation)
    {
        ASSERT(squareLocation.IsOnBoard(), "squareLocation is not a valid location on the chess board");
        uint8 halfOfSquareLocation = squareLocation.GetRawValue() >> 1;  // Divide by 2
        bool isSecondSquare = (squareLocation.GetRawValue() % 2); // Odd numbered squares work out to be the second square
        m_InternalBoard[halfOfSquareLocation].WriteSquareColor(pieceColor, SquarePosition(isSecondSquare));
    }


    void BoardCastleRights::SetCastleRight(PieceColor color, CastleRightFlags castleRights)
    {
        if (color.IsWhite())
        {
            m_Data |= castleRights; // Set bit
        }
        else
        {
            m_Data |= (castleRights << 2);
        }
    }


    void BoardCastleRights::UnsetCastleRight(PieceColor color, CastleRightFlags castleRights)
    {
        if (color.IsWhite())
        {
            m_Data &= ~castleRights;
        }
        else
        {
            m_Data &= ~(castleRights << 2);
        }
    }


    KingCastleRights BoardCastleRights::ReadCastleRights(PieceColor color) const
    {
        bool queenSide, kingSide;
        if (color.IsWhite())
        {
            queenSide = m_Data & QUEEN_SIDE;
            kingSide = m_Data & KING_SIDE;
        }
        else
        {
            queenSide = m_Data & (QUEEN_SIDE << 2);
            kingSide = m_Data & (KING_SIDE << 2);
        }

        return KingCastleRights({kingSide, queenSide});
    }

}