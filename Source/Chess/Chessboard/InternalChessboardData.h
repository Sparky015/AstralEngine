//
// Created by Andrew Fagan on 11/18/24.
//

#pragma once

#include "../GameState/ChessPieceTypes.h"

namespace Game {

    enum SquarePosition : bool
    {
        FIRST_SQUARE = false,
        SECOND_SQUARE = true
    };

    /** @struct Contains the compacted properties of two horizontally adjacent squares in a chess board. */
    struct TwoSquares
    {
        TwoSquares();

        TwoSquares(PieceType pieceType1, PieceColor pieceColor1, PieceType pieceType2, PieceColor pieceColor2);

        /** @brief Reads the type of the selected square. */
        [[nodiscard]] PieceType ReadSquareType(SquarePosition squarePosition) const;

        /** @brief Reads the color of the selected square. */
        [[nodiscard]] PieceColor ReadSquareColor(SquarePosition squarePosition) const;

        /** @brief Writes the type to the selected square. */
        void WriteSquareType(PieceType type, SquarePosition squarePosition);

        /** @brief Writes the color to the selected square. */
        void WriteSquareColor(PieceColor color, SquarePosition squarePosition);

    private:
        uint8 m_Data;
    };


    /** @struct Representation of the chess board internally. Gives helper functions to access compacted data. */
    struct InternalBoardRepresentation
    {
        InternalBoardRepresentation();

        /** @brief Reads the type of the square at the location given. */
        [[nodiscard]] PieceType ReadSquareType(SquareLocation squareLocation) const;

        /** @brief Reads the color of the square at the location given. */
        [[nodiscard]] PieceColor ReadSquareColor(SquareLocation squareLocation) const;

        /** @brief Writes the type of the square at the location given. */
        void WriteSquareType(PieceType pieceType, SquareLocation squareLocation);

        /** @brief Writes the color of the square at the location given. */
        void WriteSquareColor(PieceColor pieceColor, SquareLocation squareLocation);

    private:
        std::array<TwoSquares, 32> m_InternalBoard;
    };


    struct KingCastleRights
    {
        bool KingSide;
        bool QueenSide;
    };


    enum CastleRightFlags : uint8
    {
        QUEEN_SIDE = 1,
        KING_SIDE = 0b1 << 1
    };


    class BoardCastleRights
    {
    public:
        void SetCastleRight(PieceColor color, CastleRightFlags castleRights);
        void UnsetCastleRight(PieceColor color, CastleRightFlags castleRights);
        void SetAllFlags();
        void UnsetAllFlags();
        [[nodiscard]] KingCastleRights ReadCastleRights(PieceColor color) const;
    private:
        uint8 m_Data{0};
    };


    struct PreviousMoveData
    {
        PreviousMoveData();

        SquareLocation InitialPieceLocation;
        SquareLocation FinalPieceLocation;
        PieceType MovingPieceType;
        PieceID MovingPieceID;

        PieceType PieceTakenType; // Populate these if the last action was a take
        PieceID TakenPieceID;
    };

}