//
// Created by Andrew Fagan on 11/18/24.
//

#pragma once

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

        /** @brief Constructs a chess board with the standard set up. */
        Board();

        /** @brief Constructs a chess board with the set up of the given FEN sequence. */
        explicit Board(const std::string& FEN);

        ~Board() = default;


        /** @brief Moves a specified piece from its current location to the specified location
         * @param color The color of the piece being moved.
         * @param pieceIndex The index of the piece being moved.
         * @param targetBoardLocation The location that the piece is being moved to. Expected range is 0-63 inclusive.
         * @throws std::logic_error Errors when the square being moved to is already occupied.
         * @throws std::out_of_range Errors when the specified location is not 0-63 inclusive.
         * @note The move piece functionality is separated from the take piece functionality, so it will throw errors when the intention
         * is to move to a square that is empty, but the square actually isn't. This assumes that there is a valid check being made
         * to see if the move is possible prior to this function's call. */
        void MovePiece(const PieceColor color, const PieceID pieceID, const uint8 targetBoardLocation);


        /** @brief Moves a specified piece from its current location to a specified location and takes the piece on the square
         * @param color The color of the piece being moved.
         * @param pieceIndex The index of the piece being moved.
         * @param targetBoardLocation The location that the piece is being moved to. Expected range is 0-63 inclusive.
         * @throws std::logic_error Errors when the square being moved to is empty.
         * @throws std::out_of_range Errors when the specified location is not 0-63 inclusive.
         * @note The take piece functionality is separated the move piece functionality, so it will throw errors when the intention
         * is to move to a square that is occupied, but the square actually isn't. This assumes that there is a valid check being made
         * to see if the take is possible prior to this function's call. */
        void TakePiece(const PieceColor color, const PieceID pieceID, const uint8 targetBoardLocation);


        /** @brief Updates a pawn to the specified promotion type
         * @param color The color of the piece being moved.
         * @param pieceIndex The index of the piece being moved.
         * @param promotionType The type of piece that the pawn in being promoted to.
         * @throws std::logic_error Errors when the given piece ID is not a pawn OR when promotion type is not a
         * bishop, knight, rook, or queen. */
        void PromotePawn(const PieceColor color, const PieceID pieceID, const PieceType promotionType);


        /** @brief Reads a location of a piece in the chess board.
         * @param color The color of the piece being moved.
         * @param pieceID The ID of the piece that is being read.
         * @return The location of the specified piece (or 255 if piece is not on board) */
        [[nodiscard]] uint8 ReadPieceLocation(const PieceColor color, const PieceID pieceID) const;

        /** @brief Gets the type of a piece in the chess board.
         * @param color The color of the piece being moved.
         * @param pieceID The ID of the piece that is being read.
         * @return The piece type of the specified piece */
        [[nodiscard]] PieceType ReadPieceType(const PieceColor color, const PieceID pieceID) const;

        /** @brief Reads the color of the piece on the square of the given location. This assumes there is a piece on the
         * given location.
         * @param squareLocation The location of the square being read.
         * @throws std::logic_error Errors if there is not a piece on the square of the given location.
         * @throws std::out_of_range Errors if the squareLocation is not in a valid range of 0-63
         * @return The color of the piece at the given location. */
        [[nodiscard]] PieceColor ReadSquareColor(uint8 squareLocation) const;

        /** @brief Reads the type of the piece on the square of the given location. Returns Piece::NONE if there are no pieces on the square.
         * @param squareLocation The location of the square being read.
         * @throws std::out_of_range Errors if the squareLocation is not in a valid range of 0-63
         * @return The type of piece on the square at the location given. */
        [[nodiscard]] PieceType ReadSquareType(uint8 squareLocation) const;

        /** @brief Reads the the ID of the piece on the square. This assumes that there is a piece on the location given.
         * @param squareLocation The location of the square being read.
         * @throws std::logic_error Errors if there is not a piece on the square of the given location.
         * @throws std::out_of_range Errors if the squareLocation is not in a valid range of 0-63
         * @return The ID of the piece on the square at the given location. */
        [[nodiscard]] PieceID ReadSquarePieceID(uint8 squareLocation) const;


        /** @brief Returns the character that the piece represents at the given location.
         * @param squareLocation The location of the piece that is being converted.
         * @throws std::out_of_range Errors if the squareLocation is not in a valid range of 0-63
         * @return The character that represents the piece. */
        char GetCharacterOfPiece(uint8 squareLocation);

        /** @brief Clears the chess board of any pieces. Results in an empty chess board. */
        void ClearBoard();

        /** @brief Prints the chess board to the console. */
        void PrintBoardToConsole();

    private:

        /** @brief Writes the type of the piece on the square of the given location.
         * @param squareLocation The location of the square being read.
         * @param pieceType The type of the piece being written.
         * @throws std::out_of_range Errors if the squareLocation is not in a valid range of 0-63 */
        void WriteSquareType(uint8 squareLocation, PieceType pieceType);

        /** @brief Writes the color of the piece on the square of the given location.
         * @param squareLocation The location of the square being read.
         * @param The color of the piece being written.
         * @throws std::out_of_range Errors if the squareLocation is not in a valid range of 0-63 */
        void WriteSquareColor(uint8 squareLocation, PieceColor pieceColor);

        /** @brief Writes a location of a piece to the chess board
         * @param color The color of the piece being moved.
         * @param pieceID The ID of the piece that is being written to.
         * @param boardLocation The location that is being written to the piece.
         * @throws std::out_of_range Errors when the specified location is not 0-63 inclusive. */
        void WritePieceLocation(const PieceColor color, const PieceID pieceID, const uint8 boardLocation);


        //TODO: Profile different memory layouts for speed

        std::array<uint8, 16> m_BlackPieceLocations;
        std::array<uint8, 16> m_WhitePieceLocations;
        InternalBoardRepresentation m_Board;

    public:
        inline InternalBoardRepresentation& GetInternalBoard()
        { return m_Board; } /// For testing only!
    };

}