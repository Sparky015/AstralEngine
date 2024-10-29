//
// Created by Andrew Fagan on 10/23/24.
//

#include <gtest/gtest.h>
#include "Game/Board.h"

class Board_FEN_ConstructorTest : public ::testing::Test
{
public:
    Board FEN_Board = Board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR");
    Board FEN2_Board = Board("r1bk3r/p2pBpNp/n4n2/1p1NP2P/6P1/3P4/P1P1K3/q5b1");
};

TEST_F(Board_FEN_ConstructorTest, FEN_Constructor_CheckingLocation1)
{
// Example test to check if board initialized correctly from FEN string
    EXPECT_EQ(FEN_Board.ReadPieceLocation(PieceColor::BLACK, PieceID::ROOK_1), A8);
    EXPECT_EQ(FEN_Board.ReadPieceLocation(PieceColor::WHITE, PieceID::PAWN_1), A2);
}

TEST_F(Board_FEN_ConstructorTest, FEN_Constructor_CheckingLocation2)
{
// Example test to check specific pieces from the second FEN string
    EXPECT_EQ(FEN2_Board.ReadPieceLocation(PieceColor::BLACK, PieceID::QUEEN), A1);
    EXPECT_EQ(FEN2_Board.ReadPieceLocation(PieceColor::WHITE, PieceID::BISHOP_1), E7);
}

TEST_F(Board_FEN_ConstructorTest, FEN_Constructor_CheckingLocation3)
{
// Example test to check if board initialized correctly from FEN string
    EXPECT_EQ(FEN2_Board.ReadPieceLocation(PieceColor::WHITE, PieceID::PAWN_3), D3);
    EXPECT_EQ(FEN2_Board.ReadPieceLocation(PieceColor::BLACK, PieceID::PAWN_1), A7);
}

