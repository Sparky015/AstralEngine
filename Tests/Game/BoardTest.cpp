//
// Created by Andrew Fagan on 10/21/24.
//



#include <gtest/gtest.h>
#include "Game/Board.h"

class BoardTest : public ::testing::Test
{
public:
    Board StandardBoard = Board();
    //Board FEN_Board = Board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR");
    //Board FEN2_Board = Board("r1bk3r/p2pBpNp/n4n2/1p1NP2P/6P1/3P4/P1P1K3/q5b1");
};

TEST_F(BoardTest, MovePiece_UpdateLocation)
{
    StandardBoard.MovePiece(PieceColor::BLACK, PieceID::BISHOP_2, B4);
    EXPECT_EQ(StandardBoard.ReadBoardLocation(PieceColor::BLACK, PieceID::BISHOP_2), B4);
}

TEST_F(BoardTest, MovePiece_LowerOutOfBounds)
{
    EXPECT_THROW(StandardBoard.MovePiece(PieceColor::BLACK, PieceID::BISHOP_1, -1), std::out_of_range);
}