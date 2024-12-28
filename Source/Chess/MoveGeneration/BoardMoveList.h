//
// Created by Andrew Fagan on 11/18/24.
//

#pragma once

#include "../Chessboard/Chessboard.h"

#include "PieceMoveLists/ChessPieceMoveList.h"

namespace Game {


    class BoardMoveList
    {
    public:
        explicit BoardMoveList(Chessboard& board, PieceColor color);
        ~BoardMoveList() = default;

        void UpdateMoveList(const Chessboard& board, PieceColor color);

        [[nodiscard]] const PieceMoveList& GetRegularMoves(PieceID pieceID, PieceColor pieceColor) const;
        [[nodiscard]] const PieceMoveList& GetAttackingMoves(PieceID pieceID, PieceColor pieceColor) const;

    private:
        void SyncPieceTypesToBoard(const Chessboard& board, PieceColor color);
        std::unique_ptr<ChessPieceMoveList>& GetPiecePtr(PieceID pieceID, PieceColor pieceColor);
        [[nodiscard]] const std::unique_ptr<ChessPieceMoveList>& GetPiecePtr(PieceID pieceID, PieceColor pieceColor) const;

        std::unique_ptr<ChessPieceMoveList> m_WhitePiece1;
        std::unique_ptr<ChessPieceMoveList> m_WhitePiece2;
        std::unique_ptr<ChessPieceMoveList> m_WhitePiece3;
        std::unique_ptr<ChessPieceMoveList> m_WhitePiece4;
        std::unique_ptr<ChessPieceMoveList> m_WhitePiece5;
        std::unique_ptr<ChessPieceMoveList> m_WhitePiece6;
        std::unique_ptr<ChessPieceMoveList> m_WhitePiece7;
        std::unique_ptr<ChessPieceMoveList> m_WhitePiece8;
        std::unique_ptr<ChessPieceMoveList> m_WhitePiece9;
        std::unique_ptr<ChessPieceMoveList> m_WhitePiece10;
        std::unique_ptr<ChessPieceMoveList> m_WhitePiece11;
        std::unique_ptr<ChessPieceMoveList> m_WhitePiece12;
        std::unique_ptr<ChessPieceMoveList> m_WhitePiece13;
        std::unique_ptr<ChessPieceMoveList> m_WhitePiece14;
        std::unique_ptr<ChessPieceMoveList> m_WhitePiece15;
        std::unique_ptr<ChessPieceMoveList> m_WhitePiece16;

        std::unique_ptr<ChessPieceMoveList> m_BlackPiece1;
        std::unique_ptr<ChessPieceMoveList> m_BlackPiece2;
        std::unique_ptr<ChessPieceMoveList> m_BlackPiece3;
        std::unique_ptr<ChessPieceMoveList> m_BlackPiece4;
        std::unique_ptr<ChessPieceMoveList> m_BlackPiece5;
        std::unique_ptr<ChessPieceMoveList> m_BlackPiece6;
        std::unique_ptr<ChessPieceMoveList> m_BlackPiece7;
        std::unique_ptr<ChessPieceMoveList> m_BlackPiece8;
        std::unique_ptr<ChessPieceMoveList> m_BlackPiece9;
        std::unique_ptr<ChessPieceMoveList> m_BlackPiece10;
        std::unique_ptr<ChessPieceMoveList> m_BlackPiece11;
        std::unique_ptr<ChessPieceMoveList> m_BlackPiece12;
        std::unique_ptr<ChessPieceMoveList> m_BlackPiece13;
        std::unique_ptr<ChessPieceMoveList> m_BlackPiece14;
        std::unique_ptr<ChessPieceMoveList> m_BlackPiece15;
        std::unique_ptr<ChessPieceMoveList> m_BlackPiece16;
    };

} // Game
