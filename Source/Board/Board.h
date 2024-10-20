/**
* @file Board.h
* @author Andrew Fagan
* @date 10/20/2024
*/

#pragma once

#include "pch.h"


/**
 * @enum This tracks the index of each piece in the Board's piece arrays.
 * @note The names of each ID is what the piece is at the start of the game.
*/
enum class PieceID : uint8
{
    PAWN_1,
    PAWN_2,
    PAWN_3,
    PAWN_4,
    PAWN_5,
    PAWN_6,
    PAWN_7,
    PAWN_8,
    BISHOP_1,
    BISHOP_2,
    KNIGHT_1,
    KNIGHT_2,
    ROOK_1,
    ROOK_2,
    QUEEN,
    KING
};

/**
 * @enum This identifies the type of a piece.
*/
enum class PieceType : uint8
{
    PAWN,
    BISHOP,
    KNIGHT,
    ROOK,
    QUEEN,
    KING
};

/**
* @enum This identifies the color of a piece
*/
enum class PieceColor : bool
{
    BLACK,
    WHITE
};


/**
 * @class This holds the data representation of a chess board.
 */
class Board
{
public:

    Board();
    ~Board();


    /**
     * @brief Moves a specified piece from its current location to the specified location
     * @param color The color of the piece being moved.
     * @param pieceIndex The index of the piece being moved.
     * @param targetBoardLocation The location that the piece is being moved to. Expected range is 0-63 inclusive.
     * @throws std::logic_error Errors when the square being moved to is already occupied.
     * @throws std::invalid_argument Errors when the specified location is not 0-63 inclusive.
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
     * @throws std::invalid_argument Errors when the specified location is not 0-63 inclusive.
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
      * @throws std::invalid_argument Errors when the specified location is not 0-63 inclusive.
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
     * @throws std::invalid_argument Errors when the specified location is not 0-63 inclusive.
     */
    void WriteBoardLocation(const PieceColor color, const PieceID pieceID, const uint8 boardLocation);

private:

    // TODO: Improve the information density of the piece types
    std::array<uint8, 16> m_BlackPieceLocations;
    std::array<PieceType, 16> m_BlackPieceTypes;
    std::array<uint8, 16> m_WhitePieceLocations;
    std::array<PieceType, 16> m_WhitePieceTypes;
};