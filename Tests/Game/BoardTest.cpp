//
// Created by Andrew Fagan on 10/21/24.
//



#include <gtest/gtest.h>
#include "Game/Board.h"

class BoardTest : public ::testing::Test
{
public:
    Board StandardBoard = Board();
};

TEST_F(BoardTest, MovePiece_Test)
{
    StandardBoard.MovePiece(PieceColor::BLACK, PieceID::BISHOP_2, B4);
    EXPECT_EQ(StandardBoard.ReadBoardLocation(PieceColor::BLACK, PieceID::BISHOP_2), B4);
}

TEST_F(BoardTest, MovePiece_LowerOutOfBounds)
{
    EXPECT_THROW(StandardBoard.MovePiece(PieceColor::BLACK, PieceID::BISHOP_1, -1), std::out_of_range);
}




class FENBoardTest : public BoardTest {
public:
    Board FEN_Board = Board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR");
    Board FEN2_Board = Board("r1bk3r/p2pBpNp/n4n2/1p1NP2P/6P1/3P4/P1P1K3/q5b1");
};

TEST_F(FENBoardTest, FEN_BoardInitialization) {
    // Example test to check if board initialized correctly from FEN string
    EXPECT_EQ(FEN_Board.ReadBoardLocation(PieceColor::BLACK, PieceID::ROOK_1), A8);
    EXPECT_EQ(FEN_Board.ReadBoardLocation(PieceColor::WHITE, PieceID::PAWN_1), A2);
}

TEST_F(FENBoardTest, FEN2_BoardInitialization) {
    // Example test to check specific pieces from the second FEN string
    EXPECT_EQ(FEN2_Board.ReadBoardLocation(PieceColor::BLACK, PieceID::QUEEN), A1);
    EXPECT_EQ(FEN2_Board.ReadBoardLocation(PieceColor::WHITE, PieceID::BISHOP_1), E7);
}

TEST_F(FENBoardTest, MovePiece_FENBoardUpdateLocation) {
    FEN_Board.MovePiece(PieceColor::BLACK, PieceID::KNIGHT_2, C6);
    EXPECT_EQ(FEN_Board.ReadBoardLocation(PieceColor::BLACK, PieceID::KNIGHT_2), C6);
}