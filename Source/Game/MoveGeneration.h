//
// Created by Andrew Fagan on 11/2/24.
//

#pragma once

#include "Game/Board.h"


struct MoveList
{
    std::vector<uint8> Pawn1;
    std::vector<uint8> Pawn2;
    std::vector<uint8> Pawn3;
    std::vector<uint8> Pawn4;
    std::vector<uint8> Pawn5;
    std::vector<uint8> Pawn6;
    std::vector<uint8> Pawn7;
    std::vector<uint8> Pawn8;
    std::vector<uint8> Rook1;
    std::vector<uint8> Rook2;
    std::vector<uint8> Knight1;
    std::vector<uint8> Knight2;
    std::vector<uint8> Bishop1;
    std::vector<uint8> Bishop2;
    std::vector<uint8> King;
    std::vector<uint8> Queen;

    MoveList() = default;

    std::vector<uint8>& GetPieceVec(PieceID pieceID)
    {
        switch(pieceID)
        {
            case PAWN_1: return Pawn1;
            case PAWN_2: return Pawn2;
            case PAWN_3: return Pawn3;
            case PAWN_4: return Pawn4;
            case PAWN_5: return Pawn5;
            case PAWN_6: return Pawn6;
            case PAWN_7: return Pawn7;
            case PAWN_8: return Pawn8;
            case ROOK_1: return Rook1;
            case ROOK_2: return Rook2;
            case KNIGHT_1: return Knight1;
            case KNIGHT_2: return Knight2;
            case BISHOP_1: return Bishop1;
            case BISHOP_2: return Bishop2;
            case KING: return King;
            case QUEEN: return Queen;
        }
    }
};

namespace Game{

    class MoveListGenerator
    {
    public:
        MoveListGenerator();
        ~MoveListGenerator() = default;

        void GenerateMoveList(MoveList* outMoveList, const Board* board, PieceColor pieceColor);

    private:
        void CalculatePawnMoveList(PieceID pieceID, PieceColor pieceColor, uint8 pieceLocation);
        void CalculateRookMoveList(PieceID pieceID, PieceColor pieceColor, uint8 pieceLocation);
        void CalculateBishopMoveList(PieceID pieceID, PieceColor pieceColor, uint8 pieceLocation);
        void CalculateKnightMoveList(PieceID pieceID, PieceColor pieceColor, uint8 pieceLocation);
        void CalculateQueenMoveList(PieceID pieceID, PieceColor pieceColor, uint8 pieceLocation);
        void CalculateKingMoveList(PieceID pieceID, PieceColor pieceColor, uint8 pieceLocation);

        MoveList* m_MoveList;
        const Board* m_Board;
    };




}