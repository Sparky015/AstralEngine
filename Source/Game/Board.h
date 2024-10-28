/**
* @file Board.h
* @author Andrew Fagan
* @date 10/20/2024
*/

#pragma once

#include "pch.h"

#include "PieceInfo.h"


enum BoardLocation : uint8
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

    explicit Board(const bool isDefaultBoard);
    explicit Board(const std::string& FEN);

    ~Board() = default;


    /**
     * @brief Moves a specified piece from its current location to the specified location
     * @param color The color of the piece being moved.
     * @param pieceIndex The index of the piece being moved.
     * @param targetBoardLocation The location that the piece is being moved to. Expected range is 0-63 inclusive.
     * @throws std::logic_error Errors when the square being moved to is already occupied.
     * @throws std::out_of_range Errors when the specified location is not 0-63 inclusive.
     * @note The move piece functionality is separated from the take piece functionality, so it will throw errors when the intention
     * is to move to a square that is empty, but the square actually isn't. This assumes that there is a valid check being made
     * to see if the move is possible prior to this function's call.
     */
    void MovePiece(const PieceColor color, const PieceID pieceID, const uint8 targetBoardLocation);


    /**
     * @brief Moves a specified piece from its current location to a specified location and takes the piece on the square
     * @param color The color of the piece being moved.
     * @param pieceIndex The index of the piece being moved.
     * @param targetBoardLocation The location that the piece is being moved to. Expected range is 0-63 inclusive.
     * @throws std::logic_error Errors when the square being moved to is empty.
     * @throws std::out_of_range Errors when the specified location is not 0-63 inclusive.
     * @note The take piece functionality is separated the move piece functionality, so it will throw errors when the intention
     * is to move to a square that is occupied, but the square actually isn't. This assumes that there is a valid check being made
     * to see if the take is possible prior to this function's call.
     */
     void TakePiece(const PieceColor color, const PieceID pieceID, const uint8 targetBoardLocation);

     /**
      * @brief Updates a pawn to the specified promotion type
      * @param color The color of the piece being moved.
      * @param pieceIndex The index of the piece being moved.
      * @param promotionType The type of piece that the pawn in being promoted to.
      * @throws std::out_of_range Errors when the specified location is not 0-63 inclusive.
      */
      void PromotePawn(const PieceColor color, const PieceID pieceID, const PieceType promotionType) const;


    /**
     * @brief Reads a location of a piece in the chess board.
     * @param color The color of the piece being moved.
     * @param pieceID The ID of the piece that is being read.
     * @return The location of the specified piece (or 255 if piece is not on board)
     */
    uint8 ReadPieceLocation(const PieceColor color, const PieceID pieceID) const;


    /**
     * @brief Writes a location of a piece to the chess board
     * @param color The color of the piece being moved.
     * @param pieceID The ID of the piece that is being written to.
     * @param boardLocation The location that is being written to the piece.
     * @throws std::out_of_range Errors when the specified location is not 0-63 inclusive.
     */
    void WritePieceLocation(const PieceColor color, const PieceID pieceID, const uint8_t boardLocation);



    /**
    * @brief Reads the information of a square in the chess board.
    * @param squareLocation The color of the piece being moved.
    * @return The location of the specified piece (or 255 if piece is not on board)
    */
    Piece ReadBoardLocation(uint8 squareLocation) const;


    /**
     * @brief Writes a location of a piece to the chess board
     * @param color The color of the piece being moved.
     * @param pieceID The ID of the piece that is being written to.
     * @param boardLocation The location that is being written to the piece.
     * @throws std::out_of_range Errors when the specified location is not 0-63 inclusive.
     */
    void WriteBoardLocation(bool isOccupied, PieceType pieceType,  PieceColor pieceColor);

    /**
     * @brief Writes a piece to the board of a piece to the chess board
     * @param color The color of the piece being moved.
     * @param pieceID The ID of the piece that is being written to.
     * @param boardLocation The location that is being written to the piece.
     * @throws std::out_of_range Errors when the specified location is not 0-63 inclusive.
     */
    void WriteBoardLocation(Piece piece, uint8 boardLocation);



private:
    //TODO: Make this one byte with piece color and piece type as 3 and 1 bit respectively for each square. 8 bits total for two squares
    // Piece type none indicates empty square

    /**
    * @struct Contains properties of two combined horizontally adjacent squares in a chess board
    */
    struct TwoSquares
    {
        TwoSquares(PieceType pieceType1, PieceColor pieceColor1, PieceType pieceType2, PieceColor pieceColor2);

        PieceType ReadSquareOneType();
        PieceColor ReadColorOne();
        void WriteSquareOneType(PieceType type);
        void WriteSquareOneColor(PieceColor color);

        PieceType ReadSquareTwoType();
        PieceColor ReadColorTwo();
        void WriteSquareTwoType(PieceType type);
        void WriteSquareTwoColor(PieceColor color);

    private:
        uint8 m_Data;
    };

    struct InternalBoardRepresentation
    {
        InternalBoardRepresentation(std::array<TwoSquares, 32> m_Board);

        PieceType ReadType(uint8 squareLocation);
        PieceColor ReadColor(uint8 squareLocation);

    private:
        std::array<TwoSquares, 32> m_Board;
    };


    //TODO: Profile different memory layouts for speed

    std::array<uint8, 16> m_BlackPieceLocations;
    std::array<uint8, 16> m_WhitePieceLocations;
    InternalBoardRepresentation m_Board;
};