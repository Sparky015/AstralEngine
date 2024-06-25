//
// Created by sparky on 4/20/2024.
//

#pragma once

#include "Ayla/Events/Event.h"

namespace Ayla::Events {

    /**
 * KeyEvent class used to represent and manage keyboard events
 */
    class KeyEvent : public Event {
    public:
        /**
         * Get Events Category Flags.
         *
         * @return the InputCategory.
         */
        int getEventCategoryFlags() override {
            return (InputCategory);
        }

        /**
         * Get Events Type.
         *
         * @return the MOUSE_BUTTON_RELEASED.
         */
        int getEventType() override {
            return MOUSE_BUTTON_RELEASED;
        }
    };

    /**
     * KeyPressedEvent class to represent and manage the key pressed events
     */
    class KeyPressedEvent : public KeyEvent {
    public:
        /**
         * Construct a new Key Pressed Events object
         *
         * @param keycode // Code of the key
         */
        explicit KeyPressedEvent(int keycode) : m_keycode(keycode) {}

        /**
         * Get Events Category Flags.
         *
         * @return the KeyboardCategory and InputCategory.
         */
        int getEventCategoryFlags() override {
            return (KeyboardCategory | InputCategory);
        }

        /**
         * Get Events Type.
         *
         * @return the KEY_PRESSED.
         */
        int getEventType() override {
            return KEY_PRESSED;
        }

        /**
         * Get Key code.
         *
         * @return the key code
         */
        int getKeycode() const { return m_keycode; }

    private:
        int m_keycode;
    };

    /**
     * KeyPressedRepeatingEvent class to represent and manage keys that are continuously pressed
     */
    class KeyPressedRepeatingEvent : public KeyEvent {
    public:
        /**
         * Construct a new Key Pressed Repeating Events object
         *
         * @param keycode // Code of the key
         */
        KeyPressedRepeatingEvent(int keycode) : m_keycode(keycode) {}

        /**
         * Get Events Category Flags.
         *
         * @return the KeyboardCategory and InputCategory.
         */
        int getEventCategoryFlags() override {
            return (KeyboardCategory | InputCategory);
        }

        /**
         * Get Events Type.
         *
         * @return the KEY_PRESSED_REPEATING.
         */
        int getEventType() override {
            return KEY_PRESSED_REPEATING;
        }

        /**
         * Get Key code.
         *
         * @return the key code
         */
        int getKeycode() const { return m_keycode; }

    private:
        int m_keycode;
    };

    /**
     * KeyReleasedEvent class to represent and manage the key release events
     */
    class KeyReleasedEvent : public KeyEvent {
    public:
        /**
         * Construct a new Key Released Events object
         *
         * @param keycode // Code of the key
         */
        KeyReleasedEvent(int keycode) : m_keycode(keycode) {}

        /**
         * Get Events Category Flags.
         *
         * @return the KeyboardCategory and InputCategory.
         */
        int getEventCategoryFlags() override {
            return (KeyboardCategory | InputCategory);
        }

        /**
         * Get Events Type.
         *
         * @return the KEY_RELEASED.
         */
        int getEventType() override {
            return KEY_RELEASED;
        }

        /**
         * Get Key code.
         *
         * @return the key code
         */
        int getKeycode() const { return m_keycode; }

    private:
        int m_keycode;
    };



}; // Ayla
