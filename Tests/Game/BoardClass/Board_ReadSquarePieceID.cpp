//
// Created by Andrew Fagan on 10/29/24.
//

#include <gtest/gtest.h>
#include "Game/Board.h"

using namespace Game;

class Board_ReadSquarePieceID : public ::testing::Test
{
public:
    Board board = Board();
};

TEST_F(Board_ReadSquarePieceID, CheckIDAccuracy1)
{
    EXPECT_EQ(board.ReadSquarePieceID(A8), ROOK_1);
}

TEST_F(Board_ReadSquarePieceID, CheckIDAccuracy2)
{
    EXPECT_EQ(board.ReadSquarePieceID(E2), PAWN_5);
}

TEST_F(Board_ReadSquarePieceID, CheckIDAccuracy3)
{
    EXPECT_EQ(board.ReadSquarePieceID(B1), KNIGHT_1);
}

TEST_F(Board_ReadSquarePieceID, CheckIDAccuracy4)
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

TEST_F(Board_ReadSquarePieceID, MessedUpMoveOrTake1)
{
    board.WriteSquareType(G3, PieceType::ROOK);
    EXPECT_THROW(board.ReadSquarePieceID(G3), std::logic_error);
}

TEST_F(Board_ReadSquarePieceID, MessedUpMoveOrTake2)
{
    board.WriteSquareType(B4, PieceType::PAWN);
    EXPECT_THROW(board.ReadSquarePieceID(B4), std::logic_error);
}

