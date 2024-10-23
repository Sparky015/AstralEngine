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
    A1, A2, A3, A4, A5, A6, A7, A8,
    B1, B2, B3, B4, B5, B6, B7, B8,
    C1, C2, C3, C4, C5, C6, C7, C8,
    D1, D2, D3, D4, D5, D6, D7, D8,
    E1, E2, E3, E4, E5, E6, E7, E8,
    F1, F2, F3, F4, F5, F6, F7, F8,
    G1, G2, G3, G4, G5, G6, G7, G8,
    H1, H2, H3, H4, H5, H6, H7, H8
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
      void PromotePawn(const PieceColor color, const PieceID pieceIndex, const PieceType promotionType) const;


    /**
     * @brief Reads a location of a piece in the chess board.
     * @param color The color of the piece being moved.
     * @param pieceID The ID of the piece that is being read.
     * @return The location of the specified piece (or 255 if piece is not on board)
     */
    uint8 ReadBoardLocation(const PieceColor color, const PieceID pieceID) const;


    /**
     * @brief Writes a location of a piece to the chess board
     * @param color The color of the piece being moved.
     * @param pieceID The ID of the piece that is being written to.
     * @param boardLocation The location that is being written to the piece.
     * @throws std::out_of_range Errors when the specified location is not 0-63 inclusive.
     */
    void WriteBoardLocation(const PieceColor color, const PieceID pieceID, const uint8 boardLocation);

private:

    // TODO: Improve the information density of the piece types
    std::array<uint8, 16> m_BlackPieceLocations;
    std::array<PieceType, 16> m_BlackPieceTypes;
    std::array<uint8, 16> m_WhitePieceLocations;
    std::array<PieceType, 16> m_WhitePieceTypes;
};