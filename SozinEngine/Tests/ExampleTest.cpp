//
// Created by Andrew Fagan on 10/21/24.
//

#include <gtest/gtest.h>
#include "pch.h"

// Test for basic integer comparison
TEST(BasicTest, IntegerComparison) {
    EXPECT_EQ(1, 1);      // Test that 1 is equal to 1
    EXPECT_NE(1, 2);      // Test that 1 is not equal to 2
    EXPECT_LT(1, 2);      // Test that 1 is less than 2
    EXPECT_LE(1, 1);      // Test that 1 is less than or equal to 1
    EXPECT_GT(2, 1);      // Test that 2 is greater than 1
    EXPECT_GE(2, 2);      // Test that 2 is greater than or equal to 2
}

// Test for basic floating-point comparison
TEST(BasicTest, FloatingPointComparison) {
    EXPECT_FLOAT_EQ(1.0f, 1.0f);     // Test that 1.0 is equal to 1.0 (float)
    EXPECT_DOUBLE_EQ(1.0, 1.0);      // Test that 1.0 is equal to 1.0 (double)
    EXPECT_NEAR(1.0, 1.001, 0.01);   // Test that 1.0 is near 1.001 within delta 0.01
}

// Test for basic string comparison
TEST(BasicTest, StringComparison) {
    EXPECT_STREQ("hello", "hello");        // Test that two C-strings are equal
    EXPECT_STRNE("hello", "world");        // Test that two C-strings are not equal
    EXPECT_EQ(std::string("hello"), "hello");  // Test that std::string and C-string are equal
    EXPECT_NE(std::string("hello"), "world");  // Test that std::string and C-string are not equal
}

// Test for basic boolean value
TEST(BasicTest, BooleanValue) {
    EXPECT_TRUE(true);     // Test that the value is true
    EXPECT_FALSE(false);   // Test that the value is false
}




void validateAge(int age)
{
    if (age < 0 || age > 150) {
        throw std::out_of_range("Invalid age");
    }
}


TEST(ExceptionTest, ValidateAge) {
    // Test that validateAge throws std::out_of_range for invalid ages
    EXPECT_THROW(validateAge(-1), std::out_of_range);  // Age below 0
    EXPECT_THROW(validateAge(151), std::out_of_range); // Age above 150

    // Test that validateAge does not throw for valid ages
    EXPECT_NO_THROW(validateAge(25));   // Valid age
    EXPECT_NO_THROW(validateAge(0));    // Boundary case
    EXPECT_NO_THROW(validateAge(150));  // Boundary case
}



/*
 * Writing Tests:
 * 2-3 for functionality (or more until the whole function has coverage)
 * 1 for each error status or exception that can be thrown
 * 1 for each edge case of inputs
 *
 * */