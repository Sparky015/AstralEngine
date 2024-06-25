//
// Created by Andrew Fagan on 5/16/24.
//
#pragma once

#include "Ayla/Input/InputLayer.h"
#include "Ayla/aypch.h"



namespace Ayla::Input {

    enum KeyCodes {
        AY_KEY_RIGHT_CLICK = 0,
        AY_KEY_MIDDLE_CLICK,
        AY_KEY_LEFT_CLICK,

        AY_KEY_A,
        AY_KEY_B,
        AY_KEY_C,
        AY_KEY_D,
        AY_KEY_E,
        AY_KEY_F,
        AY_KEY_G,
        AY_KEY_H,
        AY_KEY_I,
        AY_KEY_J,
        AY_KEY_K,
        AY_KEY_L,
        AY_KEY_M,
        AY_KEY_N,
        AY_KEY_O,
        AY_KEY_P,
        AY_KEY_Q,
        AY_KEY_R,
        AY_KEY_S,
        AY_KEY_T,
        AY_KEY_U,
        AY_KEY_V,
        AY_KEY_W,
        AY_KEY_X,
        AY_KEY_Y,
        AY_KEY_Z,

        AY_KEY_0,
        AY_KEY_1,
        AY_KEY_2,
        AY_KEY_3,
        AY_KEY_4,
        AY_KEY_5,
        AY_KEY_6,
        AY_KEY_7,
        AY_KEY_8,
        AY_KEY_9,

        AY_KEY_F1,
        AY_KEY_F2,
        AY_KEY_F3,
        AY_KEY_F4,
        AY_KEY_F5,
        AY_KEY_F6,
        AY_KEY_F7,
        AY_KEY_F8,
        AY_KEY_F9,
        AY_KEY_F10,
        AY_KEY_F11,
        AY_KEY_F12,
        AY_KEY_ESCAPE,
        AY_KEY_ENTER,
        AY_KEY_TAB,
        AY_KEY_BACKSPACE,
        AY_KEY_INSERT,
        AY_KEY_DELETE,
        AY_KEY_RIGHT,
        AY_KEY_LEFT,
        AY_KEY_DOWN,
        AY_KEY_UP,
        AY_KEY_PAGE_UP,
        AY_KEY_PAGE_DOWN,
        AY_KEY_HOME,
        AY_KEY_END,
        AY_KEY_CAPS_LOCK,
        AY_KEY_SCROLL_LOCK,
        AY_KEY_NUM_LOCK,
        AY_KEY_PRINT_SCREEN,
        AY_KEY_PAUSE,
        AY_KEY_LEFT_SHIFT,
        AY_KEY_LEFT_CONTROL,
        AY_KEY_LEFT_ALT,
        AY_KEY_LEFT_SUPER,
        AY_KEY_RIGHT_SHIFT,
        AY_KEY_RIGHT_CONTROL,
        AY_KEY_RIGHT_ALT,
        AY_KEY_RIGHT_SUPER,
    };


class InputState {
public:
    static InputState& get();

    void onEvent(Event&);


    InputState(const InputState&) = delete;
    InputState& operator=(const InputState&) = delete;
    InputState(InputState&&) = delete;
    InputState& operator=(InputState&&) = delete;

    struct KeyState {
        bool isDown;
        bool isRepeating;
        std::string name;

        KeyState() : isDown(false), isRepeating(false), name("No Name Given"){}
        KeyState(std::string&& name)  : isDown(false), isRepeating(false), name(name){}

    };

    KeyState access(int keycode);

private:
    static InputState* m_instance;
    InputState();
    ~InputState();



    KeyState m_keyState[78];

    double m_mouseXPosition;
    double m_mouseYPosition;



    InputLayer m_inputLayer;
};

    namespace Translation {

        int translateGLFWKeycodesToAyla(int GLFWKeycode);

    }

}

