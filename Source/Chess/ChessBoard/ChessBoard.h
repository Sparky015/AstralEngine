/**
* @file Board.h
* @author Andrew Fagan
* @date 10/20/2024
*/

#pragma once

#include "pch.h"

#include "InternalChessBoardData.h"
#include "../GameState/ChessPieceTypes.h"

namespace Game {


/**
 * @class This holds the data representation of a chess board.
 */
    class ChessBoard
    {
    public:

        ChessBoard();
        explicit ChessBoard(const std::string& FEN);
        ~ChessBoard() = default;

        void MovePiece(const PieceColor color, const PieceID pieceID, SquareLocation targetBoardLocation);
        void TakePiece(const PieceColor color, const PieceID pieceID, SquareLocation targetBoardLocation);
        void PromotePawn(const PieceColor color, const PieceID pieceID, const PieceType promotionType);

        [[nodiscard]] SquareLocation ReadPieceLocation(const PieceColor color, const PieceID pieceID) const;
        [[nodiscard]] PieceType ReadPieceType(const PieceColor color, const PieceID pieceID) const;
        [[nodiscard]] PieceColor ReadSquareColor(SquareLocation squareLocation) const;
        [[nodiscard]] PieceType ReadSquareType(SquareLocation squareLocation) const;
        [[nodiscard]] PieceID ReadSquarePieceID(SquareLocation squareLocation) const;

        [[nodiscard]] KingCastleRights GetCastleRights(PieceColor color) const;
        [[nodiscard]] PieceColor GetActiveColor() const;
        [[nodiscard]] inline uint8 GetHalfMoveCount() const { return m_HalfMoveCount;}
        [[nodiscard]] inline uint8 GetFullMoveCount() const { return m_HalfMoveCount;}
        [[nodiscard]] inline PreviousMoveBuffer GetLastMoveBuffer() const {return m_LastMoveBuffer;}

        PieceID GetKingPieceID(PieceColor kingColor) const;

        char GetCharacterOfPiece(SquareLocation squareLocation);
        void ClearBoard();
        void PrintBoardToConsole();

    private:


        uint16 m_FullMoveCount;
        uint8 m_HalfMoveCount;
        PieceColor m_ActiveColor;
        PreviousMoveBuffer m_LastMoveBuffer;
        BoardCastleRights m_CastleRights;

        std::array<SquareLocation, 16> m_BlackPieceLocations; // These two contain the location info of a piece
        std::array<SquareLocation, 16> m_WhitePieceLocations;

        InternalBoardRepresentation m_Board; // This contains the piece type and color of a piece

    public:
        inline InternalBoardRepresentation& GetInternalBoard()
        { return m_Board; } /// For testing only!
    };

}
