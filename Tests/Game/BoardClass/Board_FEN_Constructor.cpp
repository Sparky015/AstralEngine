//
// Created by Andrew Fagan on 10/23/24.
//

#include <gtest/gtest.h>
#include "Game/Board.h"

using namespace Game;

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
    EXPECT_EQ(FEN_Board.ReadPieceLocation(PieceColor::WHITE, PieceID::ROOK_2), H1);
    EXPECT_EQ(FEN_Board.ReadPieceLocation(PieceColor::BLACK, PieceID::PAWN_6), F7);
}

TEST_F(Board_FEN_ConstructorTest, FEN_Constructor_CheckingLocation2)
{
// Example test to check if board initialized correctly from FEN string
    EXPECT_EQ(FEN_Board.ReadPieceLocation(PieceColor::BLACK, PieceID::KING), E8);
    EXPECT_EQ(FEN_Board.ReadPieceLocation(PieceColor::BLACK, PieceID::BISHOP_2), F8);
    EXPECT_EQ(FEN_Board.ReadPieceLocation(PieceColor::WHITE, PieceID::KING), E1);
    EXPECT_EQ(FEN_Board.ReadPieceLocation(PieceColor::WHITE, PieceID::PAWN_3), C2);
}

TEST_F(Board_FEN_ConstructorTest, FEN_Constructor_CheckingEmptyLocations1)
{
    EXPECT_EQ(FEN_Board.ReadSquareType(D4), PieceType::NONE);
    EXPECT_EQ(FEN_Board.ReadSquareType(E4), PieceType::NONE);
    EXPECT_EQ(FEN_Board.ReadSquareType(F3), PieceType::NONE);
    EXPECT_EQ(FEN_Board.ReadSquareType(A3), PieceType::NONE);
    EXPECT_EQ(FEN_Board.ReadSquareType(H6), PieceType::NONE);
    EXPECT_EQ(FEN_Board.ReadSquareType(C5), PieceType::NONE);
}

TEST_F(Board_FEN_ConstructorTest, FEN2_Constructor_CheckingLocation1)
{
    EXPECT_EQ(FEN2_Board.ReadPieceLocation(PieceColor::BLACK, PieceID::QUEEN), A1);
    EXPECT_EQ(FEN2_Board.ReadPieceLocation(PieceColor::WHITE, PieceID::BISHOP_1), E7);
}

TEST_F(Board_FEN_ConstructorTest, FEN2_Constructor_CheckingLocation2)
{
    EXPECT_EQ(FEN2_Board.ReadPieceLocation(PieceColor::WHITE, PieceID::PAWN_3), G4);
    EXPECT_EQ(FEN2_Board.ReadPieceLocation(PieceColor::BLACK, PieceID::PAWN_1), A7);
}

TEST_F(Board_FEN_ConstructorTest, FEN2_Constructor_CheckingEmptyLocation1)
{
    EXPECT_EQ(FEN2_Board.ReadSquareType(G8), PieceType::NONE);
    EXPECT_EQ(FEN2_Board.ReadSquareType(B7), PieceType::NONE);
}

TEST_F(Board_FEN_ConstructorTest, FEN2_Constructor_CheckingEmptyLocation2)
{
    EXPECT_EQ(FEN2_Board.ReadSquareType(D4), PieceType::NONE);
    EXPECT_EQ(FEN2_Board.ReadSquareType(E4), PieceType::NONE);
}

TEST_F(Board_FEN_ConstructorTest, FEN2_Constructor_CheckingEmptyLocation3)
{
    EXPECT_EQ(FEN2_Board.ReadSquareType(A3), PieceType::NONE);
    EXPECT_EQ(FEN2_Board.ReadSquareType(B1), PieceType::NONE);
}

