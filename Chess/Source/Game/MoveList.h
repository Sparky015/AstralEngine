//
// Created by Andrew Fagan on 11/18/24.
//

#pragma once

#include "Board/Board.h"
#include "Pieces/Pawn.h"
#include "Pieces/Rook.h"
#include "Pieces/Knight.h"
#include "Pieces/Bishop.h"
#include "Pieces/Queen.h"
#include "Pieces/King.h"

namespace Game {

    class MoveList
    {
    public:
        explicit MoveList(Board& board, PieceColor color);
        ~MoveList() = default;

        void GenerateMoves(Board& board, PieceColor color);

        const std::vector<uint8>& GetRegularMoves(PieceID pieceID);
        const std::vector<uint8>& GetAttackingMoves(PieceID pieceID);

    private:
        void SyncPieceTypesToBoard(Board& board, PieceColor color);
        std::unique_ptr<ChessPiece>& GetPiecePtr(PieceID pieceID);

        std::unique_ptr<ChessPiece> m_Piece1;
        std::unique_ptr<ChessPiece> m_Piece2;
        std::unique_ptr<ChessPiece> m_Piece3;
        std::unique_ptr<ChessPiece> m_Piece4;
        std::unique_ptr<ChessPiece> m_Piece5;
        std::unique_ptr<ChessPiece> m_Piece6;
        std::unique_ptr<ChessPiece> m_Piece7;
        std::unique_ptr<ChessPiece> m_Piece8;
        std::unique_ptr<ChessPiece> m_Piece9;
        std::unique_ptr<ChessPiece> m_Piece10;
        std::unique_ptr<ChessPiece> m_Piece11;
        std::unique_ptr<ChessPiece> m_Piece12;
        std::unique_ptr<ChessPiece> m_Piece13;
        std::unique_ptr<ChessPiece> m_Piece14;
        std::unique_ptr<ChessPiece> m_Piece15;
        std::unique_ptr<ChessPiece> m_Piece16;
    };

} // Game
