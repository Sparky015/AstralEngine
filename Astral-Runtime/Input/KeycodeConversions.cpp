//
// Created by Andrew Fagan on 12/6/24.
//

#include "GLFW/glfw3.h"
#include "Keycodes.h"

namespace Input {

    int TranslateGLFWKeycodesToAstral(int glfwKeycode)
    {
        switch (glfwKeycode)
        {
            case GLFW_MOUSE_BUTTON_LEFT:           return KEY_LEFT_CLICK;
            case GLFW_MOUSE_BUTTON_RIGHT:          return KEY_RIGHT_CLICK;
            case GLFW_MOUSE_BUTTON_MIDDLE:         return KEY_MIDDLE_CLICK;

            case GLFW_KEY_A:                       return KEY_A;
            case GLFW_KEY_B:                       return KEY_B;
            case GLFW_KEY_C:                       return KEY_C;
            case GLFW_KEY_D:                       return KEY_D;
            case GLFW_KEY_E:                       return KEY_E;
            case GLFW_KEY_F:                       return KEY_F;
            case GLFW_KEY_G:                       return KEY_G;
            case GLFW_KEY_H:                       return KEY_H;
            case GLFW_KEY_I:                       return KEY_I;
            case GLFW_KEY_J:                       return KEY_J;
            case GLFW_KEY_K:                       return KEY_K;
            case GLFW_KEY_L:                       return KEY_L;
            case GLFW_KEY_M:                       return KEY_M;
            case GLFW_KEY_N:                       return KEY_N;
            case GLFW_KEY_O:                       return KEY_O;
            case GLFW_KEY_P:                       return KEY_P;
            case GLFW_KEY_Q:                       return KEY_Q;
            case GLFW_KEY_R:                       return KEY_R;
            case GLFW_KEY_S:                       return KEY_S;
            case GLFW_KEY_T:                       return KEY_T;
            case GLFW_KEY_U:                       return KEY_U;
            case GLFW_KEY_V:                       return KEY_V;
            case GLFW_KEY_W:                       return KEY_W;
            case GLFW_KEY_X:                       return KEY_X;
            case GLFW_KEY_Y:                       return KEY_Y;
            case GLFW_KEY_Z:                       return KEY_Z;

            case GLFW_KEY_0:                       return KEY_0;
            case GLFW_KEY_1:                       return KEY_1;
            case GLFW_KEY_2:                       return KEY_2;
            case GLFW_KEY_3:                       return KEY_3;
            case GLFW_KEY_4:                       return KEY_4;
            case GLFW_KEY_5:                       return KEY_5;
            case GLFW_KEY_6:                       return KEY_6;
            case GLFW_KEY_7:                       return KEY_7;
            case GLFW_KEY_8:                       return KEY_8;
            case GLFW_KEY_9:                       return KEY_9;

            case GLFW_KEY_SPACE:                   return KEY_SPACE;
            case GLFW_KEY_ESCAPE:                  return KEY_ESCAPE;
            case GLFW_KEY_ENTER:                   return KEY_ENTER;
            case GLFW_KEY_TAB:                     return KEY_TAB;
            case GLFW_KEY_BACKSPACE:               return KEY_BACKSPACE;
            case GLFW_KEY_INSERT:                  return KEY_INSERT;
            case GLFW_KEY_DELETE:                  return KEY_DELETE;
            case GLFW_KEY_RIGHT:                   return KEY_RIGHT_ARROW;
            case GLFW_KEY_LEFT:                    return KEY_LEFT_ARROW;
            case GLFW_KEY_DOWN:                    return KEY_DOWN_ARROW;
            case GLFW_KEY_UP:                      return KEY_UP_ARROW;
            case GLFW_KEY_PAGE_UP:                 return KEY_PAGE_UP;
            case GLFW_KEY_PAGE_DOWN:               return KEY_PAGE_DOWN;
            case GLFW_KEY_HOME:                    return KEY_HOME;
            case GLFW_KEY_END:                     return KEY_END;
            case GLFW_KEY_CAPS_LOCK:               return KEY_CAPS_LOCK;
            case GLFW_KEY_SCROLL_LOCK:             return KEY_SCROLL_LOCK;
            case GLFW_KEY_NUM_LOCK:                return KEY_NUM_LOCK;
            case GLFW_KEY_PRINT_SCREEN:            return KEY_PRINT_SCREEN;
            case GLFW_KEY_PAUSE:                   return KEY_PAUSE;
            case GLFW_KEY_APOSTROPHE:              return KEY_APOSTROPHE;
            case GLFW_KEY_COMMA:                   return KEY_COMMA;
            case GLFW_KEY_MINUS:                   return KEY_MINUS;
            case GLFW_KEY_EQUAL:                   return KEY_EQUAL;
            case GLFW_KEY_BACKSLASH:               return KEY_BACKSLASH;
            case GLFW_KEY_SEMICOLON:               return KEY_SEMICOLON;
            case GLFW_KEY_SLASH:                   return KEY_SLASH;
            case GLFW_KEY_LEFT_BRACKET:            return KEY_LEFT_BRACKET;
            case GLFW_KEY_RIGHT_BRACKET:           return KEY_RIGHT_BRACKET;
            case GLFW_KEY_GRAVE_ACCENT:            return KEY_GRAVE_ACCENT;
            case GLFW_KEY_PERIOD:                  return KEY_PERIOD;

            case GLFW_KEY_KP_0:                    return KEY_KP_0;
            case GLFW_KEY_KP_1:                    return KEY_KP_1;
            case GLFW_KEY_KP_2:                    return KEY_KP_2;
            case GLFW_KEY_KP_3:                    return KEY_KP_3;
            case GLFW_KEY_KP_4:                    return KEY_KP_4;
            case GLFW_KEY_KP_5:                    return KEY_KP_5;
            case GLFW_KEY_KP_6:                    return KEY_KP_6;
            case GLFW_KEY_KP_7:                    return KEY_KP_7;
            case GLFW_KEY_KP_8:                    return KEY_KP_8;
            case GLFW_KEY_KP_9:                    return KEY_KP_9;
            case GLFW_KEY_KP_DECIMAL:              return KEY_KP_DECIMAL;
            case GLFW_KEY_KP_DIVIDE:               return KEY_KP_DIVIDE;
            case GLFW_KEY_KP_MULTIPLY:             return KEY_KP_MULTIPLY;
            case GLFW_KEY_KP_SUBTRACT:             return KEY_KP_SUBTRACT;
            case GLFW_KEY_KP_ADD:                  return KEY_KP_ADD;
            case GLFW_KEY_KP_ENTER:                return KEY_KP_ENTER;
            case GLFW_KEY_KP_EQUAL:                return KEY_KP_EQUAL;


            case GLFW_KEY_LEFT_SHIFT:              return KEY_LEFT_SHIFT;
            case GLFW_KEY_RIGHT_SHIFT:             return KEY_RIGHT_SHIFT;
            case GLFW_KEY_LEFT_CONTROL:            return KEY_LEFT_CONTROL;
            case GLFW_KEY_RIGHT_CONTROL:           return KEY_RIGHT_CONTROL;
            case GLFW_KEY_LEFT_ALT:                return KEY_LEFT_ALT;
            case GLFW_KEY_RIGHT_ALT:               return KEY_RIGHT_ALT;
            case GLFW_KEY_LEFT_SUPER:              return KEY_LEFT_SUPER;
            case GLFW_KEY_RIGHT_SUPER:             return KEY_RIGHT_SUPER;

            case GLFW_KEY_F1:                      return KEY_F1;
            case GLFW_KEY_F2:                      return KEY_F2;
            case GLFW_KEY_F3:                      return KEY_F3;
            case GLFW_KEY_F4:                      return KEY_F4;
            case GLFW_KEY_F5:                      return KEY_F5;
            case GLFW_KEY_F6:                      return KEY_F6;
            case GLFW_KEY_F7:                      return KEY_F7;
            case GLFW_KEY_F8:                      return KEY_F8;
            case GLFW_KEY_F9:                      return KEY_F9;
            case GLFW_KEY_F10:                     return KEY_F10;
            case GLFW_KEY_F11:                     return KEY_F11;
            case GLFW_KEY_F12:                     return KEY_F12;
            case GLFW_KEY_F13:                     return KEY_F13;
            case GLFW_KEY_F14:                     return KEY_F14;
            case GLFW_KEY_F15:                     return KEY_F15;
            case GLFW_KEY_F16:                     return KEY_F16;
            case GLFW_KEY_F17:                     return KEY_F17;
            case GLFW_KEY_F18:                     return KEY_F18;
            case GLFW_KEY_F19:                     return KEY_F19;
            case GLFW_KEY_F20:                     return KEY_F20;
            case GLFW_KEY_F21:                     return KEY_F21;
            case GLFW_KEY_F22:                     return KEY_F22;
            case GLFW_KEY_F23:                     return KEY_F23;
            case GLFW_KEY_F24:                     return KEY_F24;
            case -1:                               return KEY_NONE;    // GLFW defaults to -1 when the key is unrecognized
            default:                               ASTRAL_ERROR("Invalid Keycode!");

        }
    }

}