//
// Created by Andrew Fagan on 10/28/24.
//

#include <gtest/gtest.h>
#include "Game/Board/Board.h"

using namespace Game;

class Board_InternalBoardRepresentationTest : public ::testing::Test
{
public:

    Board board = Board();
};

TEST_F(Board_InternalBoardRepresentationTest, ColorRead1)
{
    EXPECT_EQ(board.GetInternalBoard().ReadSquareColor(C8), BLACK);
    EXPECT_EQ(board.GetInternalBoard().ReadSquareColor(B1), WHITE);
    EXPECT_EQ(board.GetInternalBoard().ReadSquareColor(A7), BLACK);
    EXPECT_EQ(board.GetInternalBoard().ReadSquareColor(D1), WHITE);
}

TEST_F(Board_InternalBoardRepresentationTest, ColorRead2)
{
    EXPECT_EQ(board.GetInternalBoard().ReadSquareColor(F7), BLACK);
    EXPECT_EQ(board.GetInternalBoard().ReadSquareColor(G2), WHITE);
    EXPECT_EQ(board.GetInternalBoard().ReadSquareColor(H5), BLACK);
}

TEST_F(Board_InternalBoardRepresentationTest, ColorWrite1)
{
    board.GetInternalBoard().WriteSquareColor(WHITE, F3);
    EXPECT_EQ(board.GetInternalBoard().ReadSquareColor(F3), WHITE);
}

TEST_F(Board_InternalBoardRepresentationTest, ColorWrite2)
{
    board.GetInternalBoard().WriteSquareColor(BLACK, G8);
    EXPECT_EQ(board.GetInternalBoard().ReadSquareColor(G8), BLACK);

    board.GetInternalBoard().WriteSquareColor(WHITE, G8);
    EXPECT_EQ(board.GetInternalBoard().ReadSquareColor(G8), WHITE);
}

TEST_F(Board_InternalBoardRepresentationTest, TypeRead1)
{
    EXPECT_EQ(board.GetInternalBoard().ReadSquareType(F7), PieceType::PAWN);
    EXPECT_EQ(board.GetInternalBoard().ReadSquareType(G2), PieceType::PAWN);
}

TEST_F(Board_InternalBoardRepresentationTest, TypeRead2)
{
    EXPECT_EQ(board.GetInternalBoard().ReadSquareType(E4), PieceType::NONE);
    EXPECT_EQ(board.GetInternalBoard().ReadSquareType(B1), PieceType::KNIGHT);
}

TEST_F(Board_InternalBoardRepresentationTest, TypeWrite1)
{
    board.GetInternalBoard().WriteSquareType(PieceType::PAWN, E4);
    EXPECT_EQ(board.GetInternalBoard().ReadSquareType(E4), PieceType::PAWN);
    EXPECT_EQ(board.GetInternalBoard().ReadSquareType(D4), PieceType::NONE);
    EXPECT_EQ(board.GetInternalBoard().ReadSquareType(F4), PieceType::NONE);
}

TEST_F(Board_InternalBoardRepresentationTest, TypeWrite2)
{
    board.GetInternalBoard().WriteSquareType(PieceType::QUEEN, F3);
    EXPECT_EQ(board.GetInternalBoard().ReadSquareType(F3), PieceType::QUEEN);
    EXPECT_EQ(board.GetInternalBoard().ReadSquareType(E3), PieceType::NONE);
    EXPECT_EQ(board.GetInternalBoard().ReadSquareType(G3), PieceType::NONE);
}

TEST_F(Board_InternalBoardRepresentationTest, TypeWrite3)
{
    board.GetInternalBoard().WriteSquareType(PieceType::ROOK, C3);
    board.GetInternalBoard().WriteSquareType(PieceType::KING, D3);
    board.GetInternalBoard().WriteSquareType(PieceType::QUEEN, E3);
    EXPECT_EQ(board.GetInternalBoard().ReadSquareType(C3), PieceType::ROOK);
    EXPECT_EQ(board.GetInternalBoard().ReadSquareType(D3), PieceType::KING);
    EXPECT_EQ(board.GetInternalBoard().ReadSquareType(E3), PieceType::QUEEN);
}