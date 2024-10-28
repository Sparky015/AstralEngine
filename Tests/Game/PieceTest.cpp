//
// Created by Andrew Fagan on 10/27/24.
//

#include <gtest/gtest.h>
#include "Game/PieceInfo.h"

class PieceTest : public ::testing::Test
{
public:
    Piece piece1 = Piece(ROOK_1, BLACK);
    Piece piece2 = Piece(KING, WHITE);
};

TEST_F(PieceTest, ConstructorAccuracy1)
{
    EXPECT_EQ(piece1.ReadInternalData(), 0b00111000);
}

TEST_F(PieceTest, ConstructorAccuracy2)
{
    EXPECT_EQ(piece2.ReadInternalData(), 0b11011100);
}

TEST_F(PieceTest, ReadAccuracy1)
{
    EXPECT_EQ(piece1.ReadColor(), BLACK);
    EXPECT_EQ(piece1.ReadPieceID(), ROOK_1);
    EXPECT_EQ(piece1.ReadType(), PieceType::ROOK);
}

TEST_F(PieceTest, ReadAccuracy2)
{
    EXPECT_EQ(piece2.ReadColor(), WHITE);
    EXPECT_EQ(piece2.ReadPieceID(), KING);
    EXPECT_EQ(piece2.ReadType(), PieceType::KING);
}

TEST_F(PieceTest, WriteAccuracy1)
{
    piece1.SetColor(WHITE);
    piece1.SetPieceID(QUEEN);
    piece1.SetType(PieceType::QUEEN);

    EXPECT_EQ(piece1.ReadColor(), WHITE);
    EXPECT_EQ(piece1.ReadPieceID(), QUEEN);
    EXPECT_EQ(piece1.ReadType(), PieceType::QUEEN);
}

TEST_F(PieceTest, WriteAccuracy2)
{
    piece2.SetColor(WHITE);
    piece2.SetPieceID(PAWN_5);
    piece2.SetType(PieceType::PAWN);

    EXPECT_EQ(piece2.ReadColor(), WHITE);
    EXPECT_EQ(piece2.ReadPieceID(), PAWN_5);
    EXPECT_EQ(piece2.ReadType(), PieceType::PAWN);
}

TEST_F(PieceTest, WriteAccuracy3)
{
    piece1.SetColor(PieceColor::BLACK);
    piece1.SetPieceID(KNIGHT_1);
    piece1.SetType(PieceType::KNIGHT);

    EXPECT_EQ(piece1.ReadColor(), PieceColor::BLACK);
    EXPECT_EQ(piece1.ReadPieceID(), KNIGHT_1);
    EXPECT_EQ(piece1.ReadType(), PieceType::KNIGHT);
}

TEST_F(PieceTest, WriteAccuracy4)
{
    piece2.SetColor(PieceColor::BLACK);
    piece2.SetPieceID(BISHOP_2);
    piece2.SetType(PieceType::BISHOP);

    EXPECT_EQ(piece2.ReadColor(), PieceColor::BLACK);
    EXPECT_EQ(piece2.ReadPieceID(), BISHOP_2);
    EXPECT_EQ(piece2.ReadType(), PieceType::BISHOP);
}