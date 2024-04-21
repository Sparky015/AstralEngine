//
// Created by Andrew Fagan on 3/27/24.
//

#pragma once
#include "Ayla/aypch.h"

namespace Ayla{

    enum EventTypes {NONE,
            WINDOW_CLOSE, WINDOW_RESIZE, WINDOW_FOCUS, WINDOW_LOST_FOCUS, WINDOW_MOVED,
            APP_TICK, APP_UPDATE, APP_RENDER,
            KEY_PRESSED, KEY_RELEASED,
            MOUSE_BUTTON_PRESSED, MOUSE_BUTTON_RELEASED, MOUSE_SCROLLED
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

        //TODO: Add an Event dispatcher






        virtual int getEventCategory();
        virtual int getEventType();

    protected:
        bool _handled = false;

    private:




    };


    int PollEvent(Event&);

}




