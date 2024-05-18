//
// Created by Andrew Fagan on 3/27/24.
//

#pragma once
#include "Ayla/aypch.h"

namespace Ayla {


    enum EventTypes {
        NONE,
        WINDOW_CLOSE, WINDOW_RESIZE, WINDOW_GAINED_FOCUS, WINDOW_LOST_FOCUS, WINDOW_MOVED,
        APP_TICK, APP_UPDATE, APP_RENDER,
        KEY_PRESSED, KEY_RELEASED, KEY_PRESSED_REPEATING,
        MOUSE_BUTTON_PRESSED, MOUSE_BUTTON_RELEASED, MOUSE_SCROLLED, MOUSE_CURSOR_MOVED
    };


    enum EventCategory {
        None,
        ApplicationCategory = 1 << 0,
        InputCategory = 1 << 1,
        KeyboardCategory = 1 << 2,
        MouseCategory = 1 << 3, // <3
        MouseButtonCategory = 1 << 4,
    };


    class Event {
    public:

        virtual ~Event() = default;
        virtual int getEventCategoryFlags();
        virtual int getEventType();

        // Evaluates true if one of the flags match
        bool isInCategory(EventCategory category) {
            return getEventCategoryFlags() & category;
        }

        bool isInBothCategories(EventCategory category1, EventCategory category2){
            return (getEventCategoryFlags() & category1) && (getEventCategoryFlags() & category2);
        }

        // Evaluates true if all flags match
        bool hasExactCategoryMatch(EventCategory category){
            return (getEventCategoryFlags() & category) == category;
        }

        void setIsHandled(bool isHandled);
        bool isHandled();

    protected:
        bool m_isHandled = false;
    };

    int PollEvent(Event &event);

}



