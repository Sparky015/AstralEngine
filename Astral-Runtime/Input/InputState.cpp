//
// Created by Andrew Fagan on 12/6/24.
//

#include "InputState.h"

#include "Debug/Utilities/Asserts.h"
#include "Debug/Utilities/Error.h"
#include "Debug/Utilities/Loggers.h"

namespace Astral {

    std::array<KeyState, InputState::NUMBER_OF_KEYS> InputState::m_KeyState = {KeyState()};
    MouseCursorState InputState::m_MouseCursorState = MouseCursorState();

    bool InputState::IsKeyDown(int keycode)
    {
        return m_KeyState[keycode].IsDown;
    }


    bool InputState::IsKeyRepeating(int keycode)
    {
        return m_KeyState[keycode].IsRepeating;
    }


    std::string_view InputState::GetKeyName(int keycode)
    {
        switch (keycode)
        {
            case KEY_NONE:              return "None";
            case KEY_LEFT_CLICK:        return "Left Click";
            case KEY_MIDDLE_CLICK:      return "Middle Click";
            case KEY_RIGHT_CLICK:       return "Right Click";
            case KEY_A:                 return "A";
            case KEY_B:                 return "B";
            case KEY_C:                 return "C";
            case KEY_D:                 return "D";
            case KEY_E:                 return "E";
            case KEY_F:                 return "F";
            case KEY_G:                 return "G";
            case KEY_H:                 return "H";
            case KEY_I:                 return "I";
            case KEY_J:                 return "J";
            case KEY_K:                 return "K";
            case KEY_L:                 return "L";
            case KEY_M:                 return "M";
            case KEY_N:                 return "N";
            case KEY_O:                 return "O";
            case KEY_P:                 return "P";
            case KEY_Q:                 return "Q";
            case KEY_R:                 return "R";
            case KEY_S:                 return "S";
            case KEY_T:                 return "T";
            case KEY_U:                 return "U";
            case KEY_V:                 return "V";
            case KEY_W:                 return "W";
            case KEY_X:                 return "X";
            case KEY_Y:                 return "Y";
            case KEY_Z:                 return "Z";
            case KEY_0:                 return "0";
            case KEY_1:                 return "1";
            case KEY_2:                 return "2";
            case KEY_3:                 return "3";
            case KEY_4:                 return "4";
            case KEY_5:                 return "5";
            case KEY_6:                 return "6";
            case KEY_7:                 return "7";
            case KEY_8:                 return "8";
            case KEY_9:                 return "9";
            case KEY_SPACE:             return "Space";
            case KEY_ESCAPE:            return "Escape";
            case KEY_ENTER:             return "Enter";
            case KEY_TAB:               return "Tab";
            case KEY_BACKSPACE:         return "Backspace";
            case KEY_INSERT:            return "Insert";
            case KEY_DELETE:            return "Delete";
            case KEY_RIGHT_ARROW:       return "Right Arrow";
            case KEY_LEFT_ARROW:        return "Left Arrow";
            case KEY_DOWN_ARROW:        return "Down Arrow";
            case KEY_UP_ARROW:          return "Up Arrow";
            case KEY_PAGE_UP:           return "Page Up";
            case KEY_PAGE_DOWN:         return "Page Down";
            case KEY_HOME:              return "Home";
            case KEY_END:               return "End";
            case KEY_CAPS_LOCK:         return "Caps Lock";
            case KEY_SCROLL_LOCK:       return "Scroll Lock";
            case KEY_NUM_LOCK:          return "Num Lock";
            case KEY_PRINT_SCREEN:      return "Print Screen";
            case KEY_PAUSE:             return "Pause";
            case KEY_APOSTROPHE:        return "Apostrophe";
            case KEY_COMMA:             return "Comma";
            case KEY_MINUS:             return "Minus";
            case KEY_EQUAL:             return "Equal";
            case KEY_BACKSLASH:         return "Backslash";
            case KEY_SEMICOLON:         return "Semicolon";
            case KEY_SLASH:             return "Slash";
            case KEY_LEFT_BRACKET:      return "Left Bracket";
            case KEY_RIGHT_BRACKET:     return "Right Bracket";
            case KEY_GRAVE_ACCENT:      return "Grave Accent";
            case KEY_PERIOD:            return "Period";
            case KEY_LEFT_SHIFT:        return "Left Shift";
            case KEY_RIGHT_SHIFT:       return "Right Shift";
            case KEY_LEFT_CONTROL:      return "Left Control";
            case KEY_RIGHT_CONTROL:     return "Right Control";
            case KEY_LEFT_ALT:          return "Left Alt";
            case KEY_RIGHT_ALT:         return "Right Alt";
            case KEY_LEFT_SUPER:        return "Left Super";
            case KEY_RIGHT_SUPER:       return "Right Super";
            case KEY_KP_0:              return "Keypad 0";
            case KEY_KP_1:              return "Keypad 1";
            case KEY_KP_2:              return "Keypad 2";
            case KEY_KP_3:              return "Keypad 3";
            case KEY_KP_4:              return "Keypad 4";
            case KEY_KP_5:              return "Keypad 5";
            case KEY_KP_6:              return "Keypad 6";
            case KEY_KP_7:              return "Keypad 7";
            case KEY_KP_8:              return "Keypad 8";
            case KEY_KP_9:              return "Keypad 9";
            case KEY_KP_DECIMAL:        return "Keypad Decimal";
            case KEY_KP_DIVIDE:         return "Keypad Divide";
            case KEY_KP_MULTIPLY:       return "Keypad Multiply";
            case KEY_KP_SUBTRACT:       return "Keypad Subtract";
            case KEY_KP_ADD:            return "Keypad Add";
            case KEY_KP_ENTER:          return "Keypad Enter";
            case KEY_KP_EQUAL:          return "Keypad Equal";
            case KEY_F1:                return "F1";
            case KEY_F2:                return "F2";
            case KEY_F3:                return "F3";
            case KEY_F4:                return "F4";
            case KEY_F5:                return "F5";
            case KEY_F6:                return "F6";
            case KEY_F7:                return "F7";
            case KEY_F8:                return "F8";
            case KEY_F9:                return "F9";
            case KEY_F10:               return "F10";
            case KEY_F11:               return "F11";
            case KEY_F12:               return "F12";
            case KEY_F13:               return "F13";
            case KEY_F14:               return "F14";
            case KEY_F15:               return "F15";
            case KEY_F16:               return "F16";
            case KEY_F17:               return "F17";
            case KEY_F18:               return "F18";
            case KEY_F19:               return "F19";
            case KEY_F20:               return "F20";
            case KEY_F21:               return "F21";
            case KEY_F22:               return "F22";
            case KEY_F23:               return "F23";
            case KEY_F24:               return "F24";
            default:                    ASTRAL_ERROR("Invalid keycode!");
        }
    }


    double InputState::MousePositionX()
    {
        return m_MouseCursorState.MouseXPosition;
    }


    double InputState::MousePositionY()
    {
        return m_MouseCursorState.MouseYPosition;
    }


    Astral::EventListener<KeyPressedEvent> InputState::m_KeyPressListener = Astral::EventListener<KeyPressedEvent> {[](KeyPressedEvent keyPressedEvent){InputState::OnKeyPress(keyPressedEvent);}};
    Astral::EventListener<KeyReleasedEvent> InputState::m_KeyReleaseListener = Astral::EventListener<KeyReleasedEvent> {[](KeyReleasedEvent keyReleasedEvent){InputState::OnKeyRelease(keyReleasedEvent);}};
    Astral::EventListener<KeyRepeatingEvent> InputState::m_KeyRepeatingListener = Astral::EventListener<KeyRepeatingEvent> {[](KeyRepeatingEvent keyRepeatingEvent){InputState::OnKeyRepeating(keyRepeatingEvent);}};
    Astral::EventListener<MouseMovedEvent> InputState::m_MouseMovedListener = Astral::EventListener<MouseMovedEvent> {[](MouseMovedEvent mouseMovedEvent){InputState::OnMouseMoved(mouseMovedEvent);}};


    void InputState::OnKeyPress(KeyPressedEvent keyPressedEvent)
    {
        uint8 keycode = keyPressedEvent.keycode;
        ASSERT(keycode < NUMBER_OF_KEYS, "Keycode is not valid. Needs to be in range of 0 - NUMBER_OF_KEYS.")
        m_KeyState[keyPressedEvent.keycode].IsDown = true;
        m_KeyState[keyPressedEvent.keycode].IsRepeating = false;
    }


    void InputState::OnKeyRelease(KeyReleasedEvent keyReleasedEvent)
    {
        uint8 keycode = keyReleasedEvent.keycode;
        ASSERT(keycode < NUMBER_OF_KEYS, "Keycode is not valid. Needs to be in range of 0 - NUMBER_OF_KEYS.")
        m_KeyState[keyReleasedEvent.keycode].IsDown = false;
        m_KeyState[keyReleasedEvent.keycode].IsRepeating = false;
    }


    void InputState::OnKeyRepeating(KeyRepeatingEvent keyRepeatingEvent)
    {
        uint8 keycode = keyRepeatingEvent.keycode;
        ASSERT(keycode < NUMBER_OF_KEYS, "Keycode is not valid. Needs to be in range of 0 - NUMBER_OF_KEYS.")
        m_KeyState[keyRepeatingEvent.keycode].IsRepeating = true;
    }


    void InputState::OnMouseMoved(MouseMovedEvent mouseMovedEvent)
    {
        m_MouseCursorState.MouseXPosition = mouseMovedEvent.xPosition;
        m_MouseCursorState.MouseYPosition = mouseMovedEvent.yPosition;
    }


    void InputState::Init()
    {
        TRACE("Initializing Input State!")
        m_KeyPressListener.StartListening();
        m_KeyReleaseListener.StartListening();
        m_KeyRepeatingListener.StartListening();
        m_MouseMovedListener.StartListening();
    }


    void InputState::Shutdown()
    {
        TRACE("Shutting down Input State!")
        m_KeyPressListener.StopListening();
        m_KeyReleaseListener.StopListening();
        m_KeyRepeatingListener.StopListening();
        m_MouseMovedListener.StopListening();
    }

}
