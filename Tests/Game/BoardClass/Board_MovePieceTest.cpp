//
// Created by Andrew Fagan on 10/21/24.
//


#include <gtest/gtest.h>
#include "Game/Board.h"

class BoardTest : public ::testing::Test
{
public:
    Board EmptyBoard = Board(false);
    Board StandardBoard = Board(true);
};


TEST_F(BoardTest, MovePiece_UpdateLocation)
{
    EXPECT_NO_THROW(StandardBoard.MovePiece(PieceColor::BLACK, PieceID::BISHOP_2, E4));
    EXPECT_EQ(StandardBoard.ReadBoardLocation(PieceColor::BLACK, PieceID::BISHOP_2), E4);
}

TEST_F(BoardTest, MovePiece_UpdateLocation2)
{
    EXPECT_NO_THROW(StandardBoard.MovePiece(PieceColor::WHITE, PieceID::PAWN_8, H4));
    EXPECT_EQ(StandardBoard.ReadBoardLocation(PieceColor::BLACK, PieceID::BISHOP_2), H4);
}

TEST_F(BoardTest, MovePiece_UpdateLocation3)
{
    EXPECT_NO_THROW(EmptyBoard.MovePiece(PieceColor::WHITE, PieceID::PAWN_8, G7));
    EXPECT_EQ(EmptyBoard.ReadBoardLocation(PieceColor::BLACK, PieceID::BISHOP_2), G7);
}

TEST_F(BoardTest, MovePiece_UpdateLocation5)
{
    EXPECT_NO_THROW(EmptyBoard.MovePiece(PieceColor::WHITE, PieceID::PAWN_8, H1));
    EXPECT_EQ(EmptyBoard.ReadBoardLocation(PieceColor::BLACK, PieceID::BISHOP_2), H1);
}

TEST_F(BoardTest, MovePiece_UpdateLocation6)
{
    EXPECT_NO_THROW(EmptyBoard.MovePiece(PieceColor::WHITE, PieceID::PAWN_8, A8));
    EXPECT_EQ(EmptyBoard.ReadBoardLocation(PieceColor::BLACK, PieceID::BISHOP_2), A8);
}

TEST_F(BoardTest, MovePiece_LowerOutOfBounds)
{
    EXPECT_THROW(EmptyBoard.MovePiece(PieceColor::BLACK, PieceID::BISHOP_1, -1), std::out_of_range);
}

TEST_F(BoardTest, MovePiece_HigherOutOfBounds)
{
    EXPECT_THROW(EmptyBoard.MovePiece(PieceColor::WHITE, PieceID::BISHOP_1, 73), std::out_of_range);
}

TEST_F(BoardTest, MovePiece_LowerOnBound)
{
    EXPECT_NO_THROW(EmptyBoard.MovePiece(PieceColor::WHITE, PieceID::BISHOP_1, 0));
    EXPECT_EQ(EmptyBoard.ReadBoardLocation(PieceColor::BLACK, PieceID::BISHOP_1), A1);
}

TEST_F(BoardTest, MovePiece_HigherOnBound)
{
    EXPECT_NO_THROW(EmptyBoard.MovePiece(PieceColor::BLACK, PieceID::BISHOP_1, 63));
    EXPECT_EQ(EmptyBoard.ReadBoardLocation(PieceColor::BLACK, PieceID::BISHOP_2), H8);
}

TEST_F(BoardTest, MovePiece_SquareAlreadyOccupied)
{
    EXPECT_THROW(StandardBoard.MovePiece(PieceColor::BLACK, PieceID::BISHOP_1, A2), std::logic_error);
}

TEST_F(BoardTest, MovePiece_SquareAlreadyOccupied2)
{
    EXPECT_THROW(StandardBoard.MovePiece(PieceColor::WHITE, PieceID::BISHOP_1, H2), std::logic_error);
}



