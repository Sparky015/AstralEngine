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



    SInputState::SInputState() : m_inputLayer(InputLayer()) {
        AY_TRACE("[Sholas] InputState: Initializing Input State");

        m_inputLayer.SetCallback(std::bind(&SInputState::onEvent, this, std::placeholders::_1));
    }

    SInputState::~SInputState() {
        AY_TRACE("[Sholas] InputState: Destroying Input State");
        delete m_instance;
        m_instance = nullptr;
    }

    SInputState* SInputState::m_instance = nullptr;

    void SInputState::init() {
        AY_ASSERT(m_instance == nullptr, "Input/InputState.cpp: Input State has already been initialized!");

        AY_TRACE("[Sholas] InputState: Initializing Input State'");
        m_instance = new SInputState();

    }

    SInputState& SInputState::get() {
        AY_ASSERT(m_instance != nullptr, "Input/InputState.cpp: Input State has not been initialized yet!");

        return *m_instance;
    }


    void SInputState::onEvent(IEvent& event) {
        AY_ASSERT(event.IsInCategory(INPUT_CATEGORY), "Input/InputState.cpp: InputState received an event that is not in the Input Category!");
        //AY_TRACE("Input Received\t");
        if (event.IsInCategory(KEYBOARD_CATEGORY)){
            if (event.GetEventType() == KEY_PRESSED){
                auto keyPressedEvent = dynamic_cast<KeyPressedEvent&>(event);
                m_keyState[keyPressedEvent.getKeycode()].isDown = true;
                //std::cout << keyPressedEvent.getKeycode();.
            }

            if (event.GetEventType() == KEY_RELEASED){
                auto keyReleasedEvent = dynamic_cast<KeyReleasedEvent&>(event);
                m_keyState[keyReleasedEvent.getKeycode()].isDown = false;
                m_keyState[keyReleasedEvent.getKeycode()].isRepeating = false;
                //std::cout << keyReleasedEvent.getKeycode();
            }

            if (event.GetEventType() == KEY_PRESSED_REPEATING){
                auto keyPressedRepeatingEvent = dynamic_cast<KeyPressedRepeatingEvent&>(event);
                m_keyState[keyPressedRepeatingEvent.getKeycode()].isRepeating = true;
                //std::cout << keyPressedRepeatingEvent.getKeycode();
            }

        }

        if (event.IsInCategory(MOUSE_CATEGORY)) {
            if (event.GetEventType() == MOUSE_BUTTON_PRESSED) {
                auto mouseButtonPressedEvent = dynamic_cast<MouseButtonPressEvent&>(event);
                m_keyState[mouseButtonPressedEvent.getButton()].isDown = true;
                //std::cout << mouseButtonPressedEvent.getButton();
            }

            if (event.GetEventType() == MOUSE_BUTTON_RELEASED) {
                auto mouseButtonReleasedEvent = dynamic_cast<MouseButtonReleaseEvent&>(event);
                m_keyState[mouseButtonReleasedEvent.getButton()].isDown = false;
                m_keyState[mouseButtonReleasedEvent.getButton()].isRepeating = false;
                //std::cout << mouseButtonReleasedEvent.getButton();
            }

            if (event.GetEventType() == MOUSE_CURSOR_MOVED) {
                auto mouseMovedEvent = dynamic_cast<MouseMovedEvent&>(event);
                m_mouseCursorState.mouseXPosition = mouseMovedEvent.getXPos();
                m_mouseCursorState.mouseYPosition = mouseMovedEvent.getYPos();
                //std::cout << mouseMovedEvent.GetXPos() << ", " << mouseMovedEvent.GetYPos();
            }

            if (event.GetEventType() == MOUSE_SCROLLED) {
                auto mouseScrolledEvent = dynamic_cast<MouseScrollEvent&>(event);
                //std::cout << mouseScrolledEvent.getXOffset() << ", " << mouseScrolledEvent.getYOffset();
            }
        }

        event.SetIsHandled(true);
    }


    double SInputState::mousePositionX() const {
        return m_mouseCursorState.mouseXPosition;
    }

    double SInputState::mousePositionY() const {
        return m_mouseCursorState.mouseYPosition;
    }



    bool SInputState::isKeyDown(int keycode) const {
        return m_keyState[keycode].isDown;
    }

    bool SInputState::isKeyRepeating(int keycode) const {
        return m_keyState[keycode].isRepeating;
    }

    std::string SInputState::getKeyName(int keycode) const {
        return m_keyState[keycode].name;
    }


}


