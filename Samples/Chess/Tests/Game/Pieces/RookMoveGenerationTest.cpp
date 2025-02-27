//
// Created by Andrew Fagan on 11/24/24.
//

#include <gtest/gtest.h>

#include "../../../../Source/Chess/GameState/ChessPieceTypes.h"
#include "Chessboard/Chessboard.h"
#include "MoveGeneration/BoardMoveList.h"
#include "../../../../Source/Chess/Common/ConversionHelpers.h"

namespace Game {

    class RookMoveGenerationTest : public ::testing::Test
    {
    public:
        void SetUp() override
        {
            whiteMoveList.UpdateMoveList(FEN_board, PieceColor::WHITE);
            blackMoveList.UpdateMoveList(FEN_board, PieceColor::BLACK);
        }

        Chessboard FEN_board{Chessboard("5kr1/1p2qp2/1Qb1p1pp/p1ppn1R1/PrN1PBnP/1RP1bN2/1P2KPP1/5B2 w - - 3 23")};
        BoardMoveList whiteMoveList{FEN_board, PieceColor::WHITE};
        BoardMoveList blackMoveList{FEN_board, PieceColor::BLACK};
    };
    //
    // TEST_F(RookMoveGenerationTest, RegularMoves1)
    // {
    //     PieceID pieceID = FEN_board.ReadSquarePieceID(G8);
    //     std::vector<uint8> regularMoves = blackMoveList.GetRegularMoves(pieceID);
    //     EXPECT_NE(std::find(regularMoves.begin(), regularMoves.end(), G7), regularMoves.end());
    //     EXPECT_NE(std::find(regularMoves.begin(), regularMoves.end(), H8), regularMoves.end());
    //     EXPECT_EQ(regularMoves.size(), 2);
    //     LOG(ConvertVectorToString(regularMoves) );
    // }
    //
    //
    // TEST_F(RookMoveGenerationTest, RegularMoves2)
    // {
    //     PieceID pieceID = FEN_board.ReadSquarePieceID(G5);
    //     std::vector<uint8> regularMoves = whiteMoveList.GetRegularMoves(pieceID);
    //     EXPECT_NE(std::find(regularMoves.begin(), regularMoves.end(), H5), regularMoves.end());
    //     EXPECT_NE(std::find(regularMoves.begin(), regularMoves.end(), F5), regularMoves.end());
    //     EXPECT_EQ(regularMoves.size(), 2);
    //     LOG(ConvertVectorToString(regularMoves));
    // }
    //
    //
    // TEST_F(RookMoveGenerationTest, RegularMoves3)
    // {
    //     PieceID pieceID = FEN_board.ReadSquarePieceID(B4);
    //     std::vector<uint8> regularMoves = blackMoveList.GetRegularMoves(pieceID);
    //     EXPECT_NE(std::find(regularMoves.begin(), regularMoves.end(),  B5), regularMoves.end());
    //     EXPECT_EQ(regularMoves.size(), 1);
    //     LOG(ConvertVectorToString(regularMoves));
    // }
    //
    // TEST_F(RookMoveGenerationTest, RegularMoves4)
    // {
    //     PieceID pieceID = FEN_board.ReadSquarePieceID(B3);
    //     std::vector<uint8> regularMoves = whiteMoveList.GetRegularMoves(pieceID);
    //     EXPECT_NE(std::find(regularMoves.begin(), regularMoves.end(), A3), regularMoves.end());
    //     EXPECT_EQ(regularMoves.size(), 1);
    //     LOG(ConvertVectorToString(regularMoves));
    // }
    //
    //
    // TEST_F(RookMoveGenerationTest, AttackingMoves1)
    // {
    //     PieceID pieceID = FEN_board.ReadSquarePieceID(B4);
    //     std::vector<uint8> attackingMoves = blackMoveList.GetAttackingMoves(pieceID);
    //     EXPECT_NE(std::find(attackingMoves.begin(), attackingMoves.end(), A4), attackingMoves.end());
    //     EXPECT_NE(std::find(attackingMoves.begin(), attackingMoves.end(), C4), attackingMoves.end());
    //     EXPECT_NE(std::find(attackingMoves.begin(), attackingMoves.end(), B3), attackingMoves.end());
    //     EXPECT_NE(std::find(attackingMoves.begin(), attackingMoves.end(), B6), attackingMoves.end());
    //     EXPECT_EQ(attackingMoves.size(), 4);
    //     LOG(ConvertVectorToString(attackingMoves));
    // }
    //
    //
    // TEST_F(RookMoveGenerationTest, AttackingMoves2)
    // {
    //     PieceID pieceID = FEN_board.ReadSquarePieceID(G5);
    //     std::vector<uint8> attackingMoves = whiteMoveList.GetAttackingMoves(pieceID);
    //     EXPECT_NE(std::find(attackingMoves.begin(), attackingMoves.end(), E5), attackingMoves.end());
    //     EXPECT_NE(std::find(attackingMoves.begin(), attackingMoves.end(), G4), attackingMoves.end());
    //     EXPECT_NE(std::find(attackingMoves.begin(), attackingMoves.end(), G6), attackingMoves.end());
    //     EXPECT_EQ(attackingMoves.size(), 3);
    //     LOG(ConvertVectorToString(attackingMoves));
    // }
    //
    //
    // TEST_F(RookMoveGenerationTest, AttackingMoves3)
    // {
    //     PieceID pieceID = FEN_board.ReadSquarePieceID(G8);
    //     std::vector<uint8> attackingMoves = blackMoveList.GetAttackingMoves(pieceID);
    //     EXPECT_EQ(std::find(attackingMoves.begin(), attackingMoves.end(), G6), attackingMoves.end());
    //     EXPECT_EQ(std::find(attackingMoves.begin(), attackingMoves.end(), G8), attackingMoves.end());
    //     EXPECT_EQ(attackingMoves.size(), 0);
    //     LOG(ConvertVectorToString(attackingMoves));
    // }

}