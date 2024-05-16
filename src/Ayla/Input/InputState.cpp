//
// Created by Andrew Fagan on 5/16/24.
//

#include "InputState.h"
#include "Ayla/Events/ApplicationEvent.h"
#include "Ayla/Events/KeyEvent.h"
#include "Ayla/Events/MouseEvent.h"

namespace Ayla {
    InputState::InputState() {
        m_inputLayer = InputLayer();
        m_inputLayer.setCallback(std::bind(&InputState::onEvent, this, std::placeholders::_1));
    }

    InputState::~InputState() {
        delete m_instance;
        m_instance = nullptr;
    }

    InputState* InputState::m_instance = nullptr;

    InputState& InputState::get() {
        if (m_instance == nullptr){
            m_instance = new InputState();
        }
        return *m_instance;
    }


    void InputState::onEvent(Ayla::Event& event) {
        AY_ASSERT(!event.isInCategory(InputCategory), "InputState received an unrelated event!");
        if (event.isInCategory(KeyboardCategory)){
            if (event.getEventType() == KEY_PRESSED){
                auto keyPressedEvent = dynamic_cast<Ayla::KeyPressedEvent&>(event);
                keyPressedEvent.getKeycode();
            }

            if (event.getEventType() == KEY_RELEASED){
                auto keyReleasedEvent = dynamic_cast<Ayla::KeyReleasedEvent&>(event);
                keyReleasedEvent.getKeycode();
            }

            if (event.getEventType() == KEY_PRESSED_REPEATING){
                auto keyPressedRepeatingEvent = dynamic_cast<Ayla::KeyPressedRepeatingEvent&>(event);
                keyPressedRepeatingEvent.getKeycode();
            }

        }
    }


}