//
// Created by Andrew Fagan on 10/21/24.
//


#include <gtest/gtest.h>
#include "Game/Board.h"

class Board_MovePieceTest : public ::testing::Test
{
public:
    Board EmptyBoard = Board(false);
    Board StandardBoard = Board(true);
};


TEST_F(Board_MovePieceTest, MovePiece_UpdateLocation)
{
    EXPECT_NO_THROW(StandardBoard.MovePiece(PieceColor::BLACK, PieceID::BISHOP_2, E4));
    EXPECT_EQ(StandardBoard.ReadPieceLocation(PieceColor::BLACK, PieceID::BISHOP_2), E4);
}

TEST_F(Board_MovePieceTest, MovePiece_UpdateLocation2)
{
    EXPECT_NO_THROW(StandardBoard.MovePiece(PieceColor::WHITE, PieceID::KING, H4));
    EXPECT_EQ(StandardBoard.ReadPieceLocation(PieceColor::WHITE, PieceID::KING), H4);
}

TEST_F(Board_MovePieceTest, MovePiece_UpdateLocation3)
{
    EXPECT_NO_THROW(EmptyBoard.MovePiece(PieceColor::BLACK, PieceID::BISHOP_2, G7));
    EXPECT_EQ(EmptyBoard.ReadPieceLocation(PieceColor::BLACK, PieceID::BISHOP_2), G7);
}

TEST_F(Board_MovePieceTest, MovePiece_UpdateLocation5)
{
    EXPECT_NO_THROW(EmptyBoard.MovePiece(PieceColor::WHITE, PieceID::ROOK_1, H1));
    EXPECT_EQ(EmptyBoard.ReadPieceLocation(PieceColor::WHITE, PieceID::ROOK_1), H1);
}

TEST_F(Board_MovePieceTest, MovePiece_UpdateLocation6)
{
    EXPECT_NO_THROW(EmptyBoard.MovePiece(PieceColor::BLACK, PieceID::KNIGHT_1, A8));
    EXPECT_EQ(EmptyBoard.ReadPieceLocation(PieceColor::BLACK, PieceID::KNIGHT_1), A8);
}

TEST_F(Board_MovePieceTest, MovePiece_LowerOutOfBounds)
{
    EXPECT_THROW(EmptyBoard.MovePiece(PieceColor::BLACK, PieceID::BISHOP_1, -1), std::out_of_range);
}

TEST_F(Board_MovePieceTest, MovePiece_HigherOutOfBounds)
{
    EXPECT_THROW(EmptyBoard.MovePiece(PieceColor::WHITE, PieceID::BISHOP_1, 73), std::out_of_range);
}

TEST_F(Board_MovePieceTest, MovePiece_LowerOnBound)
{
    EXPECT_NO_THROW(EmptyBoard.MovePiece(PieceColor::WHITE, PieceID::BISHOP_1, 0));
    EXPECT_EQ(EmptyBoard.ReadPieceLocation(PieceColor::WHITE, PieceID::BISHOP_1), 0);
}

TEST_F(Board_MovePieceTest, MovePiece_HigherOnBound)
{
    EXPECT_NO_THROW(EmptyBoard.MovePiece(PieceColor::BLACK, PieceID::BISHOP_2, 63));
    EXPECT_EQ(EmptyBoard.ReadPieceLocation(PieceColor::BLACK, PieceID::BISHOP_2), 63);
}