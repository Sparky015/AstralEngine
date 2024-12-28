/**
* @file KeycodeConversionsTest.cpp
* @author Andrew Fagan
* @date 12/27/2024
*/

#include <gtest/gtest.h>

#include "Input/KeycodeConversions.h"
#include "GLFW/glfw3.h"
#include "Input/Keycodes.h"

class KeycodeTranslationTest : public ::testing::Test
{
protected:
    void SetUp() override {
        // Any setup if needed
    }
};


namespace Input {

    // Mouse Buttons
    TEST_F(KeycodeTranslationTest, MouseButtons)
    {
        EXPECT_EQ(TranslateGLFWKeycodesToAstral(GLFW_MOUSE_BUTTON_LEFT), KEY_LEFT_CLICK);
        EXPECT_EQ(TranslateGLFWKeycodesToAstral(GLFW_MOUSE_BUTTON_RIGHT), KEY_RIGHT_CLICK);
        EXPECT_EQ(TranslateGLFWKeycodesToAstral(GLFW_MOUSE_BUTTON_MIDDLE), KEY_MIDDLE_CLICK);
    }

    // Alphabetic Keys
    TEST_F(KeycodeTranslationTest, AlphabeticKeys)
    {
        EXPECT_EQ(TranslateGLFWKeycodesToAstral(GLFW_KEY_A), KEY_A);
        EXPECT_EQ(TranslateGLFWKeycodesToAstral(GLFW_KEY_M), KEY_M);
        EXPECT_EQ(TranslateGLFWKeycodesToAstral(GLFW_KEY_Z), KEY_Z);
    }

    // Numeric Keys
    TEST_F(KeycodeTranslationTest, NumericKeys)
    {
        EXPECT_EQ(TranslateGLFWKeycodesToAstral(GLFW_KEY_0), KEY_0);
        EXPECT_EQ(TranslateGLFWKeycodesToAstral(GLFW_KEY_5), KEY_5);
        EXPECT_EQ(TranslateGLFWKeycodesToAstral(GLFW_KEY_9), KEY_9);
    }

    // Function Keys
    TEST_F(KeycodeTranslationTest, FunctionKeys)
    {
        EXPECT_EQ(TranslateGLFWKeycodesToAstral(GLFW_KEY_F1), KEY_F1);
        EXPECT_EQ(TranslateGLFWKeycodesToAstral(GLFW_KEY_F12), KEY_F12);
        EXPECT_EQ(TranslateGLFWKeycodesToAstral(GLFW_KEY_F24), KEY_F24);
    }

    // Special Keys
    TEST_F(KeycodeTranslationTest, SpecialKeys)
    {
        EXPECT_EQ(TranslateGLFWKeycodesToAstral(GLFW_KEY_SPACE), KEY_SPACE);
        EXPECT_EQ(TranslateGLFWKeycodesToAstral(GLFW_KEY_ENTER), KEY_ENTER);
        EXPECT_EQ(TranslateGLFWKeycodesToAstral(GLFW_KEY_TAB), KEY_TAB);
        EXPECT_EQ(TranslateGLFWKeycodesToAstral(GLFW_KEY_ESCAPE), KEY_ESCAPE);
        EXPECT_EQ(TranslateGLFWKeycodesToAstral(GLFW_KEY_BACKSPACE), KEY_BACKSPACE);
    }

    // Arrow Keys
    TEST_F(KeycodeTranslationTest, ArrowKeys)
    {
        EXPECT_EQ(TranslateGLFWKeycodesToAstral(GLFW_KEY_UP), KEY_UP_ARROW);
        EXPECT_EQ(TranslateGLFWKeycodesToAstral(GLFW_KEY_DOWN), KEY_DOWN_ARROW);
        EXPECT_EQ(TranslateGLFWKeycodesToAstral(GLFW_KEY_LEFT), KEY_LEFT_ARROW);
        EXPECT_EQ(TranslateGLFWKeycodesToAstral(GLFW_KEY_RIGHT), KEY_RIGHT_ARROW);
    }

    // Modifier Keys
    TEST_F(KeycodeTranslationTest, ModifierKeys)
    {
        EXPECT_EQ(TranslateGLFWKeycodesToAstral(GLFW_KEY_LEFT_SHIFT), KEY_LEFT_SHIFT);
        EXPECT_EQ(TranslateGLFWKeycodesToAstral(GLFW_KEY_RIGHT_SHIFT), KEY_RIGHT_SHIFT);
        EXPECT_EQ(TranslateGLFWKeycodesToAstral(GLFW_KEY_LEFT_CONTROL), KEY_LEFT_CONTROL);
        EXPECT_EQ(TranslateGLFWKeycodesToAstral(GLFW_KEY_RIGHT_CONTROL), KEY_RIGHT_CONTROL);
        EXPECT_EQ(TranslateGLFWKeycodesToAstral(GLFW_KEY_LEFT_ALT), KEY_LEFT_ALT);
        EXPECT_EQ(TranslateGLFWKeycodesToAstral(GLFW_KEY_RIGHT_ALT), KEY_RIGHT_ALT);
    }

    // Numpad Keys
    TEST_F(KeycodeTranslationTest, NumpadKeys)
    {
        EXPECT_EQ(TranslateGLFWKeycodesToAstral(GLFW_KEY_KP_0), KEY_KP_0);
        EXPECT_EQ(TranslateGLFWKeycodesToAstral(GLFW_KEY_KP_5), KEY_KP_5);
        EXPECT_EQ(TranslateGLFWKeycodesToAstral(GLFW_KEY_KP_9), KEY_KP_9);
        EXPECT_EQ(TranslateGLFWKeycodesToAstral(GLFW_KEY_KP_DECIMAL), KEY_KP_DECIMAL);
        EXPECT_EQ(TranslateGLFWKeycodesToAstral(GLFW_KEY_KP_DIVIDE), KEY_KP_DIVIDE);
        EXPECT_EQ(TranslateGLFWKeycodesToAstral(GLFW_KEY_KP_MULTIPLY), KEY_KP_MULTIPLY);
    }

    // Edge Cases
    TEST_F(KeycodeTranslationTest, EdgeCases)
    {
        EXPECT_EQ(TranslateGLFWKeycodesToAstral(-1), KEY_NONE);

        EXPECT_ANY_THROW(TranslateGLFWKeycodesToAstral(99999));

    }

}