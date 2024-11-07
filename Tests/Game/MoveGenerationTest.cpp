//
// Created by Andrew Fagan on 11/6/24.
//

#include <gtest/gtest.h>
#include "Game/Board.h"
#include "Game/MoveGeneration.h"

using namespace Game;

class MoveGenerationTest : public ::testing::Test
{
public:
    void SetUp() override
    {
        GenerateMoveList(whiteMoveList, board, PieceColor::WHITE);
        GenerateMoveList(blackMoveList, board, PieceColor::BLACK);
    }

    Board board; // standard starting board setup
    MoveList whiteMoveList;
    MoveList blackMoveList;

};

TEST_F(MoveGenerationTest, BlackPawnRegularMove)
{
    EXPECT_EQ(blackMoveList.Pawn1.size(), 1);
    EXPECT_EQ(blackMoveList.Pawn1[0], 16);
}

TEST_F(MoveGenerationTest, WhitePawnRegularMove)
{
    EXPECT_EQ(blackMoveList.Pawn2.size(), 1);
    EXPECT_EQ(blackMoveList.Pawn2[0], 41);
}

TEST_F(MoveGenerationTest, BlackPawnBlockedMove)
{
    board.MovePiece(PieceColor::WHITE, PAWN_4, 41);
    EXPECT_EQ(blackMoveList.Pawn2.size(), 0);
}

TEST_F(MoveGenerationTest, WhitePawnBlockedMove)
{
    board.MovePiece(PieceColor::BLACK, ROOK_1, 46);
    EXPECT_EQ(blackMoveList.Pawn7.size(), 0);
}

TEST_F(MoveGenerationTest, WhitePawnEnpassantMove)
{
    board.MovePiece(PieceColor::WHITE, PAWN_2, 33);
    board.MovePiece(PieceColor::BLACK, PAWN_3, 34);
    EXPECT_EQ(whiteMoveList.Pawn2.size(), 2);
    EXPECT_EQ(whiteMoveList.Pawn2[0], 41); // normal pawn move
    EXPECT_EQ(whiteMoveList.Pawn2[1], 42); // enpassant attacking move
}

TEST_F(MoveGenerationTest, BlackPawnEnpassantMove)
{
    board.MovePiece(PieceColor::BLACK, PAWN_7, 38);
    board.MovePiece(PieceColor::WHITE, PAWN_6, 37);
    EXPECT_EQ(whiteMoveList.Pawn2.size(), 2);
    EXPECT_EQ(whiteMoveList.Pawn2[0], 46); // normal pawn move
    EXPECT_EQ(whiteMoveList.Pawn2[1], 45); // enpassant attacking move
}