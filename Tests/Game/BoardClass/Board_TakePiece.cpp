//
// Created by Andrew Fagan on 10/29/24.
//


#include <gtest/gtest.h>
#include "Game/Board/Board.h"

using namespace Game;

class Board_TakePieceTest : public ::testing::Test
{
public:
    Board board = Board();
};

TEST_F(Board_TakePieceTest, IsTakenPieceGone1)
{
    PieceType oldPieceType = board.ReadSquareType(E2);
    board.TakePiece(BLACK, ROOK_1, E2);
    EXPECT_NE(board.ReadSquareType(E2), oldPieceType);
    EXPECT_EQ(board.ReadSquareType(E2), PieceType::ROOK);
}

TEST_F(Board_TakePieceTest, IsTakenPieceGone2)
{
    PieceType oldPieceType = board.ReadSquareType(A8);
    board.TakePiece(WHITE, QUEEN, A8);
    EXPECT_NE(board.ReadSquareType(A8), oldPieceType);
    EXPECT_EQ(board.ReadSquareType(A8), PieceType::QUEEN);
}

TEST_F(Board_TakePieceTest, IsTakenPieceGone3)
{
    PieceType oldPieceType = board.ReadSquareType(F7);
    board.TakePiece(WHITE, KING, F7);
    EXPECT_NE(board.ReadSquareType(F7), oldPieceType);
    EXPECT_EQ(board.ReadSquareType(F7), PieceType::KING);
}


TEST_F(Board_TakePieceTest, SquareEmpty1)
{
    EXPECT_THROW(board.TakePiece(BLACK, PAWN_4, E3), std::logic_error);
}

TEST_F(Board_TakePieceTest, SquareEmpty2)
{
    EXPECT_THROW(board.TakePiece(WHITE, QUEEN, F5), std::logic_error);
}

TEST_F(Board_TakePieceTest, InvalidMoveLocation1)
{
        EXPECT_THROW(board.TakePiece(BLACK, PAWN_4, -3), std::out_of_range);
}

TEST_F(Board_TakePieceTest, InvalidMoveLocation2)
{
    EXPECT_THROW(board.TakePiece(WHITE, QUEEN, 64), std::out_of_range);
}

TEST_F(Board_TakePieceTest, OnMoveLocationBounds)
{
    EXPECT_NO_THROW(board.TakePiece(WHITE, QUEEN, A8));
    EXPECT_EQ(board.ReadSquareType(A8), PieceType::QUEEN);
}