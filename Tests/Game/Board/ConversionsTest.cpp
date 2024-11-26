//
// Created by Andrew Fagan on 11/24/24.
//

#include <gtest/gtest.h>

#include "Game/Board/Conversions.h"

namespace Game {

    TEST(ConversionsTest, ChessNotationToInt)
    {
        EXPECT_EQ(ConvertChessNotationToInt("B2"), 49);
        EXPECT_EQ(ConvertChessNotationToInt("A8"), 0);
        EXPECT_EQ(ConvertChessNotationToInt("C4"), 34);
        EXPECT_EQ(ConvertChessNotationToInt("E4"), 36);
        EXPECT_EQ(ConvertChessNotationToInt("H3"), 47);
    }

    TEST(ConversionsTest, ConvertIntToChessNotation)
    {
        EXPECT_EQ(ConvertIntToChessNotation(49), "B2");
        EXPECT_EQ(ConvertIntToChessNotation(0), "A8");
        EXPECT_EQ(ConvertIntToChessNotation(34), "C4");
        EXPECT_EQ(ConvertIntToChessNotation(36), "E4");
        EXPECT_EQ(ConvertIntToChessNotation(47), "H3");
        EXPECT_EQ(ConvertIntToChessNotation(17), "B6");
        EXPECT_EQ(ConvertIntToChessNotation(25), "B5");
        EXPECT_EQ(ConvertIntToChessNotation(54), "G2");
    }

}