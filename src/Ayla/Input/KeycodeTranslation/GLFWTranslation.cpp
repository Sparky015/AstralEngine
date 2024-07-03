//
// Created by Andrew Fagan on 7/2/24.
//

#include "GLFWTranslation.h"


#include <GLFW/glfw3.h>
#include "Ayla/Input/Keycodes.h"

namespace Ayla::Input::Translation {


    std::unordered_map<int, int> keys = {
            {GLFW_MOUSE_BUTTON_LEFT, AY_KEY_LEFT_CLICK},
            {GLFW_MOUSE_BUTTON_MIDDLE, AY_KEY_MIDDLE_CLICK},
            {GLFW_MOUSE_BUTTON_RIGHT, AY_KEY_RIGHT_CLICK},

            {GLFW_KEY_A, AY_KEY_A},
            {GLFW_KEY_B, AY_KEY_B},
            {GLFW_KEY_C, AY_KEY_C},
            {GLFW_KEY_D, AY_KEY_D},
            {GLFW_KEY_E, AY_KEY_E},
            {GLFW_KEY_F, AY_KEY_F},
            {GLFW_KEY_G, AY_KEY_G},
            {GLFW_KEY_H, AY_KEY_H},
            {GLFW_KEY_I, AY_KEY_I},
            {GLFW_KEY_J, AY_KEY_J},
            {GLFW_KEY_K, AY_KEY_K},
            {GLFW_KEY_L, AY_KEY_L},
            {GLFW_KEY_M, AY_KEY_M},
            {GLFW_KEY_N, AY_KEY_N},
            {GLFW_KEY_O, AY_KEY_O},
            {GLFW_KEY_P, AY_KEY_P},
            {GLFW_KEY_Q, AY_KEY_Q},
            {GLFW_KEY_R, AY_KEY_R},
            {GLFW_KEY_S, AY_KEY_S},
            {GLFW_KEY_T, AY_KEY_T},
            {GLFW_KEY_U, AY_KEY_U},
            {GLFW_KEY_V, AY_KEY_V},
            {GLFW_KEY_W, AY_KEY_W},
            {GLFW_KEY_X, AY_KEY_X},
            {GLFW_KEY_Y, AY_KEY_Y},
            {GLFW_KEY_Z, AY_KEY_Z},

            // number keys
            {GLFW_KEY_0, AY_KEY_0},
            {GLFW_KEY_1, AY_KEY_1},
            {GLFW_KEY_2, AY_KEY_2},
            {GLFW_KEY_3, AY_KEY_3},
            {GLFW_KEY_4, AY_KEY_4},
            {GLFW_KEY_5, AY_KEY_5},
            {GLFW_KEY_6, AY_KEY_6},
            {GLFW_KEY_7, AY_KEY_7},
            {GLFW_KEY_8, AY_KEY_8},
            {GLFW_KEY_9, AY_KEY_9},

            // function keys
            {GLFW_KEY_F1, AY_KEY_F1},
            {GLFW_KEY_F2, AY_KEY_F2},
            {GLFW_KEY_F3, AY_KEY_F3},
            {GLFW_KEY_F4, AY_KEY_F4},
            {GLFW_KEY_F5, AY_KEY_F5},
            {GLFW_KEY_F6, AY_KEY_F6},
            {GLFW_KEY_F7, AY_KEY_F7},
            {GLFW_KEY_F8, AY_KEY_F8},
            {GLFW_KEY_F9, AY_KEY_F9},
            {GLFW_KEY_F10, AY_KEY_F10},
            {GLFW_KEY_F11, AY_KEY_F11},
            {GLFW_KEY_F12, AY_KEY_F12},


            { GLFW_KEY_ESCAPE, AY_KEY_ESCAPE },
            { GLFW_KEY_ENTER, AY_KEY_ENTER },
            { GLFW_KEY_TAB, AY_KEY_TAB },
            { GLFW_KEY_BACKSPACE, AY_KEY_BACKSPACE },
            {GLFW_KEY_SPACE, AY_KEY_SPACE},
            { GLFW_KEY_INSERT, AY_KEY_INSERT },
            { GLFW_KEY_DELETE, AY_KEY_DELETE },
            { GLFW_KEY_RIGHT, AY_KEY_RIGHT },
            { GLFW_KEY_LEFT, AY_KEY_LEFT },
            { GLFW_KEY_DOWN, AY_KEY_DOWN },
            { GLFW_KEY_UP, AY_KEY_UP },
            { GLFW_KEY_PAGE_UP, AY_KEY_PAGE_UP },
            { GLFW_KEY_PAGE_DOWN, AY_KEY_PAGE_DOWN },
            { GLFW_KEY_HOME, AY_KEY_HOME },
            { GLFW_KEY_END, AY_KEY_END },
            { GLFW_KEY_CAPS_LOCK, AY_KEY_CAPS_LOCK },
            { GLFW_KEY_SCROLL_LOCK, AY_KEY_SCROLL_LOCK },
            { GLFW_KEY_NUM_LOCK, AY_KEY_NUM_LOCK },
            { GLFW_KEY_PRINT_SCREEN, AY_KEY_PRINT_SCREEN },
            { GLFW_KEY_PAUSE, AY_KEY_PAUSE },
            { GLFW_KEY_LEFT_SHIFT, AY_KEY_LEFT_SHIFT },
            { GLFW_KEY_LEFT_CONTROL, AY_KEY_LEFT_CONTROL },
            { GLFW_KEY_LEFT_ALT, AY_KEY_LEFT_ALT },
            { GLFW_KEY_LEFT_SUPER, AY_KEY_LEFT_SUPER },
            { GLFW_KEY_RIGHT_SHIFT, AY_KEY_RIGHT_SHIFT },
            { GLFW_KEY_RIGHT_CONTROL, AY_KEY_RIGHT_CONTROL },
            { GLFW_KEY_RIGHT_ALT, AY_KEY_RIGHT_ALT },
            { GLFW_KEY_RIGHT_SUPER, AY_KEY_RIGHT_SUPER}
    };

    int translateGLFWKeycodesToAyla(int GLFWKeycode){
        AY_ASSERT_SS(keys.find(GLFWKeycode) != keys.end(),  "Input/KeycodeTranslation/GLFWTranslation.cpp: Keycode '" << GLFWKeycode << "' is not in the GLFW keycode translation hashmap!"); // Sanity Check to see if the keycode is not in the translation hashmap
        return keys[GLFWKeycode];
    }
}