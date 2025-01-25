//
// Created by Andrew Fagan on 1/23/25.
//

#include "Core/Containers/Array.h"
#include <gtest/gtest.h>

class ArrayTest : public ::testing::Test
{
public:
    Astral::Array<int, 5> m_DefaultArray;
};

TEST_F(ArrayTest, DefaultConstructor_FillsDefaultInstances)
{
    EXPECT_EQ(m_DefaultArray.Index(0), 0);
    EXPECT_EQ(m_DefaultArray.Index(2), 0);
    EXPECT_EQ(m_DefaultArray.Index(4), 0);

    Astral::Array<int, 5> expected{0, 0, 0, 0, 0};
    EXPECT_EQ(m_DefaultArray, expected);
}

TEST_F(ArrayTest, SingleElementConstructor_FillsArrayWithSingleGivenInstance)
{
    Astral::Array<int, 5> array = Astral::Array<int, 5>(20);
    EXPECT_EQ(array.Index(0), 20);
    EXPECT_EQ(array.Index(2), 20);
    EXPECT_EQ(array.Index(4), 20);

    Astral::Array<int, 5> expected{20, 20, 20, 20, 20};
    EXPECT_EQ(array, expected);
}

TEST_F(ArrayTest, InitializerListConstructor_FillsArrayWithGivenValues)
{
    Astral::Array<int, 5> array = Astral::Array<int, 5>(20);
    EXPECT_EQ(array.Index(0), 20);
    EXPECT_EQ(array.Index(2), 20);
    EXPECT_EQ(array.Index(4), 20);

    Astral::Array<int, 5> expected{20, 20, 20, 20, 20};
    EXPECT_EQ(array, expected);
}

TEST_F(ArrayTest, InitializerListOperator_FillsArrayWithGivenValues)
{
    Astral::Array<int, 5> array = Astral::Array<int, 5>(20);
    EXPECT_EQ(array.Index(0), 20);
    EXPECT_EQ(array.Index(2), 20);
    EXPECT_EQ(array.Index(4), 20);

    array = {30, 50, 501, 10, 70};
    EXPECT_EQ(array.Index(0), 30);
    EXPECT_EQ(array.Index(2), 501);
    EXPECT_EQ(array.Index(4), 70);
}

TEST_F(ArrayTest, Fill_PopulatesArrayWithSingleInstance)
{
    Astral::Array<int, 5> array = Astral::Array<int, 5>({23, 1, 5, 6, 8});
    array.Fill(2);
    EXPECT_EQ(array.Index(0), 2);
    EXPECT_EQ(array.Index(2), 2);
    EXPECT_EQ(array.Index(4), 2);

    Astral::Array<int, 5> expected{23, 1, 5, 6, 8};
    EXPECT_NE(array, expected);
    expected.Fill(2);
    EXPECT_EQ(array, expected);
}

TEST_F(ArrayTest, Contains_CorrectlyIdentifiesContainedElements)
{
    Astral::Array<int, 5> array = Astral::Array<int, 5>({23, 1, 5, 6, 8});

    EXPECT_EQ(array.Contains(23), true);
    EXPECT_EQ(array.Contains(1), true);
    EXPECT_EQ(array.Contains(5), true);
    EXPECT_EQ(array.Contains(6), true);
    EXPECT_EQ(array.Contains(8), true);

    EXPECT_EQ(array.Contains(0), false);
    EXPECT_EQ(array.Contains(-1), false);
    EXPECT_EQ(array.Contains(7), false);
    EXPECT_EQ(array.Contains(9), false);
    EXPECT_EQ(array.Contains(-5), false);
}

TEST_F(ArrayTest, Iterators_CorrectlyIteratesThroughContainer)
{
    Astral::Array<int, 8> array = {2, 3, 4, 5, 6, 78, 8, 0};

    size_t forloopIndex = 0;
    for (int i : array)
    {
        EXPECT_EQ(array[forloopIndex], i);
        forloopIndex++;
    }
}