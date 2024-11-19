/**
* @file Board.h
* @author Andrew Fagan
* @date 10/20/2024
*/

#pragma once

#include "pch.h"

#include "Game/Board/InternalBoardData.h"
#include "Game/PieceInfo.h"

namespace Game {

    enum [[maybe_unused]] BoardLocation : uint8
    {
        A8, B8, C8, D8, E8, F8, G8, H8,
        A7, B7, C7, D7, E7, F7, G7, H7,
        A6, B6, C6, D6, E6, F6, G6, H6,
        A5, B5, C5, D5, E5, F5, G5, H5,
        A4, B4, C4, D4, E4, F4, G4, H4,
        A3, B3, C3, D3, E3, F3, G3, H3,
        A2, B2, C2, D2, E2, F2, G2, H2,
        A1, B1, C1, D1, E1, F1, G1, H1,

        EMPTY = 255
    };


/**
 * @class This holds the data representation of a chess board.
 */
    class Board
    {
    public:

        Board();
        explicit Board(const std::string& FEN);
        ~Board() = default;

        void MovePiece(const PieceColor color, const PieceID pieceID, const uint8 targetBoardLocation);
        void TakePiece(const PieceColor color, const PieceID pieceID, const uint8 targetBoardLocation);
        void PromotePawn(const PieceColor color, const PieceID pieceID, const PieceType promotionType);

        [[nodiscard]] uint8 ReadPieceLocation(const PieceColor color, const PieceID pieceID) const;
        [[nodiscard]] PieceType ReadPieceType(const PieceColor color, const PieceID pieceID) const;
        [[nodiscard]] PieceColor ReadSquareColor(uint8 squareLocation) const;
        [[nodiscard]] PieceType ReadSquareType(uint8 squareLocation) const;
        [[nodiscard]] PieceID ReadSquarePieceID(uint8 squareLocation) const;

        char GetCharacterOfPiece(uint8 squareLocation);
        void ClearBoard();
        void PrintBoardToConsole();

    private:

        void WritePieceLocation(const PieceColor color, const PieceID pieceID, const uint8 boardLocation);
        void WriteSquareType(uint8 squareLocation, PieceType pieceType);
        void WriteSquareColor(uint8 squareLocation, PieceColor pieceColor);

        //TODO: Profile different memory layouts for speed
        uint16 m_FullMoveCount;
        uint8 m_HalfMoveCount;
        uint8 m_BoardState; // Holds castling rights and active color
        std::array<uint8, 16> m_BlackPieceLocations; // These two contain the location info of a piece
        std::array<uint8, 16> m_WhitePieceLocations;
        InternalBoardRepresentation m_Board; // This contains the piece type and color of a piece

    public:
        inline InternalBoardRepresentation& GetInternalBoard()
        { return m_Board; } /// For testing only!
    };

}