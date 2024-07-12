//
// Created by sparky on 4/20/2024.
//

#pragma once
#include "Ayla/Events/Event.h"

namespace Ayla::Events {

    class MouseButtonEvent : public IEvent
    {
    public:

        [[nodiscard]] int8_t GetEventCategoryFlags() const override
        {
            return MOUSE_CATEGORY | INPUT_CATEGORY;
        }


        [[nodiscard]] int GetEventType() const override
        {
            return NONE;
        }

    protected:

        /** MouseButtonEvent is not meant to be created as a standalone event. */
        MouseButtonEvent() = default;
    };


    class MouseButtonReleaseEvent : public MouseButtonEvent
    {
    public:
        explicit MouseButtonReleaseEvent(int button) : m_Button(button) {}

        [[nodiscard]] int8_t GetEventCategoryFlags() const override
        {
            return MOUSE_BUTTON_CATEGORY | MOUSE_CATEGORY | INPUT_CATEGORY;
        }


        [[nodiscard]] int GetEventType() const override {
            return MOUSE_BUTTON_RELEASED;
        }


        [[nodiscard]] int GetButton() const
        {
            return m_Button;
        }

    private:
        int m_Button;
    };


    class MouseButtonPressEvent : public MouseButtonEvent
    {
    public:
        explicit MouseButtonPressEvent(int button) : m_Button(button) {}

         [[nodiscard]] int8_t GetEventCategoryFlags() const override
         {
            return MOUSE_BUTTON_CATEGORY | MOUSE_CATEGORY | INPUT_CATEGORY;
        }


        [[nodiscard]] int GetEventType() const override
        {
            return MOUSE_BUTTON_PRESSED;
        }


        [[nodiscard]] int GetButton() const
        {
            return m_Button;
        }

    private:
        int m_Button;
    };


    class MouseScrollEvent : public IEvent
    {
    public:
        MouseScrollEvent(double xoffset, double yoffset) : m_Xoffset(xoffset), m_Yoffset(yoffset) {}

        [[nodiscard]] int8_t GetEventCategoryFlags() const override
        {
            return MOUSE_CATEGORY | INPUT_CATEGORY;
        }


        [[nodiscard]] int GetEventType() const override
        {
            return MOUSE_SCROLLED;
        }


        [[nodiscard]] double GetXOffset() const
        {
            return m_Xoffset;
        }


        [[nodiscard]] double GetYOffset() const
        {
            return m_Yoffset;
        }

    private:
        double m_Xoffset;
        double m_Yoffset;
    };


    class MouseMovedEvent : public IEvent
    {
    public:
        explicit MouseMovedEvent(double xpos, double ypos) : m_Xpos(xpos), m_Ypos(ypos) {}

        [[nodiscard]] int8_t GetEventCategoryFlags() const override
        {
            return MOUSE_CATEGORY | INPUT_CATEGORY;
        }


        [[nodiscard]] int GetEventType() const override
        {
            return MOUSE_CURSOR_MOVED;
        }


        [[nodiscard]] double GetXPos() const
        {
            return m_Xpos;
        }


        [[nodiscard]] double GetYPos() const
        {
            return m_Ypos;
        }

    private:
        double m_Xpos;
        double m_Ypos;
    };

} // namespace Ayla::Events
