//
// Created by sparky on 4/20/2024.
//

#pragma once

#include "Ayla/Events/Event.h"

namespace Ayla::Events {



    class KeyEvent : public IEvent {
    public:

        int getEventCategoryFlags() override {
            return (INPUT_CATEGORY);
        }


        int getEventType() override {
            return MOUSE_BUTTON_RELEASED;
        }
    };


    class KeyPressedEvent : public KeyEvent {
    public:

        explicit KeyPressedEvent(int keycode) : m_keycode(keycode) {}

        int getEventCategoryFlags() override {
            return (KEYBOARD_CATEGORY | INPUT_CATEGORY);
        }

        int getEventType() override {
            return KEY_PRESSED;
        }

        int getKeycode() const { return m_keycode; }

    private:
        int m_keycode;
    };

    class KeyTypedEvent : public KeyEvent {
    public:

        explicit KeyTypedEvent(unsigned int keycode) : m_keycode(keycode) {}

        int getEventCategoryFlags() override {
            return (KEYBOARD_CATEGORY | INPUT_CATEGORY);
        }

        int getEventType() override {
            return KEY_TYPED;
        }

        int getKeycode() const { return m_keycode; }

    private:
        unsigned int m_keycode;
    };


    class KeyPressedRepeatingEvent : public KeyEvent {
    public:

        KeyPressedRepeatingEvent(int keycode) : m_keycode(keycode) {}


        int getEventCategoryFlags() override {
            return (KEYBOARD_CATEGORY | INPUT_CATEGORY);
        }


        int getEventType() override {
            return KEY_PRESSED_REPEATING;
        }


        int getKeycode() const { return m_keycode; }

    private:
        int m_keycode;
    };


    class KeyReleasedEvent : public KeyEvent {
    public:

        KeyReleasedEvent(int keycode) : m_keycode(keycode) {}


        int getEventCategoryFlags() override {
            return (KEYBOARD_CATEGORY | INPUT_CATEGORY);
        }


        int getEventType() override {
            return KEY_RELEASED;
        }


        int getKeycode() const { return m_keycode; }

    private:
        int m_keycode;
    };



}; // Ayla
