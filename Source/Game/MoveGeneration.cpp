//
// Created by Andrew Fagan on 11/2/24.
//

#include "MoveGeneration.h"

namespace Game{

    enum BoardTransversal
    {
        UP = -8,
        DOWN = 8,
        LEFT = -1,
        RIGHT = 1,
        DIAGONAL_UP_RIGHT = -9,
        DIAGONAL_UP_LEFT = -7,
        DIAGONAL_DOWN_RIGHT = 9,
        DIAGONAL_DOWN_LEFT = 7,
    };


    void GenerateMoveList(MoveList& outMoveList, Board& board, PieceColor pieceColor)
    {
        for (uint8 pieceID = PAWN_1; pieceID != ROOK_2; pieceID++)
        {
            uint8 pieceLocation = board.ReadPieceLocation(pieceColor, (PieceID)pieceID);
            if (pieceLocation == EMPTY) { continue; }

            switch (ConvertPieceIDToPieceType((PieceID)pieceID))
            {
                case PieceType::PAWN:
                    CalculatePawnMoveList(outMoveList, (PieceID)pieceID, pieceColor, pieceLocation);
                    break;
                case PieceType::KNIGHT:
                    CalculateKnightMoveList(outMoveList,(PieceID)pieceID, pieceColor, pieceLocation);
                    break;
                case PieceType::ROOK:
                    break;
                case PieceType::BISHOP:
                    break;
                case PieceType::KING:
                    break;
                case PieceType::QUEEN:
                    break;
                case PieceType::NONE:
                    ERROR("Piece type should not be NONE here.")
                    break;
            }

        }


    }

    void CalculatePawnMoveList(MoveList& outMoveList, PieceID pieceID, PieceColor pieceColor, uint8 pieceLocation)
    {
        // Adding the square ahead of the pawn if the square in front of the pawn is empty


        // Check if en passant is applicable


    }

    void CalculateRookMoveList(MoveList& outMoveList, PieceID pieceID, PieceColor pieceColor, uint8 pieceLocation)
    {
        // Walking in each direction while checking if each square is empty until there is a square that is not empty.
        // If square is empty, add move to move list.


    }

    void CalculateBishopMoveList(MoveList& outMoveList, PieceID pieceID, PieceColor pieceColor, uint8 pieceLocation)
    {
        // Walking in each direction while checking if each square is empty until there is a square that is not empty.
        // If square is empty, add move to move list.

    }

    void CalculateKnightMoveList(MoveList& outMoveList, PieceID pieceID, PieceColor pieceColor, uint8 pieceLocation)
    {
        // Checking each possible square that the knight could do. If square is empty, add move to move list.

    }

    void CalculateQueenMoveList(MoveList& outMoveList, PieceID pieceID, PieceColor pieceColor, uint8 pieceLocation)
    {
        // Call bishop and rook calculation functions. The rook's and bishop's possible moves combined equal the queen's
        // possible moves.

    }

    void CalculateKingMoveList(MoveList& outMoveList, PieceID pieceID, PieceColor pieceColor, uint8 pieceLocation)
    {
        // Checking each possible square that the king could do. If square is empty, add move to move list.


        // Check if castling is possible

    }

}