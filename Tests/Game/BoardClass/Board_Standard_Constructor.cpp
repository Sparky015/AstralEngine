/**
* @file Board_Standard_Constructor.cpp
* @author Andrew Fagan
* @date 10/24/2024
*/

#include <gtest/gtest.h>
#include "Game/Board/Board.h"

using namespace Game;

class Board_StandardConstructorTest : public ::testing::Test
{
public:
    Board StandardBoard = Board();
};

TEST_F(Board_StandardConstructorTest, Standard_Constructor_CheckingLocation1)
{
    EXPECT_EQ(StandardBoard.ReadPieceLocation(PieceColor::BLACK, PieceID::PIECE_9), A8);
    EXPECT_EQ(StandardBoard.ReadPieceLocation(PieceColor::WHITE, PieceID::PIECE_1), A2);
}

TEST_F(Board_StandardConstructorTest, Standard_Constructor_CheckingLocation2)
{
    EXPECT_EQ(StandardBoard.ReadPieceLocation(PieceColor::BLACK, PieceID::PIECE_4), D7);
    EXPECT_EQ(StandardBoard.ReadPieceLocation(PieceColor::WHITE, PieceID::PIECE_14), F1);
}

TEST_F(Board_StandardConstructorTest, Standard_Constructor_CheckingLocation3)
{
    EXPECT_EQ(StandardBoard.ReadPieceLocation(PieceColor::BLACK, PieceID::PIECE_13), E8);
    EXPECT_EQ(StandardBoard.ReadPieceLocation(PieceColor::WHITE, PieceID::PIECE_13), E1);
}