//
// Created by Andrew Fagan on 5/16/24.
//
#pragma once

#include "Ayla/Input/InputLayer.h"
#include "Ayla/aypch.h"


namespace Ayla::Input {


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
        double accessMouseX();
        double accessMouseY();
    private:
        static InputState* m_instance;
        InputState();
        ~InputState();


        KeyState m_keyState[78];

        double m_mouseXPosition;
        double m_mouseYPosition;


        InputLayer m_inputLayer;
    };


}

