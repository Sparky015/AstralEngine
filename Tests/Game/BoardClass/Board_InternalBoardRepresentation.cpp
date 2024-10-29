//
// Created by Andrew Fagan on 10/28/24.
//

#include <gtest/gtest.h>
#include "Game/Board.h"

class InternalBoardRepresentationTest : public ::testing::Test
{
public:

    Board board = Board();
};

TEST_F(InternalBoardRepresentationTest, ColorRead1)
{
    EXPECT_EQ(board.GetInternalBoard().ReadColor(C8), BLACK);
    EXPECT_EQ(board.GetInternalBoard().ReadColor(B1), WHITE);
    EXPECT_EQ(board.GetInternalBoard().ReadColor(A7), BLACK);
    EXPECT_EQ(board.GetInternalBoard().ReadColor(D1), WHITE);
}

TEST_F(InternalBoardRepresentationTest, ColorRead2)
{
    EXPECT_EQ(board.GetInternalBoard().ReadColor(F7), BLACK);
    EXPECT_EQ(board.GetInternalBoard().ReadColor(G2), WHITE);
    EXPECT_EQ(board.GetInternalBoard().ReadColor(H5), BLACK);
}

TEST_F(InternalBoardRepresentationTest, ColorWrite1)
{
    board.GetInternalBoard().WriteColor(WHITE, F3);
    EXPECT_EQ(board.GetInternalBoard().ReadColor(F3), WHITE);
}

TEST_F(InternalBoardRepresentationTest, ColorWrite2)
{
    board.GetInternalBoard().WriteColor(BLACK, G8);
    EXPECT_EQ(board.GetInternalBoard().ReadColor(G8), BLACK);

    board.GetInternalBoard().WriteColor(WHITE, G8);
    EXPECT_EQ(board.GetInternalBoard().ReadColor(G8), WHITE);
}

TEST_F(InternalBoardRepresentationTest, TypeRead1)
{
    EXPECT_EQ(board.GetInternalBoard().ReadType(F7), PieceType::PAWN);
    EXPECT_EQ(board.GetInternalBoard().ReadType(G2), PieceType::PAWN);
}

TEST_F(InternalBoardRepresentationTest, TypeRead2)
{
    EXPECT_EQ(board.GetInternalBoard().ReadType(E4), PieceType::NONE);
    EXPECT_EQ(board.GetInternalBoard().ReadType(B1), PieceType::KNIGHT);
}

TEST_F(InternalBoardRepresentationTest, TypeWrite1)
{
    board.GetInternalBoard().WriteType(PieceType::PAWN, E4);
    EXPECT_EQ(board.GetInternalBoard().ReadType(E4), PieceType::PAWN);
    EXPECT_EQ(board.GetInternalBoard().ReadType(D4), PieceType::NONE);
    EXPECT_EQ(board.GetInternalBoard().ReadType(F4), PieceType::NONE);
}

TEST_F(InternalBoardRepresentationTest, TypeWrite2)
{
    board.GetInternalBoard().WriteType(PieceType::QUEEN, F3);
    EXPECT_EQ(board.GetInternalBoard().ReadType(F3), PieceType::QUEEN);
    EXPECT_EQ(board.GetInternalBoard().ReadType(E3), PieceType::NONE);
    EXPECT_EQ(board.GetInternalBoard().ReadType(G3), PieceType::NONE);
}

TEST_F(InternalBoardRepresentationTest, TypeWrite3)
{
    board.GetInternalBoard().WriteType(PieceType::ROOK, C3);
    board.GetInternalBoard().WriteType(PieceType::KING, D3);
    board.GetInternalBoard().WriteType(PieceType::QUEEN, E3);
    EXPECT_EQ(board.GetInternalBoard().ReadType(C3), PieceType::ROOK);
    EXPECT_EQ(board.GetInternalBoard().ReadType(D3), PieceType::KING);
    EXPECT_EQ(board.GetInternalBoard().ReadType(E3), PieceType::QUEEN);
}