//
// Created by Andrew Fagan on 4/17/24.
//
#pragma once

#include "Ayla/EventSystem/Event.h"


namespace Ayla{

    class EventDispatcher {
    public:

        static void addEventToQueue(Event&);
        void dispatchAllEvents();
        void dispatchBlockingEvent(Event&);
    private:
        static std::shared_ptr<Event> takeEventFromQueue();
        void dispatchEvent(const std::shared_ptr<Event>&);
        int PollEvent(Event&);
        static std::queue<std::shared_ptr<Event>> eventQueue;
    };
}
