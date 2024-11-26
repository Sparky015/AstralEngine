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
    Board FENBoard = Board("1Q6/8/8/8/8/8/8/8");

    void SetUp() override
    {
        EmptyBoard.ClearBoard();
    }
};


TEST_F(Board_MovePieceTest, MovePiece_UpdateLocation)
{
    EXPECT_NO_THROW(StandardBoard.MovePiece(PieceColor::BLACK, PieceID::PIECE_14, E4));
    EXPECT_EQ(StandardBoard.ReadPieceLocation(PieceColor::BLACK, PieceID::PIECE_14), E4);
    EXPECT_EQ(StandardBoard.ReadSquareType(E4), PieceType::BISHOP);
}

TEST_F(Board_MovePieceTest, MovePiece_UpdateLocation2)
{
    EXPECT_NO_THROW(StandardBoard.MovePiece(PieceColor::WHITE, PieceID::PIECE_13, H4));
    EXPECT_EQ(StandardBoard.ReadPieceLocation(PieceColor::WHITE, PieceID::PIECE_13), H4);
    EXPECT_EQ(StandardBoard.ReadSquareType(H4), PieceType::KING);
}

TEST_F(Board_MovePieceTest, MovePiece_UpdateLocation3)
{
    EXPECT_NO_THROW(StandardBoard.MovePiece(PieceColor::BLACK, PieceID::PIECE_14, G6));
    EXPECT_EQ(StandardBoard.ReadPieceLocation(PieceColor::BLACK, PieceID::PIECE_14), G6);
    EXPECT_EQ(StandardBoard.ReadSquareType(G6), PieceType::BISHOP);
}

TEST_F(Board_MovePieceTest, MovePiece_UpdateLocation5)
{
    EXPECT_NO_THROW(StandardBoard.MovePiece(PieceColor::WHITE, PieceID::PIECE_9, H3));
    EXPECT_EQ(StandardBoard.ReadPieceLocation(PieceColor::WHITE, PieceID::PIECE_9), H3);
    EXPECT_EQ(StandardBoard.ReadSquareType(H3), PieceType::ROOK);
}

TEST_F(Board_MovePieceTest, MovePiece_UpdateLocation6)
{
    EXPECT_NO_THROW(FENBoard.MovePiece(PieceColor::WHITE, PieceID::PIECE_1, A8));
    EXPECT_EQ(FENBoard.ReadPieceLocation(PieceColor::WHITE, PieceID::PIECE_1), A8);
    EXPECT_EQ(FENBoard.ReadSquareType(A8), PieceType::QUEEN);
}

TEST_F(Board_MovePieceTest, MovePiece_LowerOutOfBounds)
{
    EXPECT_THROW(StandardBoard.MovePiece(PieceColor::BLACK, PieceID::PIECE_11, -1), std::out_of_range);
}

TEST_F(Board_MovePieceTest, MovePiece_HigherOutOfBounds)
{
    EXPECT_THROW(StandardBoard.MovePiece(PieceColor::WHITE, PieceID::PIECE_11, 73), std::out_of_range);
}

TEST_F(Board_MovePieceTest, MovePiece_LowerOnBound)
{
    EXPECT_NO_THROW(FENBoard.MovePiece(PieceColor::WHITE, PieceID::PIECE_1, 0));
    EXPECT_EQ(FENBoard.ReadPieceLocation(PieceColor::WHITE, PieceID::PIECE_1), 0);
}

TEST_F(Board_MovePieceTest, MovePiece_HigherOnBound)
{
    EXPECT_NO_THROW(FENBoard.MovePiece(PieceColor::WHITE, PieceID::PIECE_1, 63));
    EXPECT_EQ(FENBoard.ReadPieceLocation(PieceColor::WHITE, PieceID::PIECE_1), 63);
}

TEST_F(Board_MovePieceTest, MovePiece_SpaceOccupied1)
{
    EXPECT_THROW(StandardBoard.MovePiece(PieceColor::BLACK, PieceID::PIECE_11, D7), std::logic_error);
}

TEST_F(Board_MovePieceTest, MovePiece_SpaceOccupied2)
{
    EXPECT_THROW(StandardBoard.MovePiece(PieceColor::WHITE, PieceID::PIECE_11, D2), std::logic_error);
}
