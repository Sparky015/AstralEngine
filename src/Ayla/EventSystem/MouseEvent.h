//
// Created by sparky on 4/20/2024.
//

#pragma once
#include <Ayla/EventSystem/Event.h>
namespace Ayla {

    class MouseButtonEvent : public Event {
    public:
        int getEventCategoryFlags() override {
            return (MouseButtonCategory | MouseCategory | InputCategory);
        }
    };


    class MouseButtonReleaseEvent : public MouseButtonEvent{
    public:
        MouseButtonReleaseEvent(int button) : m_button(button){}

        int getEventCategoryFlags() override {
            return (MouseButtonCategory | MouseCategory | InputCategory);
        }
        int getEventType() override {
            return MOUSE_BUTTON_RELEASED;
        }
        int getButton() const { return m_button;}
    private:
        int m_button;
    };

    class MouseButtonPressEvent : public MouseButtonEvent {
    public:
        MouseButtonPressEvent(int button) : m_button(button){}

        int getEventCategoryFlags() override {
            return (MouseButtonCategory | MouseCategory | InputCategory);
        }
        int getEventType() override{
            return MOUSE_BUTTON_PRESSED;
        }
        int getButton() const { return m_button;}
    private:
        int m_button;
    };

    class MouseScrollEvent : public Event {
    public:
        MouseScrollEvent(double xoffset, double yoffset) : m_xoffset(xoffset), m_yoffset(yoffset){}
        int getEventCategoryFlags() override {
            return (MouseCategory | InputCategory);
        }
        int getEventType() override {
            return MOUSE_SCROLLED;
        }

        double getXOffset() const { return m_xoffset; }
        double getYOffset() const { return m_yoffset; }


    private:
        double m_xoffset;
        double m_yoffset;
    };

    class MouseMovedEvent : public Event {
    public:
        MouseMovedEvent(double xpos, double ypos) : m_xpos(xpos), m_ypos(ypos){}
        int getEventCategoryFlags() override {
            return (MouseCategory | InputCategory);
        }

        int getEventType() override {
            return MOUSE_MOVED;
        }

        double getXPos() const { return m_xpos; }
        double getYPos() const { return m_ypos; }
    private:
        double m_xpos;
        double m_ypos;
    };


} // Ayla
