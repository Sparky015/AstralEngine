//
// Created by Andrew Fagan on 3/27/24.
//

#pragma once
#include "Ayla/aypch.h"

namespace Ayla{



    enum EventTypes {NONE,
            WINDOW_CLOSE, WINDOW_RESIZE, WINDOW_GAINED_FOCUS, WINDOW_LOST_FOCUS, WINDOW_MOVED,
            APP_TICK, APP_UPDATE, APP_RENDER,
            KEY_PRESSED, KEY_RELEASED, KEY_PRESSED_REPEATING,
            MOUSE_BUTTON_PRESSED, MOUSE_BUTTON_RELEASED, MOUSE_SCROLLED, MOUSE_CURSOR_MOVED
    };


    enum EventCategory {
        None,
        ApplicationCategory = 1 < 0,
        InputCategory = 1 < 1,
        KeyboardCategory = 1 < 2,
        MouseCategory = 1 < 3, // <3
        MouseButtonCategory = 1 < 4,
    };


    class Event {
    public:
        virtual ~Event() = default;

        virtual int getEventCategoryFlags();
        virtual int getEventType();

        bool isInCategory(EventCategory category) {
            return getEventCategoryFlags() & category;
        }

        bool isHandled();
    protected:
        bool m_isHandled = false;


    };


    int PollEvent(Event&);

}




