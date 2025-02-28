//
// Created by Andrew Fagan on 11/24/24.
//

#include <gtest/gtest.h>

#include "Chessboard/Chessboard.h"
#include "Common/ChessPieceTypes.h"
#include "Common/ConversionHelpers.h"
#include "MoveGeneration/BoardMoveList.h"

namespace Game {

    class PawnMoveGenerationTest : public ::testing::Test
    {
    public:
        void SetUp() override
        {
            whiteMoveList.UpdateMoveList(FEN_board, PieceColor::WHITE);
            blackMoveList.UpdateMoveList(FEN_board, PieceColor::BLACK);
        }

        Chessboard FEN_board{Chessboard("rnbqk2r/p1p3p1/5n2/Pp1ppp1p/1b1P1PP1/2P1P3/1P1B3P/RN1QKBNR w KQkq b6 0 9")};
        BoardMoveList whiteMoveList{FEN_board, PieceColor::WHITE};
        BoardMoveList blackMoveList{FEN_board, PieceColor::BLACK};
    };

    // TEST_F(PawnMoveGenerationTest, RegularMoves1)
    // {
    //     PieceID pieceID = FEN_board.ReadSquarePieceID(A5);
    //     std::vector<uint8> regularMoves = whiteMoveList.GetRegularMoves(pieceID);
    //     EXPECT_NE(std::find(regularMoves.begin(), regularMoves.end(), A6), regularMoves.end());
    //     EXPECT_EQ(regularMoves.size(), 1);
    //     LOG(ConvertVectorToString(regularMoves) );
    // }
    //
    //
    // TEST_F(PawnMoveGenerationTest, RegularMoves2)
    // {
    //     PieceID pieceID = FEN_board.ReadSquarePieceID(B2);
    //     std::vector<uint8> regularMoves = whiteMoveList.GetRegularMoves(pieceID);
    //     EXPECT_NE(std::find(regularMoves.begin(), regularMoves.end(), B3), regularMoves.end());
    //     EXPECT_EQ(regularMoves.size(), 1);
    //     LOG(ConvertVectorToString(regularMoves));
    // }
    //
    //
    // TEST_F(PawnMoveGenerationTest, RegularMoves3)
    // {
    //     PieceID pieceID = FEN_board.ReadSquarePieceID(G4);
    //     std::vector<uint8> regularMoves = whiteMoveList.GetRegularMoves(pieceID);
    //     EXPECT_NE(std::find(regularMoves.begin(), regularMoves.end(), G5), regularMoves.end());
    //     EXPECT_EQ(regularMoves.size(), 1);
    //     LOG(ConvertVectorToString(regularMoves));
    // }
    //
    // TEST_F(PawnMoveGenerationTest, RegularMoves4)
    // {
    //     PieceID pieceID = FEN_board.ReadSquarePieceID(C7);
    //     PieceMoveList regularMoves = blackMoveList.GetRegularMoves(pieceID, PieceColor::BLACK);
    //     EXPECT_NE(std::find(regularMoves.begin(), regularMoves.end(), C6), regularMoves.end());
    //     EXPECT_NE(std::find(regularMoves.begin(), regularMoves.end(), C5), regularMoves.end());
    //     EXPECT_EQ(regularMoves.Size(), 2);
    //     LOG(ConvertVectorToString(regularMoves));
    // }
    //
    //
    // TEST_F(PawnMoveGenerationTest, AttackingMoves1)
    // {
    //     PieceID pieceID = FEN_board.ReadSquarePieceID(G4);
    //     std::vector<uint8> attackingMoves = whiteMoveList.GetAttackingMoves(pieceID);
    //     EXPECT_NE(std::find(attackingMoves.begin(), attackingMoves.end(), H5), attackingMoves.end());
    //     EXPECT_NE(std::find(attackingMoves.begin(), attackingMoves.end(), F5), attackingMoves.end());
    //     EXPECT_EQ(attackingMoves.size(), 2);
    //     LOG(ConvertVectorToString(attackingMoves));
    // }
    //
    //
    // TEST_F(PawnMoveGenerationTest, AttackingMoves2)
    // {
    //     PieceID pieceID = FEN_board.ReadSquarePieceID(D4);
    //     std::vector<uint8> attackingMoves = whiteMoveList.GetAttackingMoves(pieceID);
    //     EXPECT_NE(std::find(attackingMoves.begin(), attackingMoves.end(), E5), attackingMoves.end());
    //     EXPECT_EQ(attackingMoves.size(), 1);
    //     LOG(ConvertVectorToString(attackingMoves));
    // }
    //
    //
    // TEST_F(PawnMoveGenerationTest, AttackingMoves3)
    // {
    //     PieceID pieceID = FEN_board.ReadSquarePieceID(E5);
    //     std::vector<uint8> attackingMoves = blackMoveList.GetAttackingMoves(pieceID);
    //     EXPECT_NE(std::find(attackingMoves.begin(), attackingMoves.end(), D4), attackingMoves.end());
    //     EXPECT_NE(std::find(attackingMoves.begin(), attackingMoves.end(), F4), attackingMoves.end());
    //     EXPECT_EQ(attackingMoves.size(), 2);
    //     LOG(ConvertVectorToString(attackingMoves));
    // }
    //
    //
    // TEST_F(PawnMoveGenerationTest, EnPassantAttack)
    // {
    //     PieceID pieceID = FEN_board.ReadSquarePieceID(A5);
    //     std::vector<uint8> attackingMoves = whiteMoveList.GetAttackingMoves(pieceID);
    //     EXPECT_NE(std::find(attackingMoves.begin(), attackingMoves.end(), B6), attackingMoves.end());
    //     EXPECT_EQ(attackingMoves.size(), 1);
    //     LOG(ConvertVectorToString(attackingMoves));
    // }
    //
    // TEST_F(PawnMoveGenerationTest, DoubleMove)
    // {
    //     PieceID pieceID = FEN_board.ReadSquarePieceID(H2);
    //     std::vector<uint8> regularMoves = whiteMoveList.GetRegularMoves(pieceID);
    //     EXPECT_NE(std::find(regularMoves.begin(), regularMoves.end(), H4), regularMoves.end());
    //     EXPECT_NE(std::find(regularMoves.begin(), regularMoves.end(), H3), regularMoves.end());
    //     EXPECT_EQ(regularMoves.size(), 2);
    //     LOG(ConvertVectorToString(regularMoves) );
    // }

}
