//
// Created by Andrew Fagan on 10/29/24.
//

#include <gtest/gtest.h>
#include "Game/Board/Board.h"

using namespace Game;

class Board_ReadSquarePieceID : public ::testing::Test
{
public:
    Board board = Board();
};

TEST_F(Board_ReadSquarePieceID, CheckPieceIDAccuracy1)
{
    EXPECT_EQ(board.ReadSquarePieceID(A8), ROOK_1);
}

TEST_F(Board_ReadSquarePieceID, CheckPieceIDAccuracy2)
{
    EXPECT_EQ(board.ReadSquarePieceID(E2), PAWN_5);
}

TEST_F(Board_ReadSquarePieceID, CheckPieceIDAccuracy3)
{
    EXPECT_EQ(board.ReadSquarePieceID(B1), KNIGHT_1);
}

TEST_F(Board_ReadSquarePieceID, CheckPieceIDAccuracy4)
{
    EXPECT_EQ(board.ReadSquarePieceID(F1), BISHOP_2);
}

TEST_F(Board_ReadSquarePieceID, SquareEmpty1)
{
    EXPECT_THROW(board.ReadSquarePieceID(F5), std::logic_error);
}

TEST_F(Board_ReadSquarePieceID, SquareEmpty2)
{
    EXPECT_THROW(board.ReadSquarePieceID(G3), std::logic_error);
}


