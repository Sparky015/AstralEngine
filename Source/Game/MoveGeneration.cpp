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


        // These directions are just for the knight
        UP_LEFT = -17,
        UP_RIGHT = -15,
        DOWN_LEFT = 17,
        DOWN_RIGHT = 15,
        LEFT_UP = -6,
        LEFT_DOWN = -10,
        RIGHT_UP = 6,
        RIGHT_DOWN = 10
    };


    MoveListGenerator::MoveListGenerator() : m_Board(nullptr), m_MoveList{nullptr}{}


    void MoveListGenerator::GenerateMoveList(MoveList* outMoveList, const Board* board, PieceColor pieceColor)
    {
        m_Board = board;
        m_MoveList = outMoveList;
        *m_MoveList = MoveList();

        for (uint8 pieceID = PAWN_1; pieceID != ROOK_2; pieceID++)
        {
            uint8 pieceLocation = m_Board->ReadPieceLocation(pieceColor, (PieceID)pieceID);
            if (pieceLocation == EMPTY) { continue; }
            // TODO: Fix bounds check on the first step
            switch (m_Board->ReadPieceType(pieceColor, (PieceID)pieceID))
            {
                case PieceType::PAWN:
                    CalculatePawnMoveList((PieceID)pieceID, pieceColor, pieceLocation);
                    break;
                case PieceType::KNIGHT:
                    CalculateKnightMoveList((PieceID)pieceID, pieceColor, pieceLocation);
                    break;
                case PieceType::ROOK:
                    CalculateRookMoveList((PieceID)pieceID, pieceColor, pieceLocation);
                    break;
                case PieceType::BISHOP:
                    CalculateBishopMoveList((PieceID)pieceID, pieceColor, pieceLocation);
                    break;
                case PieceType::KING:
                    CalculateKingMoveList((PieceID)pieceID, pieceColor, pieceLocation);
                    break;
                case PieceType::QUEEN:
                    CalculateQueenMoveList((PieceID)pieceID, pieceColor, pieceLocation);
                    break;
                case PieceType::NONE:
                    ERROR("Piece type should not be NONE here.")
                    break;
            }

        }

        m_Board = nullptr;
        m_MoveList = nullptr;
    }

    void MoveListGenerator::CalculatePawnMoveList(PieceID pieceID, PieceColor pieceColor, uint8 pieceLocation)
    {
        // Adding the square ahead of the pawn if the square in front of the pawn is empty
        uint8 directionMultiplier = (pieceColor == PieceColor::BLACK) ? -1 : 1;
        uint8 nextPieceLocation = pieceLocation + (UP * directionMultiplier);
        if (m_Board->ReadSquareType(nextPieceLocation) == PieceType::NONE)
        { // TODO: Add bounds checks
//            m_MoveList->GetPieceVec(pieceID).push_back(nextPieceLocation);
        }

        // Check if en passant is applicable


        // TODO: Implement en passant
        // TODO: Implement the two space moves for pawns on their first move
    }

    void MoveListGenerator::CalculateRookMoveList(PieceID pieceID, PieceColor pieceColor, uint8 pieceLocation)
    {
        // Walking in each direction while checking if each square is empty until there is a square that is not empty.
        // If square is empty, add move to move list.
        int directionMultipler = (pieceColor == PieceColor::BLACK) ? -1 : 1;
        std::array<int, 4> moveDirections = {UP, DOWN, LEFT, RIGHT};
        int nextPieceLocation;

        for (int moveDirection : moveDirections)
        {
            int moveStep = moveDirection * directionMultipler;
            nextPieceLocation = pieceLocation + moveStep;
            while (m_Board->ReadSquareType(pieceLocation) == PieceType::NONE)
            {
//                m_MoveList->GetPieceVec(pieceID).push_back(nextPieceLocation);

                // Checking if the next move will be in bounds before updating again
                if (moveStep == UP && nextPieceLocation < 8) {break;}
                else if (moveStep == LEFT && nextPieceLocation % 8 == 0) {break;}
                else if (moveStep == RIGHT && nextPieceLocation % 8 == 7) {break;}
                else if (moveStep == DOWN && nextPieceLocation > 55) {break;}

                nextPieceLocation += moveStep;
            }
        }

    }

    void MoveListGenerator::CalculateBishopMoveList(PieceID pieceID, PieceColor pieceColor, uint8 pieceLocation)
    {
        // Walking in each direction while checking if each square is empty until there is a square that is not empty.
        // If square is empty, add move to move list.
        int directionMultipler = (pieceColor == PieceColor::BLACK) ? -1 : 1;
        std::array<int, 4> moveDirections = {DIAGONAL_UP_LEFT, DIAGONAL_UP_RIGHT, DIAGONAL_DOWN_LEFT, DIAGONAL_DOWN_RIGHT};
        int nextPieceLocation;

        for (int moveDirection : moveDirections)
        {
            int moveStep = moveDirection * directionMultipler;
            nextPieceLocation = pieceLocation + moveStep;
            while (m_Board->ReadSquareType(pieceLocation) == PieceType::NONE)
            {
//                m_MoveList->GetPieceVec(pieceID).push_back(nextPieceLocation);

                // Checking if the next move will be in bounds before updating again
                if (moveStep == DIAGONAL_UP_LEFT && (nextPieceLocation < 8 || nextPieceLocation % 8 == 0)) {break;}
                else if (moveStep == DIAGONAL_UP_RIGHT && (nextPieceLocation < 8 || nextPieceLocation % 8 == 7)) {break;}
                else if (moveStep == DIAGONAL_DOWN_LEFT && (nextPieceLocation > 55 || nextPieceLocation % 8 == 0)) {break;}
                else if (moveStep == DIAGONAL_DOWN_RIGHT && (nextPieceLocation > 55 || nextPieceLocation % 8 == 7)) {break;}

                nextPieceLocation += moveStep;
            }
        }
    }

    void MoveListGenerator::CalculateKnightMoveList(PieceID pieceID, PieceColor pieceColor, uint8 pieceLocation)
    {
        // Checking each possible square that the knight could do. If square is empty, add move to move list.
        int directionMultipler = (pieceColor == PieceColor::BLACK) ? -1 : 1;

        // These directions are specific for the knight. Each number is size of the move step that the knight makes
        // to get to each location a knight can reach.
        std::array<int, 8> moveDirections = {UP_RIGHT, UP_LEFT, DOWN_RIGHT, DOWN_LEFT, LEFT_UP, LEFT_DOWN, RIGHT_UP, RIGHT_DOWN};

        int nextPieceLocation;
        for (int moveDirection : moveDirections)
        {
            int moveStep = moveDirection * directionMultipler;
            nextPieceLocation = pieceLocation + moveStep;

            if (m_Board->ReadSquareType(pieceLocation) == PieceType::NONE) // Only checks each direction once
            {
//                m_MoveList->GetPieceVec(pieceID).push_back(nextPieceLocation);

                // Checking if the next move will be in bounds before updating again
                if (moveStep == UP_LEFT && (nextPieceLocation < 16 || nextPieceLocation % 8 == 0)) {break;}
                else if (moveStep == UP_RIGHT && (nextPieceLocation < 16) || nextPieceLocation % 8 == 7) {break;}
                else if (moveStep == DOWN_LEFT && (nextPieceLocation > 47 || nextPieceLocation % 8 == 0)) {break;}
                else if (moveStep == DOWN_RIGHT && (nextPieceLocation > 47 || nextPieceLocation % 8 == 7)) {break;}
                else if (moveStep == LEFT_UP && (nextPieceLocation < 8 || nextPieceLocation % 8 == 1)) {break;}
                else if (moveStep == LEFT_DOWN && (nextPieceLocation > 55 || nextPieceLocation % 8 == 1)) {break;}
                else if (moveStep == RIGHT_UP && (nextPieceLocation < 8 || nextPieceLocation % 8 == 6)) {break;}
                else if (moveStep == RIGHT_DOWN && (nextPieceLocation > 55 || nextPieceLocation % 8 == 6)) {break;}

            }
        }
    }

    void MoveListGenerator::CalculateQueenMoveList(PieceID pieceID, PieceColor pieceColor, uint8 pieceLocation)
    {
        // Call bishop and rook calculation functions. The rook's and bishop's possible moves combined equal the queen's
        // possible moves.
        int directionMultipler = (pieceColor == PieceColor::BLACK) ? -1 : 1;
        std::array<int, 8> moveDirections = {UP, DOWN, LEFT, RIGHT, DIAGONAL_UP_LEFT, DIAGONAL_UP_RIGHT, DIAGONAL_DOWN_LEFT, DIAGONAL_DOWN_RIGHT};
        int nextPieceLocation;

        for (int moveDirection : moveDirections)
        {
            int moveStep = moveDirection * directionMultipler;
            nextPieceLocation = pieceLocation + moveStep;
            while (m_Board->ReadSquareType(pieceLocation) == PieceType::NONE)
            {
//                m_MoveList->GetPieceVec(pieceID).push_back(nextPieceLocation);

                // Checking if the next move will be in bounds before updating again
                if (moveStep == UP && nextPieceLocation < 8) {break;}
                else if (moveStep == LEFT && nextPieceLocation % 8 == 0) {break;}
                else if (moveStep == RIGHT && nextPieceLocation % 8 == 7) {break;}
                else if (moveStep == DOWN && nextPieceLocation > 55) {break;}
                else if (moveStep == DIAGONAL_UP_LEFT && (nextPieceLocation < 8 || nextPieceLocation % 8 == 0)) {break;}
                else if (moveStep == DIAGONAL_UP_RIGHT && (nextPieceLocation < 8 || nextPieceLocation % 8 == 7)) {break;}
                else if (moveStep == DIAGONAL_DOWN_LEFT && (nextPieceLocation > 55 || nextPieceLocation % 8 == 0)) {break;}
                else if (moveStep == DIAGONAL_DOWN_RIGHT && (nextPieceLocation > 55 || nextPieceLocation % 8 == 7)) {break;}

                nextPieceLocation += moveStep;
            }
        }

    }

    void MoveListGenerator::CalculateKingMoveList(PieceID pieceID, PieceColor pieceColor, uint8 pieceLocation)
    {
        // Checking each possible square that the king could do. If square is empty, add move to move list.
        int directionMultipler = (pieceColor == PieceColor::BLACK) ? -1 : 1;
        std::array<int, 8> moveDirections = {UP, DOWN, LEFT, RIGHT, DIAGONAL_UP_LEFT, DIAGONAL_UP_RIGHT, DIAGONAL_DOWN_LEFT, DIAGONAL_DOWN_RIGHT};
        int nextPieceLocation;

        for (int moveDirection : moveDirections)
        {
            int moveStep = moveDirection * directionMultipler;
            nextPieceLocation = pieceLocation + moveStep;

            if (m_Board->ReadSquareType(pieceLocation) == PieceType::NONE) // Only checks each direction once
            {
//                m_MoveList->GetPieceVec(pieceID).push_back(nextPieceLocation);

                // Checking if the next move will be in bounds before updating again
                if (moveStep == UP && nextPieceLocation < 8) {break;}
                else if (moveStep == LEFT && nextPieceLocation % 8 == 0) {break;}
                else if (moveStep == RIGHT && nextPieceLocation % 8 == 7) {break;}
                else if (moveStep == DOWN && nextPieceLocation > 55) {break;}
            }
        }

        // Check if castling is possible
        // TODO: Implement castling
    }



}