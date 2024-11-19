//
// Created by Andrew Fagan on 11/6/24.
//

#include <gtest/gtest.h>
#include "Game/Board/Board.h"
#include "Game/MoveGeneration.h"

using namespace Game;

class MoveGenerationTest : public ::testing::Test
{
public:
    void SetUp() override
    {
        moveListGen.GenerateMoveList(&blackMoveList, &board, PieceColor::BLACK);
        moveListGen.GenerateMoveList(&whiteMoveList, &board, PieceColor::WHITE);
    }

    Board board; // standard starting board setup
    MoveListGenerator moveListGen;

    MoveList blackMoveList;
    MoveList whiteMoveList;
};

// Tests for the rook piece that checks if the move list generated is correct and that the moves are valid for the rook
TEST_F(MoveGenerationTest, BlackRookRegularMove)
{
    Board emptyBoard = Board();
    emptyBoard.ClearBoard();
    moveListGen.GenerateMoveList(&blackMoveList, &emptyBoard, PieceColor::BLACK);
    EXPECT_EQ(blackMoveList.Rook1.size(), 14); // Rook can move to 0 squares in the initial position
    EXPECT_EQ(blackMoveList.Rook1[0], BoardLocation::A6); // First move is one step forward
    EXPECT_EQ(blackMoveList.Rook1[13], BoardLocation::A1); // Last move is to the end of the board
}

TEST_F(MoveGenerationTest, WhiteRookRegularMove)
{
    EXPECT_EQ(whiteMoveList.Rook1.size(), 14); // Rook can move to 14 squares in the initial position
    EXPECT_EQ(whiteMoveList.Rook1[0], BoardLocation::A3); // First move is one step forward
    EXPECT_EQ(whiteMoveList.Rook1[13], BoardLocation::A8); // Last move is to the end of the board
}

TEST_F(MoveGenerationTest, BlackRookBlockedMove)
{
    // White pawn blocks black rook
    board.MovePiece(PieceColor::WHITE, PAWN_2, BoardLocation::A6);
    moveListGen.GenerateMoveList(&blackMoveList, &board, PieceColor::BLACK);
    EXPECT_EQ(blackMoveList.Rook1.size(), 0); // Rook cannot move
}

TEST_F(MoveGenerationTest, WhiteRookBlockedMove)
{
    // Black pawn blocks white rook
    board.MovePiece(PieceColor::BLACK, PAWN_7, BoardLocation::A3);
    moveListGen.GenerateMoveList(&whiteMoveList, &board, PieceColor::WHITE);
    EXPECT_EQ(whiteMoveList.Rook1.size(), 0); // Rook cannot move
}

TEST_F(MoveGenerationTest, BlackRookCaptureMove)
{
    // White pawn in capture position
    board.MovePiece(PieceColor::WHITE, PAWN_2, BoardLocation::A6);
    moveListGen.GenerateMoveList(&blackMoveList, &board, PieceColor::BLACK);
    EXPECT_EQ(blackMoveList.Rook1.size(), 1); // Rook can capture the pawn
    EXPECT_EQ(blackMoveList.Rook1[0], BoardLocation::A6); // Capture move
}

TEST_F(MoveGenerationTest, WhiteRookCaptureMove)
{
    // Black pawn in capture position
    board.MovePiece(PieceColor::BLACK, PAWN_7, BoardLocation::A3);
    moveListGen.GenerateMoveList(&whiteMoveList, &board, PieceColor::WHITE);
    EXPECT_EQ(whiteMoveList.Rook1.size(), 1); // Rook can capture the pawn
    EXPECT_EQ(whiteMoveList.Rook1[0], BoardLocation::A3); // Capture move
}

