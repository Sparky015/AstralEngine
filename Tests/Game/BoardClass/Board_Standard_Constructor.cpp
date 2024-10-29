/**
* @file Board_Standard_Constructor.cpp
* @author Andrew Fagan
* @date 10/24/2024
*/

#include <gtest/gtest.h>
#include "Game/Board.h"

class Board_StandardConstructorTest : public ::testing::Test
{
public:
    Board StandardBoard = Board();
};

TEST_F(Board_StandardConstructorTest, Standard_Constructor_CheckingLocation1)
{
EXPECT_EQ(StandardBoard.ReadPieceLocation(PieceColor::BLACK, PieceID::ROOK_1), A8);
EXPECT_EQ(StandardBoard.ReadPieceLocation(PieceColor::WHITE, PieceID::PAWN_1), A2);
}

TEST_F(Board_StandardConstructorTest, Standard_Constructor_CheckingLocation2)
{
    EXPECT_EQ(StandardBoard.ReadPieceLocation(PieceColor::BLACK, PieceID::PAWN_4), D7);
    EXPECT_EQ((uint8) StandardBoard.ReadPieceLocation(PieceColor::WHITE, PieceID::BISHOP_2), F1);
}

TEST_F(Board_StandardConstructorTest, Standard_Constructor_CheckingLocation3)
{
    EXPECT_EQ(StandardBoard.ReadPieceLocation(PieceColor::BLACK, PieceID::KING), E8);
    EXPECT_EQ(StandardBoard.ReadPieceLocation(PieceColor::WHITE, PieceID::KING), E1);
}