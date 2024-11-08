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


    MoveListGenerator::MoveListGenerator(Board& board) : m_Board(board), m_MoveList{nullptr}
    {

    }


    void MoveListGenerator::GenerateMoveList(MoveList* outMoveList, PieceColor pieceColor)
    {
        m_MoveList = outMoveList;
        *m_MoveList = MoveList();

        for (uint8 pieceID = PAWN_1; pieceID != ROOK_2; pieceID++)
        {
            uint8 pieceLocation = m_Board.ReadPieceLocation(pieceColor, (PieceID)pieceID);
            if (pieceLocation == EMPTY) { continue; }

            switch (ConvertPieceIDToPieceType((PieceID)pieceID))
            {
                case PieceType::PAWN:
                    CalculatePawnMoveList((PieceID)pieceID, pieceColor, pieceLocation);
                    break;
                case PieceType::KNIGHT:
                    CalculateKnightMoveList((PieceID)pieceID, pieceColor, pieceLocation);
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

        m_MoveList = nullptr;
    }

    void MoveListGenerator::CalculatePawnMoveList(PieceID pieceID, PieceColor pieceColor, uint8 pieceLocation)
    {
        // Adding the square ahead of the pawn if the square in front of the pawn is empty
        uint8 directionMultipler = (pieceColor == PieceColor::BLACK) ? -1 : 1;
        uint8 nextPieceLocation = pieceLocation + (UP * directionMultipler);
        if (m_Board.ReadSquareType(nextPieceLocation) == PieceType::NONE)
        {
            m_MoveList->GetPieceVec(pieceID).push_back(nextPieceLocation);
        }

        // Check if en passant is applicable

        //TODO: Implement this ^
    }

    void MoveListGenerator::CalculateRookMoveList(PieceID pieceID, PieceColor pieceColor, uint8 pieceLocation)
    {
        // Walking in each direction while checking if each square is empty until there is a square that is not empty.
        // If square is empty, add move to move list.
        uint8 directionMultipler = (pieceColor == PieceColor::BLACK) ? -1 : 1;

        // Checking UP direction
        uint8 nextPieceLocation = pieceLocation + (UP * directionMultipler);
        while (m_Board.ReadSquareType(pieceLocation) == PieceType::NONE)
        {
            m_MoveList->GetPieceVec(pieceID).push_back(nextPieceLocation);
            nextPieceLocation += (UP * directionMultipler);
        }




    }

    void MoveListGenerator::CalculateBishopMoveList(PieceID pieceID, PieceColor pieceColor, uint8 pieceLocation)
    {
        // Walking in each direction while checking if each square is empty until there is a square that is not empty.
        // If square is empty, add move to move list.

    }

    void MoveListGenerator::CalculateKnightMoveList(PieceID pieceID, PieceColor pieceColor, uint8 pieceLocation)
    {
        // Checking each possible square that the knight could do. If square is empty, add move to move list.

    }

    void MoveListGenerator::CalculateQueenMoveList(PieceID pieceID, PieceColor pieceColor, uint8 pieceLocation)
    {
        // Call bishop and rook calculation functions. The rook's and bishop's possible moves combined equal the queen's
        // possible moves.

    }

    void MoveListGenerator::CalculateKingMoveList(PieceID pieceID, PieceColor pieceColor, uint8 pieceLocation)
    {
        // Checking each possible square that the king could do. If square is empty, add move to move list.


        // Check if castling is possible

    }



}