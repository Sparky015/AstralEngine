/**
* @file FENParser.h
* @author Andrew Fagan
* @date 12/27/2024
*/

#pragma once

#include "pch.h"
#include "Chessboard/Chessboard.h"

namespace Game {

    class FENParser
    {
    public:
        FENParser();

        void ParseFENString(const std::string& FEN_String);

        [[nodiscard]] const std::array<SquareLocation, Chessboard::NUMBER_OF_PIECES_PER_COLOR>& GetWhitePieceLocations() const { return m_WhitePieceLocations; }
        [[nodiscard]] const std::array<SquareLocation, Chessboard::NUMBER_OF_PIECES_PER_COLOR>& GetBlackPieceLocations() const { return m_BlackPieceLocations; }
        [[nodiscard]] const InternalBoardRepresentation& GetBoard() const { return m_Board; }
        [[nodiscard]] const PreviousMoveData& GetPreviousMoveData() const { return m_PreviousMoveData; }
        [[nodiscard]] uint16 GetFullMoveCount() const { return m_FullMoveCount; }
        [[nodiscard]] uint8 GetHalfMoveCount() const { return m_HalfMoveCount; }
        [[nodiscard]] PieceColor GetActiveColor() const { return m_ActiveColor; }
        [[nodiscard]] const BoardCastleRights& GetCastleRights() const { return m_CastleRights; }

    private:
        void PopulateMoveCounts();
        void PopulateActiveColor();
        void PopulateCastleRights();
        void PopulatePieceLocations();
        void PopulateEnpassantState();

        std::array<SquareLocation, Chessboard::NUMBER_OF_PIECES_PER_COLOR> m_WhitePieceLocations;
        std::array<SquareLocation, Chessboard::NUMBER_OF_PIECES_PER_COLOR> m_BlackPieceLocations;
        InternalBoardRepresentation m_Board;
        PreviousMoveData m_PreviousMoveData;
        uint16 m_FullMoveCount;
        uint8 m_HalfMoveCount;
        PieceColor m_ActiveColor;
        BoardCastleRights m_CastleRights;

        std::string pieceLocations;
        char activeColor;
        std::string castleRights;
        std::string enPassantPawn;
        std::string halfMoveCount;
        std::string fullMoveCount;
    };

}