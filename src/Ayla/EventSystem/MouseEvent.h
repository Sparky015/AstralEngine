//
// Created by sparky on 4/20/2024.
//

#pragma once
#include <Ayla/EventSystem/Event.h>
namespace Ayla {

    class MouseButtonEvent : Event {

    public:
        float getPosX() const { return m_mouseX; }
        float getPosY() const { return m_mouseY; }

        virtual int getEventCategory() override {
            return (MouseButtonCategory | MouseCategory | InputCategory);
        }

    private:
        float m_mouseX;
        float m_mouseY;
    };


    class MouseButtonReleaseEvent : MouseButtonEvent{
    public:
        virtual int getEventCategory() override {
            return (MouseButtonCategory | MouseCategory | InputCategory);
        }
        virtual int getEventType() override {
            return MOUSE_BUTTON_RELEASED;
        }
    };

    class MouseButtonPressEvent : MouseButtonEvent {
    public:
        virtual int getEventCategory() override {
            return (MouseButtonCategory | MouseCategory | InputCategory);
        }
        virtual int getEventType() override{
            return MOUSE_BUTTON_PRESSED;
        }
    };

    class MouseScrollEvent : Event {
    public:
        virtual int getEventCategory() override {
            return (MouseCategory | InputCategory);
        }
        virtual int getEventType() override {
            return MOUSE_SCROLLED;
        }
    };


} // Ayla
