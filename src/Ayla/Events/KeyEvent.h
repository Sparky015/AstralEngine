//
// Created by sparky on 4/20/2024.
//

#pragma once

#include "Ayla/Events/Event.h"

namespace Ayla::Events {

    class KeyEvent : public IEvent
    {
    public:

        [[nodiscard]] virtual int8_t GetEventCategoryFlags() const override
        {
            return (INPUT_CATEGORY);
        }


        [[nodiscard]] virtual int GetEventType() const override
        {
            return MOUSE_BUTTON_RELEASED;
        }

    protected:

        /** KeyEvent is not meant to be created as a standalone event. */
        KeyEvent() = default;
    };


    class KeyPressedEvent : public KeyEvent
    {
    public:
        explicit KeyPressedEvent(int keycode) : m_Keycode(keycode) {}

        [[nodiscard]] int8_t GetEventCategoryFlags() const override
        {
            return (KEYBOARD_CATEGORY | INPUT_CATEGORY);
        }


        [[nodiscard]] int GetEventType() const override
        {
            return KEY_PRESSED;
        }


        [[nodiscard]] int GetKeycode() const
        {
            return m_Keycode;
        }

    private:
        int m_Keycode;
    };


    class KeyTypedEvent : public KeyEvent
    {
    public:
        explicit KeyTypedEvent(unsigned int keycode) : m_Keycode(keycode) {}

        [[nodiscard]] int8_t GetEventCategoryFlags() const override
        {
            return (KEYBOARD_CATEGORY | INPUT_CATEGORY);
        }

        [[nodiscard]] int GetEventType() const override
        {
            return KEY_TYPED;
        }

        [[nodiscard]] unsigned int GetKeycode() const { return m_Keycode; }

    private:
        unsigned int m_Keycode;
    };


    class KeyPressedRepeatingEvent : public KeyEvent
    {
    public:
        explicit KeyPressedRepeatingEvent(int keycode) : m_Keycode(keycode) {}

        [[nodiscard]] int8_t GetEventCategoryFlags() const override
        {
            return (KEYBOARD_CATEGORY | INPUT_CATEGORY);
        }


        [[nodiscard]] int GetEventType() const override
        {
            return KEY_PRESSED_REPEATING;
        }


        [[nodiscard]] int GetKeycode() const
        {
            return m_Keycode;
        }

    private:
        int m_Keycode;
    };


    class KeyReleasedEvent : public KeyEvent
    {
    public:
        explicit KeyReleasedEvent(int keycode) : m_Keycode(keycode) {}

        [[nodiscard]] int8_t GetEventCategoryFlags() const override
        {
            return (KEYBOARD_CATEGORY | INPUT_CATEGORY);
        }


        [[nodiscard]] int GetEventType() const override
        {
            return KEY_RELEASED;
        }


        [[nodiscard]] int GetKeycode() const
        {
            return m_Keycode;
        }

    private:
        int m_Keycode;
    };

} // namespace Ayla::Events
