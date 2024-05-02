//
// Created by sparky on 4/20/2024.
//

#pragma once
#include "Ayla/EventSystem/Event.h"
namespace Ayla {

    /**
 * @class MouseButtonEvent
 * @brief Abstract class that represents a mouse button event.
 * @details This class is derived from the Event class.
 */
    class MouseButtonEvent : public Event {
    public:
        /**
         * @brief Gets the event category flags for a mouse button event.
         * @return Returns the bitwise OR of MouseButtonCategory, MouseCategory, and InputCategory.
         */
        int getEventCategoryFlags() override {
            return (MouseButtonCategory | MouseCategory | InputCategory);
        }
    };


/**
 * @class MouseButtonReleaseEvent
 * @brief Class that represents a mouse button release event.
 * @details This class is derived from the MouseButtonEvent class.
 */
    class MouseButtonReleaseEvent : public MouseButtonEvent {
    public:
        /**
         * @brief Constructs a new MouseButtonReleaseEvent.
         * @param button The button that was released.
         */
        MouseButtonReleaseEvent(int button) : m_button(button) {}

        /**
         * @brief Gets the event category flags for a mouse button release event.
         * @return Returns the bitwise OR of MouseButtonCategory, MouseCategory, and InputCategory.
         */
        int getEventCategoryFlags() override {
            return (MouseButtonCategory | MouseCategory | InputCategory);
        }

        /**
         * @brief Gets the event type for a mouse button release event.
         * @return Returns MOUSE_BUTTON_RELEASED.
         */
        int getEventType() override {
            return MOUSE_BUTTON_RELEASED;
        }

        /**
         * @brief Gets the button that was released for this event.
         * @return Returns the button that was released.
         */
        int getButton() const { return m_button; }

    private:
        int m_button;
    };

/**
 * @class MouseButtonPressEvent
 * @brief Class that represents a mouse button press event.
 * @details This class is derived from the MouseButtonEvent class.
 */
    class MouseButtonPressEvent : public MouseButtonEvent {
    public:
        /**
         * @brief Constructs a new MouseButtonPressEvent.
         * @param button The button that was pressed.
         */
        MouseButtonPressEvent(int button) : m_button(button) {}

        /**
         * @brief Gets the event category flags for a mouse button press event.
         * @return Returns the bitwise OR of MouseButtonCategory, MouseCategory, and InputCategory.
         */
        int getEventCategoryFlags() override {
            return (MouseButtonCategory | MouseCategory | InputCategory);
        }

        /**
         * @brief Gets the event type for a mouse button press event.
         * @return Returns MOUSE_BUTTON_PRESSED.
         */
        int getEventType() override {
            return MOUSE_BUTTON_PRESSED;
        }

        /**
         * @brief Gets the button that was pressed for this event.
         * @return Returns the button that was pressed.
         */
        int getButton() const { return m_button; }

    private:
        int m_button;
    };

/**
 * @class MouseScrollEvent
 * @brief Class that represents a mouse scroll event.
 * @details This class is derived from the Event class.
 */
    class MouseScrollEvent : public Event {
    public:
        /**
         * @brief Constructs a new MouseScrollEvent.
         * @param xoffset The x-offset of the scroll event.
         * @param yoffset The y-offset of the scroll event.
         */
        MouseScrollEvent(double xoffset, double yoffset) : m_xoffset(xoffset), m_yoffset(yoffset) {}

        /**
         * @brief Gets the event category flags for a mouse scroll event.
         * @return Returns the bitwise OR of MouseCategory and InputCategory.
         */
        int getEventCategoryFlags() override {
            return (MouseCategory | InputCategory);
        }

        /**
         * @brief Gets the event type for a mouse scroll event.
         * @return Returns MOUSE_SCROLLED.
         */
        int getEventType() override {
            return MOUSE_SCROLLED;
        }

        /**
         * @brief Gets the x-offset for this scroll event.
         * @return Returns the x-offset.
         */
        double getXOffset() const { return m_xoffset; }

        /**
         * @brief Gets the y-offset for this scroll event.
         * @return Returns the y-offset.
         */
        double getYOffset() const { return m_yoffset; }

    private:
        double m_xoffset;
        double m_yoffset;
    };

/**
 * @class MouseMovedEvent
 * @brief Class that represents a mouse move event.
 * @details This class is derived from the Event class.
 */
    class MouseMovedEvent : public Event {
    public:
        /**
         * @brief Constructs a new MouseMovedEvent.
         * @param xpos The x-position of the mouse cursor.
         * @param ypos The y-position of the mouse cursor.
         */
        MouseMovedEvent(double xpos, double ypos) : m_xpos(xpos), m_ypos(ypos) {}

        /**
         * @brief Gets the event category flags for a mouse move event.
         * @return Returns the bitwise OR of MouseCategory and InputCategory.
         */
        int getEventCategoryFlags() override {
            return (MouseCategory | InputCategory);
        }

        /**
         * @brief Gets the event type for a mouse move event.
         * @return Returns MOUSE_CURSOR_MOVED.
         */
        int getEventType() override {
            return MOUSE_CURSOR_MOVED;
        }

        /**
         * @brief Gets the x-position of the mouse cursor for this event.
         * @return Returns the x-position.
         */
        double getXPos() const { return m_xpos; }

        /**
         * @brief Gets the y-position of the mouse cursor for this event.
         * @return Returns the y-position.
         */
        double getYPos() const { return m_ypos; }

    private:
        double m_xpos;
        double m_ypos;
    };


} // Ayla
