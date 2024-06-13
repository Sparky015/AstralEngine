//
// Created by Andrew Fagan on 5/16/24.
//

#include "InputState.h"
#include "Ayla/Events/ApplicationEvent.h"
#include "Ayla/Events/KeyEvent.h"
#include "Ayla/Events/MouseEvent.h"

namespace Ayla {
    InputState::InputState() : m_inputLayer(InputLayer()) {
        AY_TRACE("InputState: Initializing Input State");
        m_inputLayer.setCallback(std::bind(&InputState::onEvent, this, std::placeholders::_1));
    }

    InputState::~InputState() {
        delete m_instance;
        m_instance = nullptr;
    }

    InputState* InputState::m_instance = nullptr;

    InputState& InputState::get() {
        AY_TRACE("InputState: Getting Input State...");
        if (m_instance == nullptr){
            AY_TRACE("InputState: Making 'new InputState()'");
            m_instance = new InputState();
        }
        return *m_instance;
    }


    void InputState::onEvent(Ayla::Event& event) {
        AY_ASSERT(event.isInCategory(InputCategory), "InputState received an event that is not in the Input Category!");
        AY_TRACE("Input Received\t");
        if (event.isInCategory(KeyboardCategory)){
            if (event.getEventType() == KEY_PRESSED){
                auto keyPressedEvent = dynamic_cast<Ayla::KeyPressedEvent&>(event);
                std::cout << keyPressedEvent.getKeycode();
            }

            if (event.getEventType() == KEY_RELEASED){
                auto keyReleasedEvent = dynamic_cast<Ayla::KeyReleasedEvent&>(event);
                std::cout << keyReleasedEvent.getKeycode();
            }

            if (event.getEventType() == KEY_PRESSED_REPEATING){
                auto keyPressedRepeatingEvent = dynamic_cast<Ayla::KeyPressedRepeatingEvent&>(event);
                std::cout << keyPressedRepeatingEvent.getKeycode();
            }

        }

        if (event.isInCategory(MouseCategory)) {
            if (event.getEventType() == MOUSE_BUTTON_PRESSED) {
                auto mouseButtonPressedEvent = dynamic_cast<Ayla::MouseButtonPressEvent&>(event);
                std::cout << mouseButtonPressedEvent.getButton();
            }

            if (event.getEventType() == MOUSE_BUTTON_RELEASED) {
                auto mouseButtonReleasedEvent = dynamic_cast<Ayla::MouseButtonReleaseEvent&>(event);
                std::cout << mouseButtonReleasedEvent.getButton();
            }

            if (event.getEventType() == MOUSE_CURSOR_MOVED) {
                auto mouseMovedEvent = dynamic_cast<Ayla::MouseMovedEvent&>(event);
                std::cout << mouseMovedEvent.getXPos() << ", " << mouseMovedEvent.getYPos();
            }

            if (event.getEventType() == MOUSE_SCROLLED) {
                auto mouseScrolledEvent = dynamic_cast<Ayla::MouseScrollEvent&>(event);
                std::cout << mouseScrolledEvent.getXOffset() << ", " << mouseScrolledEvent.getYOffset();
            }
        }


        event.setIsHandled(true);
    }


}