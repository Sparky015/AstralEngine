//
// Created by Andrew Fagan on 4/6/24.
//

#include <gtest/gtest.h>

// Demonstrate some basic assertions.
TEST(HelloTest, BasicAssertions) {
// Expect two strings not to be equal.
EXPECT_STRNE("hello", "world");
// Expect equality.
EXPECT_EQ(7 * 6, 42);
}


// Test the equality of two numeric values.
TEST(EqualityTest, NumericValues) {
EXPECT_EQ(3 * 4, 12);
}

// Test the inequality of two numeric values.
TEST(InequalityTest, NumericValues) {
EXPECT_NE(5 * 2, 11);
}

// Test the greater-than condition.
TEST(GreaterThanTest, NumericValues) {
EXPECT_GT(5 * 3, 10);
}

// Test the less-than condition.
TEST(LessThanTest, NumericValues) {
EXPECT_LT(2, 5);
}

// Test the greater-than or equal-to condition.
TEST(GreaterThanOrEqualTest, NumericValues) {
EXPECT_GE(3 + 2, 5);
}

// Test the less-than or equal-to condition.
TEST(LessThanOrEqualTest, NumericValues) {
EXPECT_LE(2, 3);
}

// Test a condition with a boolean value
TEST(BooleanConditionTest, BooleanValues) {
EXPECT_TRUE(3 > 2);
EXPECT_FALSE(2 > 3);
}