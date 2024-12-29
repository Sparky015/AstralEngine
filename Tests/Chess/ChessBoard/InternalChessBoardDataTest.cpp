/**
* @file InternalChessBoardDataTest.cpp
* @author Andrew Fagan
* @date 12/26/2024
*/

#include <gtest/gtest-spi.h>
#include <gtest/gtest.h>
#include "pch.h"

class InternalChessBoardDataTest : public::testing::Test
{
public:

};

TEST_F(InternalChessBoardDataTest, FirstTestName)
{
    EXPECT_ANY_THROW(ASSERT( 1 == 0, "test"));
}

TEST_F(InternalChessBoardDataTest, SecondTest)
{
    EXPECT_EQ(1,1);
}
