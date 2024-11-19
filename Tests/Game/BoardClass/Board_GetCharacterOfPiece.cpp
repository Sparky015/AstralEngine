//
// Created by Andrew Fagan on 11/2/24.
//


#include <gtest/gtest.h>
#include "Game/Board/Board.h"

using namespace Game;

class Board_GetCharacterOfPiece : public ::testing::Test
{
public:
    Board board = Board();
};

TEST_F(Board_GetCharacterOfPiece, CheckReturnAccuracy1)
{
    EXPECT_EQ(board.GetCharacterOfPiece(A8), 'r');
    EXPECT_EQ(board.GetCharacterOfPiece(B8), 'n');
    EXPECT_EQ(board.GetCharacterOfPiece(C8), 'b');

    EXPECT_EQ(board.GetCharacterOfPiece(A1), 'R');
    EXPECT_EQ(board.GetCharacterOfPiece(B1), 'N');
    EXPECT_EQ(board.GetCharacterOfPiece(C1), 'B');
}

TEST_F(Board_GetCharacterOfPiece, CheckReturnAccuracy2)
{
    EXPECT_EQ(board.GetCharacterOfPiece(E4), '_');
    EXPECT_NE(board.GetCharacterOfPiece(A8), '_');
    EXPECT_NE(board.GetCharacterOfPiece(C2), '_');
    EXPECT_NE(board.GetCharacterOfPiece(B1), '_');
}