//
// Created by Andrew Fagan on 11/18/24.
//

#include "MoveList.h"

namespace Game {

    MoveList::MoveList(Board& board, PieceColor color) :
    m_Piece1(nullptr), m_Piece2(nullptr), m_Piece3(nullptr), m_Piece4(nullptr),
    m_Piece5(nullptr), m_Piece6(nullptr), m_Piece7(nullptr), m_Piece8(nullptr),
    m_Piece9(nullptr), m_Piece10(nullptr), m_Piece11(nullptr), m_Piece12(nullptr),
    m_Piece13(nullptr), m_Piece14(nullptr), m_Piece15(nullptr), m_Piece16(nullptr)
    {
        PROFILE_SCOPE();
        SyncPieceTypesToBoard(board, color);
    }


    void MoveList::GenerateMoves(Board& board, PieceColor color)
    {
        PROFILE_SCOPE();
        // First sync the piece types of the board to the move list (in case of pawn promotions)
        SyncPieceTypesToBoard(board, color);

        // Generate the moves of each piece
        for (uint8 pieceID = PIECE_1; pieceID != PIECE_16 + 1; pieceID++)
        {
            std::unique_ptr<ChessPiece>& piecePtr = GetPiecePtr((PieceID)pieceID);

            if (piecePtr)
            {
                uint8 pieceLocation = board.ReadPieceLocation(color, (PieceID)pieceID);
                piecePtr->GenerateMoves(board, pieceLocation, color);
            }
        }
    }


    void MoveList::SyncPieceTypesToBoard(Board& board, PieceColor color)
    {
        PROFILE_SCOPE();
        // Loop through each piece in the board and look at its type
        for (uint8 pieceID = PIECE_1; pieceID != PIECE_16 + 1; pieceID++)
        {
            std::unique_ptr<ChessPiece>& piecePtr = GetPiecePtr((PieceID)pieceID);

            // Check to see if the piece types already match
            if (piecePtr && board.ReadPieceType(color, (PieceID)pieceID) == piecePtr->GetType())
            {
                continue;
            }

            // Make a unique pointer based on its type and assign it to the piece in movelist
            switch (board.ReadPieceType(color, (PieceID)pieceID))
            {
                case PieceType::PAWN: piecePtr = std::make_unique<Pawn>(); break;
                case PieceType::ROOK: piecePtr = std::make_unique<Rook>(); break;
                case PieceType::KNIGHT: piecePtr = std::make_unique<Knight>(); break;
                case PieceType::BISHOP: piecePtr = std::make_unique<Bishop>(); break;
                case PieceType::QUEEN: piecePtr = std::make_unique<Queen>(); break;
                case PieceType::KING: piecePtr = std::make_unique<King>(); break;
                default: throw std::invalid_argument("Invalid PieceType");
            }
        }

    }

    std::unique_ptr<ChessPiece>& MoveList::GetPiecePtr(PieceID pieceID)
    {
        switch (pieceID)
        {
            case PIECE_1: return m_Piece1;
            case PIECE_2: return m_Piece2;
            case PIECE_3: return m_Piece3;
            case PIECE_4: return m_Piece4;
            case PIECE_5: return m_Piece5;
            case PIECE_6: return m_Piece6;
            case PIECE_7: return m_Piece7;
            case PIECE_8: return m_Piece8;
            case PIECE_9: return m_Piece9;
            case PIECE_10: return m_Piece10;
            case PIECE_11: return m_Piece11;
            case PIECE_12: return m_Piece12;
            case PIECE_13: return m_Piece13;
            case PIECE_14: return m_Piece14;
            case PIECE_15: return m_Piece15;
            case PIECE_16: return m_Piece16;
            default: throw std::invalid_argument("Invalid PieceID");
        }
    }

    const std::vector<uint8>& MoveList::GetRegularMoves(PieceID pieceID)
    {
        return GetPiecePtr(pieceID)->GetRegularMoves();
    }

    const std::vector<uint8>& MoveList::GetAttackingMoves(PieceID pieceID)
    {
        return GetPiecePtr(pieceID)->GetAttackingMoves();
    }


} // Game