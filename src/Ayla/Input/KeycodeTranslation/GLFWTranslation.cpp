//
// Created by Andrew Fagan on 7/2/24.
//

#include "GLFWTranslation.h"


#include <GLFW/glfw3.h>
#include <imgui.h>

#include "Ayla/Input/Keycodes.h"

namespace Ayla::Input::Keycodes::Translation {



    int TranslateGlfwKeycodesToAyla(int glfwKeycode)
    {
        switch (glfwKeycode)
        {
            case GLFW_MOUSE_BUTTON_LEFT: return AY_KEY_LEFT_CLICK;
            case GLFW_MOUSE_BUTTON_RIGHT: return AY_KEY_RIGHT_CLICK;
            case GLFW_MOUSE_BUTTON_MIDDLE: return AY_KEY_MIDDLE_CLICK;

            case GLFW_KEY_A: return AY_KEY_A;
            case GLFW_KEY_B: return AY_KEY_B;
            case GLFW_KEY_C: return AY_KEY_C;
            case GLFW_KEY_D: return AY_KEY_D;
            case GLFW_KEY_E: return AY_KEY_E;
            case GLFW_KEY_F: return AY_KEY_F;
            case GLFW_KEY_G: return AY_KEY_G;
            case GLFW_KEY_H: return AY_KEY_H;
            case GLFW_KEY_I: return AY_KEY_I;
            case GLFW_KEY_J: return AY_KEY_J;
            case GLFW_KEY_K: return AY_KEY_K;
            case GLFW_KEY_L: return AY_KEY_L;
            case GLFW_KEY_M: return AY_KEY_M;
            case GLFW_KEY_N: return AY_KEY_N;
            case GLFW_KEY_O: return AY_KEY_O;
            case GLFW_KEY_P: return AY_KEY_P;
            case GLFW_KEY_Q: return AY_KEY_Q;
            case GLFW_KEY_R: return AY_KEY_R;
            case GLFW_KEY_S: return AY_KEY_S;
            case GLFW_KEY_T: return AY_KEY_T;
            case GLFW_KEY_U: return AY_KEY_U;
            case GLFW_KEY_V: return AY_KEY_V;
            case GLFW_KEY_W: return AY_KEY_W;
            case GLFW_KEY_X: return AY_KEY_X;
            case GLFW_KEY_Y: return AY_KEY_Y;
            case GLFW_KEY_Z: return AY_KEY_Z;

            case GLFW_KEY_0: return AY_KEY_0;
            case GLFW_KEY_1: return AY_KEY_1;
            case GLFW_KEY_2: return AY_KEY_2;
            case GLFW_KEY_3: return AY_KEY_3;
            case GLFW_KEY_4: return AY_KEY_4;
            case GLFW_KEY_5: return AY_KEY_5;
            case GLFW_KEY_6: return AY_KEY_6;
            case GLFW_KEY_7: return AY_KEY_7;
            case GLFW_KEY_8: return AY_KEY_8;
            case GLFW_KEY_9: return AY_KEY_9;

            case GLFW_KEY_SPACE: return AY_KEY_SPACE;
            case GLFW_KEY_ESCAPE: return AY_KEY_ESCAPE;
            case GLFW_KEY_ENTER: return AY_KEY_ENTER;
            case GLFW_KEY_TAB: return AY_KEY_TAB;
            case GLFW_KEY_BACKSPACE: return AY_KEY_BACKSPACE;
            case GLFW_KEY_INSERT: return AY_KEY_INSERT;
            case GLFW_KEY_DELETE: return AY_KEY_DELETE;
            case GLFW_KEY_RIGHT: return AY_KEY_RIGHT_ARROW;
            case GLFW_KEY_LEFT: return AY_KEY_LEFT_ARROW;
            case GLFW_KEY_DOWN: return AY_KEY_DOWN_ARROW;
            case GLFW_KEY_UP: return AY_KEY_UP_ARROW;
            case GLFW_KEY_PAGE_UP: return AY_KEY_PAGE_UP;
            case GLFW_KEY_PAGE_DOWN: return AY_KEY_PAGE_DOWN;
            case GLFW_KEY_HOME: return AY_KEY_HOME;
            case GLFW_KEY_END: return AY_KEY_END;
            case GLFW_KEY_CAPS_LOCK: return AY_KEY_CAPS_LOCK;
            case GLFW_KEY_SCROLL_LOCK: return AY_KEY_SCROLL_LOCK;
            case GLFW_KEY_NUM_LOCK: return AY_KEY_NUM_LOCK;
            case GLFW_KEY_PRINT_SCREEN: return AY_KEY_PRINT_SCREEN;
            case GLFW_KEY_PAUSE: return AY_KEY_PAUSE;
            case GLFW_KEY_APOSTROPHE: return AY_KEY_APOSTROPHE;
            case GLFW_KEY_COMMA: return AY_KEY_COMMA;
            case GLFW_KEY_MINUS: return AY_KEY_MINUS;
            case GLFW_KEY_EQUAL: return AY_KEY_EQUAL;
            case GLFW_KEY_BACKSLASH: return AY_KEY_BACKSLASH;
            case GLFW_KEY_SEMICOLON: return AY_KEY_SEMICOLON;
            case GLFW_KEY_SLASH: return AY_KEY_SLASH;
            case GLFW_KEY_LEFT_BRACKET: return AY_KEY_LEFT_BRACKET;
            case GLFW_KEY_RIGHT_BRACKET: return AY_KEY_RIGHT_BRACKET;
            case GLFW_KEY_GRAVE_ACCENT: return AY_KEY_GRAVE_ACCENT;
            case GLFW_KEY_PERIOD: return AY_KEY_PERIOD;

            case GLFW_KEY_KP_0: return AY_KEY_KP_0;
            case GLFW_KEY_KP_1: return AY_KEY_KP_1;
            case GLFW_KEY_KP_2: return AY_KEY_KP_2;
            case GLFW_KEY_KP_3: return AY_KEY_KP_3;
            case GLFW_KEY_KP_4: return AY_KEY_KP_4;
            case GLFW_KEY_KP_5: return AY_KEY_KP_5;
            case GLFW_KEY_KP_6: return AY_KEY_KP_6;
            case GLFW_KEY_KP_7: return AY_KEY_KP_7;
            case GLFW_KEY_KP_8: return AY_KEY_KP_8;
            case GLFW_KEY_KP_9: return AY_KEY_KP_9;
            case GLFW_KEY_KP_DECIMAL: return AY_KEY_KP_DECIMAL;
            case GLFW_KEY_KP_DIVIDE: return AY_KEY_KP_DIVIDE;
            case GLFW_KEY_KP_MULTIPLY: return AY_KEY_KP_MULTIPLY;
            case GLFW_KEY_KP_SUBTRACT: return AY_KEY_KP_SUBTRACT;
            case GLFW_KEY_KP_ADD: return AY_KEY_KP_ADD;
            case GLFW_KEY_KP_ENTER: return AY_KEY_KP_ENTER;
            case GLFW_KEY_KP_EQUAL: return AY_KEY_KP_EQUAL;

            case GLFW_KEY_F1: return AY_KEY_F1;
            case GLFW_KEY_F2: return AY_KEY_F2;
            case GLFW_KEY_F3: return AY_KEY_F3;
            case GLFW_KEY_F4: return AY_KEY_F4;
            case GLFW_KEY_F5: return AY_KEY_F5;
            case GLFW_KEY_F6: return AY_KEY_F6;
            case GLFW_KEY_F7: return AY_KEY_F7;
            case GLFW_KEY_F8: return AY_KEY_F8;
            case GLFW_KEY_F9: return AY_KEY_F9;
            case GLFW_KEY_F10: return AY_KEY_F10;
            case GLFW_KEY_F11: return AY_KEY_F11;
            case GLFW_KEY_F12: return AY_KEY_F12;
            case GLFW_KEY_F13: return AY_KEY_F13;
            case GLFW_KEY_F14: return AY_KEY_F14;
            case GLFW_KEY_F15: return AY_KEY_F15;
            case GLFW_KEY_F16: return AY_KEY_F16;
            case GLFW_KEY_F17: return AY_KEY_F17;
            case GLFW_KEY_F18: return AY_KEY_F18;
            case GLFW_KEY_F19: return AY_KEY_F19;
            case GLFW_KEY_F20: return AY_KEY_F20;
            case GLFW_KEY_F21: return AY_KEY_F21;
            case GLFW_KEY_F22: return AY_KEY_F22;
            case GLFW_KEY_F23: return AY_KEY_F23;
            case GLFW_KEY_F24: return AY_KEY_F24;


            default:
                // Sanity Check to see if the keycode is not in the translation hashmap;
                AY_ERROR("Input/KeycodeTranslation/GLFWTranslation.cpp: Keycode '" << glfwKeycode << "' is not in the GLFW keycode translation hashmap!");
                return AY_KEY_NONE;
        }
    }

    int translateGLFWKeycodesToImGui(int GLFWKeycode){
        switch (GLFWKeycode) {
            case GLFW_KEY_TAB: return ImGuiKey_Tab;
            case GLFW_KEY_LEFT: return ImGuiKey_LeftArrow;
            case GLFW_KEY_RIGHT: return ImGuiKey_RightArrow;
            case GLFW_KEY_UP: return ImGuiKey_UpArrow;
            case GLFW_KEY_DOWN: return ImGuiKey_DownArrow;
            case GLFW_KEY_PAGE_UP: return ImGuiKey_PageUp;
            case GLFW_KEY_PAGE_DOWN: return ImGuiKey_PageDown;
            case GLFW_KEY_HOME: return ImGuiKey_Home;
            case GLFW_KEY_END: return ImGuiKey_End;
            case GLFW_KEY_INSERT: return ImGuiKey_Insert;
            case GLFW_KEY_DELETE: return ImGuiKey_Delete;
            case GLFW_KEY_BACKSPACE: return ImGuiKey_Backspace;
            case GLFW_KEY_SPACE: return ImGuiKey_Space;
            case GLFW_KEY_ENTER: return ImGuiKey_Enter;
            case GLFW_KEY_ESCAPE: return ImGuiKey_Escape;
            case GLFW_KEY_APOSTROPHE: return ImGuiKey_Apostrophe;
            case GLFW_KEY_COMMA: return ImGuiKey_Comma;
            case GLFW_KEY_MINUS: return ImGuiKey_Minus;
            case GLFW_KEY_PERIOD: return ImGuiKey_Period;
            case GLFW_KEY_SLASH: return ImGuiKey_Slash;
            case GLFW_KEY_SEMICOLON: return ImGuiKey_Semicolon;
            case GLFW_KEY_EQUAL: return ImGuiKey_Equal;
            case GLFW_KEY_LEFT_BRACKET: return ImGuiKey_LeftBracket;
            case GLFW_KEY_BACKSLASH: return ImGuiKey_Backslash;
            case GLFW_KEY_RIGHT_BRACKET: return ImGuiKey_RightBracket;
            case GLFW_KEY_GRAVE_ACCENT: return ImGuiKey_GraveAccent;
            case GLFW_KEY_CAPS_LOCK: return ImGuiKey_CapsLock;
            case GLFW_KEY_SCROLL_LOCK: return ImGuiKey_ScrollLock;
            case GLFW_KEY_NUM_LOCK: return ImGuiKey_NumLock;
            case GLFW_KEY_PRINT_SCREEN: return ImGuiKey_PrintScreen;
            case GLFW_KEY_PAUSE: return ImGuiKey_Pause;
            case GLFW_KEY_KP_0: return ImGuiKey_Keypad0;
            case GLFW_KEY_KP_1: return ImGuiKey_Keypad1;
            case GLFW_KEY_KP_2: return ImGuiKey_Keypad2;
            case GLFW_KEY_KP_3: return ImGuiKey_Keypad3;
            case GLFW_KEY_KP_4: return ImGuiKey_Keypad4;
            case GLFW_KEY_KP_5: return ImGuiKey_Keypad5;
            case GLFW_KEY_KP_6: return ImGuiKey_Keypad6;
            case GLFW_KEY_KP_7: return ImGuiKey_Keypad7;
            case GLFW_KEY_KP_8: return ImGuiKey_Keypad8;
            case GLFW_KEY_KP_9: return ImGuiKey_Keypad9;
            case GLFW_KEY_KP_DECIMAL: return ImGuiKey_KeypadDecimal;
            case GLFW_KEY_KP_DIVIDE: return ImGuiKey_KeypadDivide;
            case GLFW_KEY_KP_MULTIPLY: return ImGuiKey_KeypadMultiply;
            case GLFW_KEY_KP_SUBTRACT: return ImGuiKey_KeypadSubtract;
            case GLFW_KEY_KP_ADD: return ImGuiKey_KeypadAdd;
            case GLFW_KEY_KP_ENTER: return ImGuiKey_KeypadEnter;
            case GLFW_KEY_KP_EQUAL: return ImGuiKey_KeypadEqual;
            case GLFW_KEY_LEFT_SHIFT: return ImGuiKey_LeftShift;
            case GLFW_KEY_LEFT_CONTROL: return ImGuiKey_LeftCtrl;
            case GLFW_KEY_LEFT_ALT: return ImGuiKey_LeftAlt;
            case GLFW_KEY_LEFT_SUPER: return ImGuiKey_LeftSuper;
            case GLFW_KEY_RIGHT_SHIFT: return ImGuiKey_RightShift;
            case GLFW_KEY_RIGHT_CONTROL: return ImGuiKey_RightCtrl;
            case GLFW_KEY_RIGHT_ALT: return ImGuiKey_RightAlt;
            case GLFW_KEY_RIGHT_SUPER: return ImGuiKey_RightSuper;
            case GLFW_KEY_MENU: return ImGuiKey_Menu;
            case GLFW_KEY_0: return ImGuiKey_0;
            case GLFW_KEY_1: return ImGuiKey_1;
            case GLFW_KEY_2: return ImGuiKey_2;
            case GLFW_KEY_3: return ImGuiKey_3;
            case GLFW_KEY_4: return ImGuiKey_4;
            case GLFW_KEY_5: return ImGuiKey_5;
            case GLFW_KEY_6: return ImGuiKey_6;
            case GLFW_KEY_7: return ImGuiKey_7;
            case GLFW_KEY_8: return ImGuiKey_8;
            case GLFW_KEY_9: return ImGuiKey_9;
            case GLFW_KEY_A: return ImGuiKey_A;
            case GLFW_KEY_B: return ImGuiKey_B;
            case GLFW_KEY_C: return ImGuiKey_C;
            case GLFW_KEY_D: return ImGuiKey_D;
            case GLFW_KEY_E: return ImGuiKey_E;
            case GLFW_KEY_F: return ImGuiKey_F;
            case GLFW_KEY_G: return ImGuiKey_G;
            case GLFW_KEY_H: return ImGuiKey_H;
            case GLFW_KEY_I: return ImGuiKey_I;
            case GLFW_KEY_J: return ImGuiKey_J;
            case GLFW_KEY_K: return ImGuiKey_K;
            case GLFW_KEY_L: return ImGuiKey_L;
            case GLFW_KEY_M: return ImGuiKey_M;
            case GLFW_KEY_N: return ImGuiKey_N;
            case GLFW_KEY_O: return ImGuiKey_O;
            case GLFW_KEY_P: return ImGuiKey_P;
            case GLFW_KEY_Q: return ImGuiKey_Q;
            case GLFW_KEY_R: return ImGuiKey_R;
            case GLFW_KEY_S: return ImGuiKey_S;
            case GLFW_KEY_T: return ImGuiKey_T;
            case GLFW_KEY_U: return ImGuiKey_U;
            case GLFW_KEY_V: return ImGuiKey_V;
            case GLFW_KEY_W: return ImGuiKey_W;
            case GLFW_KEY_X: return ImGuiKey_X;
            case GLFW_KEY_Y: return ImGuiKey_Y;
            case GLFW_KEY_Z: return ImGuiKey_Z;
            case GLFW_KEY_F1: return ImGuiKey_F1;
            case GLFW_KEY_F2: return ImGuiKey_F2;
            case GLFW_KEY_F3: return ImGuiKey_F3;
            case GLFW_KEY_F4: return ImGuiKey_F4;
            case GLFW_KEY_F5: return ImGuiKey_F5;
            case GLFW_KEY_F6: return ImGuiKey_F6;
            case GLFW_KEY_F7: return ImGuiKey_F7;
            case GLFW_KEY_F8: return ImGuiKey_F8;
            case GLFW_KEY_F9: return ImGuiKey_F9;
            case GLFW_KEY_F10: return ImGuiKey_F10;
            case GLFW_KEY_F11: return ImGuiKey_F11;
            case GLFW_KEY_F12: return ImGuiKey_F12;
            case GLFW_KEY_F13: return ImGuiKey_F13;
            case GLFW_KEY_F14: return ImGuiKey_F14;
            case GLFW_KEY_F15: return ImGuiKey_F15;
            case GLFW_KEY_F16: return ImGuiKey_F16;
            case GLFW_KEY_F17: return ImGuiKey_F17;
            case GLFW_KEY_F18: return ImGuiKey_F18;
            case GLFW_KEY_F19: return ImGuiKey_F19;
            case GLFW_KEY_F20: return ImGuiKey_F20;
            case GLFW_KEY_F21: return ImGuiKey_F21;
            case GLFW_KEY_F22: return ImGuiKey_F22;
            case GLFW_KEY_F23: return ImGuiKey_F23;
            case GLFW_KEY_F24: return ImGuiKey_F24;
            default: return ImGuiKey_None;
        }
    }
}