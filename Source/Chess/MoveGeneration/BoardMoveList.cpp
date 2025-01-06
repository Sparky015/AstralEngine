//
// Created by Andrew Fagan on 11/18/24.
//

#include "BoardMoveList.h"

#include "PieceMoveLists/PawnMoveList.h"
#include "PieceMoveLists/RookMoveList.h"
#include "PieceMoveLists/KnightMoveList.h"
#include "PieceMoveLists/BishopMoveList.h"
#include "PieceMoveLists/QueenMoveList.h"
#include "PieceMoveLists/KingMoveList.h"


namespace Game {

    BoardMoveList::BoardMoveList(Chessboard& board, PieceColor color) :
    m_WhitePiece1(nullptr), m_WhitePiece2(nullptr), m_WhitePiece3(nullptr), m_WhitePiece4(nullptr),
    m_WhitePiece5(nullptr), m_WhitePiece6(nullptr), m_WhitePiece7(nullptr), m_WhitePiece8(nullptr),
    m_WhitePiece9(nullptr), m_WhitePiece10(nullptr), m_WhitePiece11(nullptr), m_WhitePiece12(nullptr),
    m_WhitePiece13(nullptr), m_WhitePiece14(nullptr), m_WhitePiece15(nullptr), m_WhitePiece16(nullptr),
    m_BlackPiece1(nullptr), m_BlackPiece2(nullptr), m_BlackPiece3(nullptr), m_BlackPiece4(nullptr),
    m_BlackPiece5(nullptr), m_BlackPiece6(nullptr), m_BlackPiece7(nullptr), m_BlackPiece8(nullptr),
    m_BlackPiece9(nullptr), m_BlackPiece10(nullptr), m_BlackPiece11(nullptr), m_BlackPiece12(nullptr),
    m_BlackPiece13(nullptr), m_BlackPiece14(nullptr), m_BlackPiece15(nullptr), m_BlackPiece16(nullptr)
    {
        PROFILE_SCOPE("BoardMoveList Construction");
        SyncPieceTypesToBoard(board, color);
    }


    void BoardMoveList::UpdateMoveList(const Chessboard& board, PieceColor color)
    {
        PROFILE_SCOPE("BoardMoveList::UpdateMoveList");
        // First sync the piece types of the board to the move list (in case of pawn promotions)
        SyncPieceTypesToBoard(board, color);

        // Generate the moves of each piece
        for (uint8 pieceID = PIECE_1; pieceID != PIECE_16 + 1; pieceID++)
        {
            std::unique_ptr<ChessPieceMoveList>& piecePtr = GetPiecePtr((PieceID)pieceID, color);

            if (piecePtr)
            {
                SquareLocation pieceLocation = board.GetPieceLocation((PieceID)pieceID, color);
                piecePtr->GenerateMoves(board, pieceLocation, color);
            }
        }

    }


    void BoardMoveList::SyncPieceTypesToBoard(const Chessboard& board, PieceColor color)
    {
        PROFILE_SCOPE("BoardMoveList::SyncPieceTypesToBoard");
        // Loop through each piece in the board and look at its type
        for (uint8 pieceID = PIECE_1; pieceID != PIECE_16 + 1; pieceID++)
        {
            std::unique_ptr<ChessPieceMoveList>& piecePtr = GetPiecePtr((PieceID)pieceID, color);

            // Check to see if the piece types already match
            if (piecePtr && board.GetPieceType((PieceID)pieceID, color) == piecePtr->GetType())
            {
                continue;
            }

            // Make a unique pointer based on its type and assign it to the piece in movelist
            PieceType pieceType = board.GetPieceType((PieceID)pieceID, color);
            switch (pieceType.GetEnumValue())
            {
                case PieceType::PAWN: piecePtr = std::make_unique<PawnMoveList>(); break;
                case PieceType::ROOK: piecePtr = std::make_unique<RookMoveList>(); break;
                case PieceType::KNIGHT: piecePtr = std::make_unique<KnightMoveList>(); break;
                case PieceType::BISHOP: piecePtr = std::make_unique<BishopMoveList>(); break;
                case PieceType::QUEEN: piecePtr = std::make_unique<QueenMoveList>(); break;
                case PieceType::KING: piecePtr = std::make_unique<KingMoveList>(); break;
                case PieceType::NONE: piecePtr = nullptr; break;
                default: throw std::invalid_argument("Invalid PieceType");
            }
        }

    }

    std::unique_ptr<ChessPieceMoveList>& BoardMoveList::GetPiecePtr(PieceID pieceID, PieceColor pieceColor)
    {
        if (pieceColor.IsBlack())
        {
            switch (pieceID)
            {
                case PIECE_1: return m_BlackPiece1;
                case PIECE_2: return m_BlackPiece2;
                case PIECE_3: return m_BlackPiece3;
                case PIECE_4: return m_BlackPiece4;
                case PIECE_5: return m_BlackPiece5;
                case PIECE_6: return m_BlackPiece6;
                case PIECE_7: return m_BlackPiece7;
                case PIECE_8: return m_BlackPiece8;
                case PIECE_9: return m_BlackPiece9;
                case PIECE_10: return m_BlackPiece10;
                case PIECE_11: return m_BlackPiece11;
                case PIECE_12: return m_BlackPiece12;
                case PIECE_13: return m_BlackPiece13;
                case PIECE_14: return m_BlackPiece14;
                case PIECE_15: return m_BlackPiece15;
                case PIECE_16: return m_BlackPiece16;
                default: ERROR("Invalid PieceID");
            }
        }
        else
        {
            switch (pieceID)
            {
                case PIECE_1: return m_WhitePiece1;
                case PIECE_2: return m_WhitePiece2;
                case PIECE_3: return m_WhitePiece3;
                case PIECE_4: return m_WhitePiece4;
                case PIECE_5: return m_WhitePiece5;
                case PIECE_6: return m_WhitePiece6;
                case PIECE_7: return m_WhitePiece7;
                case PIECE_8: return m_WhitePiece8;
                case PIECE_9: return m_WhitePiece9;
                case PIECE_10: return m_WhitePiece10;
                case PIECE_11: return m_WhitePiece11;
                case PIECE_12: return m_WhitePiece12;
                case PIECE_13: return m_WhitePiece13;
                case PIECE_14: return m_WhitePiece14;
                case PIECE_15: return m_WhitePiece15;
                case PIECE_16: return m_WhitePiece16;
                default: ERROR("Invalid PieceID");
            }
        }
    }



    const std::unique_ptr<ChessPieceMoveList>& BoardMoveList::GetPiecePtr(PieceID pieceID, PieceColor pieceColor) const
    {
        if (pieceColor == PieceColor::BLACK)
        {
            switch (pieceID)
            {
                case PIECE_1: return m_BlackPiece1;
                case PIECE_2: return m_BlackPiece2;
                case PIECE_3: return m_BlackPiece3;
                case PIECE_4: return m_BlackPiece4;
                case PIECE_5: return m_BlackPiece5;
                case PIECE_6: return m_BlackPiece6;
                case PIECE_7: return m_BlackPiece7;
                case PIECE_8: return m_BlackPiece8;
                case PIECE_9: return m_BlackPiece9;
                case PIECE_10: return m_BlackPiece10;
                case PIECE_11: return m_BlackPiece11;
                case PIECE_12: return m_BlackPiece12;
                case PIECE_13: return m_BlackPiece13;
                case PIECE_14: return m_BlackPiece14;
                case PIECE_15: return m_BlackPiece15;
                case PIECE_16: return m_BlackPiece16;
                default: ERROR("Invalid PieceID");
            }
        }
        else
        {
            switch (pieceID)
            {
                case PIECE_1: return m_WhitePiece1;
                case PIECE_2: return m_WhitePiece2;
                case PIECE_3: return m_WhitePiece3;
                case PIECE_4: return m_WhitePiece4;
                case PIECE_5: return m_WhitePiece5;
                case PIECE_6: return m_WhitePiece6;
                case PIECE_7: return m_WhitePiece7;
                case PIECE_8: return m_WhitePiece8;
                case PIECE_9: return m_WhitePiece9;
                case PIECE_10: return m_WhitePiece10;
                case PIECE_11: return m_WhitePiece11;
                case PIECE_12: return m_WhitePiece12;
                case PIECE_13: return m_WhitePiece13;
                case PIECE_14: return m_WhitePiece14;
                case PIECE_15: return m_WhitePiece15;
                case PIECE_16: return m_WhitePiece16;
                default: ERROR("Invalid PieceID");
            }
        }
    }



    const PieceMoveList& BoardMoveList::GetRegularMoves(PieceID pieceID, PieceColor pieceColor) const
    {
        if (!GetPiecePtr(pieceID, pieceColor)) {static PieceMoveList emptyList; return emptyList;} // if the ptr is null
        return GetPiecePtr(pieceID, pieceColor)->GetRegularMoves();
    }

    const PieceMoveList& BoardMoveList::GetAttackingMoves(PieceID pieceID, PieceColor pieceColor) const
    {
        if (!GetPiecePtr(pieceID, pieceColor)) {static PieceMoveList emptyList; return emptyList;} // if the ptr is null
        return GetPiecePtr(pieceID, pieceColor)->GetAttackingMoves();
    }


} // Game