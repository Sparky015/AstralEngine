//
// Created by Andrew Fagan on 10/29/24.
//

#include <gtest/gtest.h>
#include "Game/Board/Board.h"

using namespace Game;

class Board_PromotePawnTest : public ::testing::Test
{
public:
    Board board = Board();
};

TEST_F(Board_PromotePawnTest, CheckType1)
{
    board.PromotePawn(BLACK, PAWN_5, PieceType::ROOK);
    EXPECT_EQ(board.ReadSquareType(board.ReadPieceLocation(BLACK, PAWN_5)), PieceType::ROOK);
}

TEST_F(Board_PromotePawnTest, CheckType2)
{
    board.PromotePawn(WHITE, PAWN_2, PieceType::QUEEN);
    EXPECT_EQ(board.ReadSquareType(board.ReadPieceLocation(WHITE, PAWN_2)), PieceType::QUEEN);
}

TEST_F(Board_PromotePawnTest, CheckType3)
{
    board.PromotePawn(BLACK, PAWN_4, PieceType::BISHOP);
    EXPECT_EQ(board.ReadSquareType(board.ReadPieceLocation(BLACK, PAWN_4)), PieceType::BISHOP);
}

TEST_F(Board_PromotePawnTest, InvalidPromotionType)
{
    EXPECT_THROW(board.PromotePawn(BLACK, PAWN_4, PieceType::KING), std::logic_error);
}

TEST_F(Board_PromotePawnTest, InvalidPieceID)
{
    EXPECT_THROW(board.PromotePawn(WHITE, ROOK_1, PieceType::KNIGHT), std::logic_error);
}