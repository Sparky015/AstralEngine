//
// Created by Andrew Fagan on 5/16/24.
//
#pragma once

#include "Ayla/Input/InputLayer.h"

namespace Ayla {
    class InputState {
    public:
        static InputState& get();

        void onEvent(Event&);


        InputState(const InputState&) = delete;
        InputState& operator=(const InputState&) = delete;
        InputState(InputState&&) = delete;
        InputState& operator=(InputState&&) = delete;

    private:
        static InputState* m_instance;
        InputState();
        ~InputState();

        InputLayer m_inputLayer;
    };
}
