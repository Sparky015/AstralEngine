//
// Created by sparky on 4/20/2024.
//

#pragma once

#include <Ayla/EventSystem/Event.h>

namespace Ayla {

    class KeyEvent : public Event {
    public:
        virtual int getEventCategoryFlags() override {
            return (InputCategory);
        }
        virtual int getEventType() override {
            return MOUSE_BUTTON_RELEASED;
        }
    };


    class KeyPressedEvent : public KeyEvent {
    public:
        KeyPressedEvent(int keycode) : m_keycode(keycode){}

        int getEventCategoryFlags() override {
            return (KeyboardCategory | InputCategory);
        }
        int getEventType() override {
            return KEY_PRESSED;
        }
        int getKeycode() const { return m_keycode;}

    private:
        int m_keycode;
    };

    class KeyPressedRepeatingEvent : public KeyEvent {
    public:
        KeyPressedRepeatingEvent(int keycode) : m_keycode(keycode){}

        int getEventCategoryFlags() override {
            return (KeyboardCategory | InputCategory);
        }
        int getEventType() override {
            return KEY_PRESSED_REPEATING;
        }

        int getKeycode() const { return m_keycode;}

    private:
        int m_keycode;
    };



    class KeyReleasedEvent : public KeyEvent {
    public:
        KeyReleasedEvent(int keycode) : m_keycode(keycode){}

        int getEventCategoryFlags() override {
            return (KeyboardCategory | InputCategory);
        }
        int getEventType() override {
            return KEY_RELEASED;
        }

        int getKeycode()const { return m_keycode;}

    private:
        int m_keycode;
    };



}; // Ayla
