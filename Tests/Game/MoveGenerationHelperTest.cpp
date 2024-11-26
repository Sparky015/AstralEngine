//
// Created by Andrew Fagan on 11/25/24.
//

#include <gtest/gtest.h>
#include "Game/Pieces/ChessPiece.h"
#include "Game/MoveGenerationHelpers.h"

namespace Game {

    class MoveGenerationHelperTest : public ::testing::Test
    {
    public:

    };

    TEST_F(MoveGenerationHelperTest, BoundsCheckTest)
    {
        EXPECT_EQ(IsWithinBounds(A8, UP), false);
        EXPECT_EQ(IsWithinBounds(A7, UP_LEFT), false);
        EXPECT_EQ(IsWithinBounds(A7, UP), true);
        EXPECT_EQ(IsWithinBounds(G6, UP_RIGHT), true);
        EXPECT_EQ(IsWithinBounds(G6, RIGHT), true);
        EXPECT_EQ(IsWithinBounds(G2, RIGHT_UP), false);
        EXPECT_EQ(IsWithinBounds(G7, RIGHT_UP), false);
        EXPECT_EQ(IsWithinBounds(A4, LEFT), false);
        EXPECT_EQ(IsWithinBounds(A4, LEFT_DOWN), false);
        EXPECT_EQ(IsWithinBounds(H1, RIGHT), false);
        EXPECT_EQ(IsWithinBounds(H2, RIGHT), false);
        EXPECT_EQ(IsWithinBounds(H2, RIGHT_UP), false);
        EXPECT_EQ(IsWithinBounds(B6, LEFT_UP), false);
        EXPECT_EQ(IsWithinBounds(A2, DOWN_LEFT), false);
        EXPECT_EQ(IsWithinBounds(G4, UP + RIGHT), true);
    }

}