//
// Created by Andrew Fagan on 5/16/24.
//

#include "InputState.h"
#include "Ayla/Events/ApplicationEvent.h"
#include "Ayla/Events/KeyEvent.h"
#include "Ayla/Events/MouseEvent.h"
#include "Ayla/Input/Keycodes.h"


using namespace Ayla::Events;

namespace Ayla::Input {



    InputState::InputState() : m_inputLayer(InputLayer()) {
        AY_TRACE("InputState: Initializing Input State");

        m_inputLayer.setCallback(std::bind(&InputState::onEvent, this, std::placeholders::_1));
    }

    InputState::~InputState() {
        delete m_instance;
        m_instance = nullptr;
    }

    InputState* InputState::m_instance = nullptr;

    void InputState::init() {
        AY_ASSERT(m_instance == nullptr, "Input/InputState.cpp: Input State has already been initialized!");

        AY_TRACE("InputState: Making 'new InputState()'");
        m_instance = new InputState();

    }

    InputState& InputState::get() {
        AY_ASSERT(m_instance != nullptr, "Input/InputState.cpp: Input State has not been initialized yet!");

        return *m_instance;
    }


    void InputState::onEvent(Event& event) {
        AY_ASSERT(event.isInCategory(InputCategory), "Input/InputState.cpp: InputState received an event that is not in the Input Category!");
        //AY_TRACE("Input Received\t");
        if (event.isInCategory(KeyboardCategory)){
            if (event.getEventType() == KEY_PRESSED){
                auto keyPressedEvent = dynamic_cast<KeyPressedEvent&>(event);
                m_keyState[keyPressedEvent.getKeycode()].isDown = true;
                //std::cout << keyPressedEvent.getKeycode();.
            }

            if (event.getEventType() == KEY_RELEASED){
                auto keyReleasedEvent = dynamic_cast<KeyReleasedEvent&>(event);
                m_keyState[keyReleasedEvent.getKeycode()].isDown = false;
                m_keyState[keyReleasedEvent.getKeycode()].isRepeating = false;
                //std::cout << keyReleasedEvent.getKeycode();
            }

            if (event.getEventType() == KEY_PRESSED_REPEATING){
                auto keyPressedRepeatingEvent = dynamic_cast<KeyPressedRepeatingEvent&>(event);
                m_keyState[keyPressedRepeatingEvent.getKeycode()].isRepeating = true;
                //std::cout << keyPressedRepeatingEvent.getKeycode();
            }

        }

        if (event.isInCategory(MouseCategory)) {
            if (event.getEventType() == MOUSE_BUTTON_PRESSED) {
                auto mouseButtonPressedEvent = dynamic_cast<MouseButtonPressEvent&>(event);
                m_keyState[mouseButtonPressedEvent.getButton()].isDown = true;
                //std::cout << mouseButtonPressedEvent.getButton();
            }

            if (event.getEventType() == MOUSE_BUTTON_RELEASED) {
                auto mouseButtonReleasedEvent = dynamic_cast<MouseButtonReleaseEvent&>(event);
                m_keyState[mouseButtonReleasedEvent.getButton()].isDown = false;
                m_keyState[mouseButtonReleasedEvent.getButton()].isRepeating = false;
                //std::cout << mouseButtonReleasedEvent.getButton();
            }

            if (event.getEventType() == MOUSE_CURSOR_MOVED) {
                auto mouseMovedEvent = dynamic_cast<MouseMovedEvent&>(event);
                m_mouseCursorState.mouseXPosition = mouseMovedEvent.getXPos();
                m_mouseCursorState.mouseYPosition = mouseMovedEvent.getYPos();
                //std::cout << mouseMovedEvent.getXPos() << ", " << mouseMovedEvent.getYPos();
            }

            if (event.getEventType() == MOUSE_SCROLLED) {
                auto mouseScrolledEvent = dynamic_cast<MouseScrollEvent&>(event);
                //std::cout << mouseScrolledEvent.getXOffset() << ", " << mouseScrolledEvent.getYOffset();
            }
        }

        event.setIsHandled(true);
    }


    double InputState::mousePositionX() const {
        return m_mouseCursorState.mouseXPosition;
    }

    double InputState::mousePositionY() const {
        return m_mouseCursorState.mouseYPosition;
    }



    bool InputState::isKeyDown(int keycode) const {
        return m_keyState[keycode].isDown;
    }

    bool InputState::isKeyRepeating(int keycode) const {
        return m_keyState[keycode].isRepeating;
    }

    std::string InputState::getKeyName(int keycode) const {
        return m_keyState[keycode].name;
    }


}


