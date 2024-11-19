//
// Created by Andrew Fagan on 11/18/24.
//

#include "MoveList.h"

namespace Game {

    Pawn& MoveList::GetPawn(PieceID pieceID)
    {
        switch(pieceID)
        {
            case PAWN_1: return m_Pawn1;
            case PAWN_2: return m_Pawn2;
            case PAWN_3: return m_Pawn3;
            case PAWN_4: return m_Pawn4;
            case PAWN_5: return m_Pawn5;
            case PAWN_6: return m_Pawn6;
            case PAWN_7: return m_Pawn7;
            case PAWN_8: return m_Pawn8;

            case ROOK_1: case KNIGHT_1: case BISHOP_1: case QUEEN:
            case KING: case BISHOP_2: case KNIGHT_2: case ROOK_2:
            ERROR("Given piece ID is not a pawn!");
        }
    }


    Rook& MoveList::GetRook(PieceID pieceID)
    {
        switch(pieceID)
        {
            case ROOK_1: return m_Rook1;
            case ROOK_2: return m_Rook2;

            case PAWN_1: case PAWN_2: case PAWN_3: case PAWN_4:
            case PAWN_5: case PAWN_6: case PAWN_7: case PAWN_8:
            case KNIGHT_1: case KNIGHT_2: case BISHOP_1: case BISHOP_2:
            case QUEEN: case KING:
            ERROR("Given piece ID is not a rook!");
        }
    }


    Knight& MoveList::GetKnight(PieceID pieceID)
    {
        switch(pieceID)
        {
            case KNIGHT_1: return m_Knight1;
            case KNIGHT_2: return m_Knight2;

            case PAWN_1: case PAWN_2: case PAWN_3: case PAWN_4:
            case PAWN_5: case PAWN_6: case PAWN_7: case PAWN_8:
            case ROOK_1: case ROOK_2: case BISHOP_1: case BISHOP_2:
            case QUEEN: case KING:
            ERROR("Given piece ID is not a knight!");
        }
    }


    Bishop& MoveList::GetBishop(PieceID pieceID)
    {
        switch(pieceID)
        {
            case BISHOP_1: return m_Bishop1;
            case BISHOP_2: return m_Bishop2;

            case PAWN_1: case PAWN_2: case PAWN_3: case PAWN_4:
            case PAWN_5: case PAWN_6: case PAWN_7: case PAWN_8:
            case ROOK_1: case ROOK_2: case KNIGHT_1: case KNIGHT_2:
            case QUEEN: case KING:
            ERROR("Given piece ID is not a bishop!");
        }
    }


    King& MoveList::GetKing(PieceID pieceID)
    {
        switch(pieceID)
        {
            case KING: return m_King;

            case PAWN_1: case PAWN_2: case PAWN_3: case PAWN_4:
            case PAWN_5: case PAWN_6: case PAWN_7: case PAWN_8:
            case ROOK_1: case ROOK_2: case KNIGHT_1: case KNIGHT_2:
            case BISHOP_1: case BISHOP_2: case QUEEN:
            ERROR("Given piece ID is not a king!");
        }
    }


    Queen& MoveList::GetQueen(PieceID pieceID)
    {
        switch(pieceID)
        {
            case QUEEN: return m_Queen;

            case PAWN_1: case PAWN_2: case PAWN_3: case PAWN_4:
            case PAWN_5: case PAWN_6: case PAWN_7: case PAWN_8:
            case ROOK_1: case ROOK_2: case KNIGHT_1: case KNIGHT_2:
            case BISHOP_1: case BISHOP_2: case KING:
            ERROR("Given piece ID is not a queen!");
        }
    }

    MoveList::MoveList()
    {

    }


} // Game