//
// Created by Andrew Fagan on 10/21/24.
//


#include <gtest/gtest.h>
#include "Game/Board/Board.h"

using namespace Game;

class Board_MovePieceTest : public ::testing::Test
{
public:
    Board StandardBoard = Board();
    Board EmptyBoard = Board();

    void SetUp() override
    {
        EmptyBoard.ClearBoard();
    }
};


TEST_F(Board_MovePieceTest, MovePiece_UpdateLocation)
{
    EXPECT_NO_THROW(StandardBoard.MovePiece(PieceColor::BLACK, PieceID::BISHOP_2, E4));
    EXPECT_EQ(StandardBoard.ReadPieceLocation(PieceColor::BLACK, PieceID::BISHOP_2), E4);
    EXPECT_EQ(StandardBoard.ReadSquareType(E4), PieceType::BISHOP);
}

TEST_F(Board_MovePieceTest, MovePiece_UpdateLocation2)
{
    EXPECT_NO_THROW(StandardBoard.MovePiece(PieceColor::WHITE, PieceID::KING, H4));
    EXPECT_EQ(StandardBoard.ReadPieceLocation(PieceColor::WHITE, PieceID::KING), H4);
    EXPECT_EQ(StandardBoard.ReadSquareType(H4), PieceType::KING);
}

TEST_F(Board_MovePieceTest, MovePiece_UpdateLocation3)
{
    EXPECT_NO_THROW(EmptyBoard.MovePiece(PieceColor::BLACK, PieceID::BISHOP_2, G7));
    EXPECT_EQ(EmptyBoard.ReadPieceLocation(PieceColor::BLACK, PieceID::BISHOP_2), G7);
    EXPECT_EQ(EmptyBoard.ReadSquareType(G7), PieceType::BISHOP);
}

TEST_F(Board_MovePieceTest, MovePiece_UpdateLocation5)
{
    EXPECT_NO_THROW(EmptyBoard.MovePiece(PieceColor::WHITE, PieceID::ROOK_1, H1));
    EXPECT_EQ(EmptyBoard.ReadPieceLocation(PieceColor::WHITE, PieceID::ROOK_1), H1);
    EXPECT_EQ(EmptyBoard.ReadSquareType(H1), PieceType::ROOK);
}

TEST_F(Board_MovePieceTest, MovePiece_UpdateLocation6)
{
    EXPECT_NO_THROW(EmptyBoard.MovePiece(PieceColor::BLACK, PieceID::KNIGHT_1, A8));
    EXPECT_EQ(EmptyBoard.ReadPieceLocation(PieceColor::BLACK, PieceID::KNIGHT_1), A8);
    EXPECT_EQ(EmptyBoard.ReadSquareType(A8), PieceType::KNIGHT);
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

TEST_F(Board_MovePieceTest, MovePiece_SpaceOccupied1)
{
    EXPECT_THROW(StandardBoard.MovePiece(PieceColor::BLACK, PieceID::BISHOP_1, D7), std::logic_error);
}

TEST_F(Board_MovePieceTest, MovePiece_SpaceOccupied2)
{
    EXPECT_THROW(StandardBoard.MovePiece(PieceColor::WHITE, PieceID::BISHOP_1, D2), std::logic_error);
}
