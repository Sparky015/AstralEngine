/**
* @file ChessBoard.h
* @author Andrew Fagan
* @date 10/20/2024
*/

#pragma once

#include "pch.h"

#include "InternalChessboardData.h"
#include "Common/ChessPieceTypes.h"

namespace Game {

   /**
    * @class Chessboard
    * @brief This holds the data representation of a chess board.
    */
    class Chessboard
    {
    public:

        Chessboard();
        explicit Chessboard(const std::string& FEN);
        ~Chessboard() = default;

        /**@brief Moves a chess piece to a given location
         * @param pieceID ID of the moving chess piece
         * @param pieceColor Color of the moving chess piece
         * @param targetLocation Location to move the chess piece to */
        void MovePiece(const PieceID pieceID, const PieceColor pieceColor, const SquareLocation targetLocation);

        /**@brief Captures a chess piece at the given location
         * @param pieceID ID of capturing chess piece
         * @param pieceColor Color of capturing chess piece
         * @param targetPieceLocation Location of chess piece being captured */
        void CapturePiece(const PieceID pieceID, const PieceColor pieceColor, const SquareLocation targetPieceLocation);

        /**@brief Promotes a pawn to the given piece type
         * @param pieceID ID of the pawn
         * @param pieceColor Color of the pawn
         * @param promotionType Piece type to promote the pawn to */
        void PromotePawn(const PieceID pieceID, const PieceColor pieceColor, const PieceType promotionType);

        /**@brief Adds a piece to the board
         * @param pieceID Piece ID of the piece being added
         * @param pieceType Piece type of the piece being added
         * @param pieceColor Color of piece being added
         * @param location Location where the piece will be added
         * @note There is a limit of 16 pieces per color */
        void AddPiece(PieceID pieceID, PieceType pieceType, PieceColor pieceColor, SquareLocation location);

        /**@brief Removes a piece from the board
         * @param pieceID Piece ID of the piece being added
         * @param pieceColor Color of piece being added */
        void RemovePiece(PieceID pieceID, PieceColor pieceColor);

        /**@brief Gets the location of a chess piece
         * @param pieceID ID of chess piece
         * @param pieceColor Color of chess piece
         * @return Location of the chess piece */
        [[nodiscard]] SquareLocation GetPieceLocation(const PieceID pieceID, const PieceColor pieceColor) const;

        /**@brief Gets the piece type of a chess piece
         * @param pieceID ID of chess piece
         * @param pieceColor Color of chess piece
         * @return Piece type of chess piece
         * @note This relies on the piece's location to find its type */
        [[nodiscard]] PieceType GetPieceType(const PieceID pieceID, const PieceColor pieceColor) const;

        /**@brief Sets the PieceType of a piece
         * @param pieceID The piece ID of the piece
         * @param pieceType The piece type to set the given piece to
         * @param pieceColor The piece color of the piece */
        void SetPieceType(PieceID pieceID, PieceType pieceType, PieceColor pieceColor);

        /**@brief Gets the color of a piece on a square
         * @param squareLocation Location to check color of piece at
         * @return Color of a piece at given square
         * @note This assumes that there is a piece on the given square */
        [[nodiscard]] PieceColor GetSquareColor(const SquareLocation squareLocation) const;

        /**@brief Gets the type of a piece on a square
         * @param squareLocation Location to check type of piece at
         * @return Type of piece at given square
         * @note This assumes that there is a piece on the given square */
        [[nodiscard]] PieceType GetSquareType(const SquareLocation squareLocation) const;

        /**@brief Gets the ID of a piece on a square
         * @param squareLocation Location to check type of piece at.
         * @return ID of piece at given square
         * @note There must be a piece on the square given. */
        [[nodiscard]] PieceID GetSquarePieceID(const SquareLocation squareLocation) const;

        /**@brief Gets the state of the castle rights of the chess board.
         * @return The castle rights of the board */
        [[nodiscard]] inline KingCastleRights GetCastleRights(const PieceColor color) const { return m_CastleRights.ReadCastleRights(color); }

        /**@brief Gets color of the side whose turn it is
         * @return The active color of the board */
        [[nodiscard]] inline PieceColor GetActiveColor() const { return m_ActiveColor; }

        /**@brief Gets color of the side whose turn it is
         * @return The active color of the board */
        inline void SetActiveColor(const PieceColor activeColor) { m_ActiveColor = activeColor; }

        /**@brief Gets the half move count of the board
         * @return The half move count */
        [[nodiscard]] inline uint8 GetHalfMoveCount() const { return m_HalfMoveCount;}

        /**@brief Gets the full move count of the board
         * @return The full move count */
        [[nodiscard]] inline uint8 GetFullMoveCount() const { return m_FullMoveCount;}

        /**@brief Gets the previous move data
         * @return The previous move data object */
        [[nodiscard]] inline PreviousMoveData GetPreviousMoveData() const {return m_PreviousMoveData;}

        /**@brief Gets the piece ID of the king
         * @param kingColor The color of the king
         * @return The piece ID of the king */
        [[nodiscard]] PieceID GetKingPieceID(const PieceColor kingColor) const;

        /**@brief Gets the character representation of the given square
         * @param squareLocation The location of the square
         * @return The character that represents the piece
         * @note Upper case letters represent white*/
        [[nodiscard]] char GetCharacterOfPiece(const SquareLocation squareLocation) const;

        /**@brief Clears the board to an empty board */
        void ClearBoard();

        /**@brief Prints the state of the board to the console */
        void PrintBoardToConsole() const;

        static constexpr uint8 NUMBER_OF_SQUARES = 64;
        static constexpr uint8 NUMBER_OF_PIECES_PER_COLOR = 16;

    private:

        uint16 m_FullMoveCount;
        uint8 m_HalfMoveCount;
        PieceColor m_ActiveColor;
        PreviousMoveData m_PreviousMoveData;
        BoardCastleRights m_CastleRights;
        std::array<SquareLocation, NUMBER_OF_PIECES_PER_COLOR> m_BlackPieceLocations; // These two contain the location info of a piece
        std::array<SquareLocation, NUMBER_OF_PIECES_PER_COLOR> m_WhitePieceLocations;
        InternalBoardRepresentation m_Board; // This contains the piece type and color of a piece

        public:
        inline InternalBoardRepresentation& GetInternalBoard()
        { return m_Board; } /// For testing only!
    };

}
