//
// Created by Andrew Fagan on 11/18/24.
//

#pragma once

#include "Game/PieceInfo.h"

namespace Game {

    /** @struct Contains the compacted properties of two horizontally adjacent squares in a chess board. */
    struct TwoSquares
    {
        TwoSquares();

        TwoSquares(PieceType pieceType1, PieceColor pieceColor1, PieceType pieceType2, PieceColor pieceColor2);

        /** @brief Reads the type of the selected square. */
        [[nodiscard]] PieceType ReadSquareType(bool isSecondSquare) const;

        /** @brief Reads the color of the selected square. */
        [[nodiscard]] PieceColor ReadSquareColor(bool isSecondSquare) const;

        /** @brief Writes the type to the selected square. */
        void WriteSquareType(PieceType type, bool isSecondSquare);

        /** @brief Writes the color to the selected square. */
        void WriteSquareColor(PieceColor color, bool isSecondSquare);

    private:
        uint8 m_Data;
    };


    /** @struct Representation of the chess board internally. Gives helper functions to access compacted data. */
    struct InternalBoardRepresentation
    {
        InternalBoardRepresentation();

        /** @brief Reads the type of the square at the location given. */
        [[nodiscard]] PieceType ReadSquareType(uint8 squareLocation) const;

        /** @brief Reads the color of the square at the location given. */
        [[nodiscard]] PieceColor ReadSquareColor(uint8 squareLocation) const;

        /** @brief Writes the type of the square at the location given. */
        void WriteSquareType(PieceType pieceType, uint8 squareLocation);

        /** @brief Writes the color of the square at the location given. */
        void WriteSquareColor(PieceColor pieceColor, uint8 squareLocation);

    private:
        std::array<TwoSquares, 32> m_InternalBoard;
    };

}