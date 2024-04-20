//
// Created by Andrew Fagan on 4/17/24.
//
#pragma once

#include "Ayla/EventSystem/Event.h"


namespace Ayla{

    class EventDispatcher {
    public:

        static void addEventToQueue(Event& event);
        void dispatchAllEvents();

    private:
        static std::shared_ptr<Event> takeEventFromQueue();
        void dispatchEvent(const std::shared_ptr<Event>&);
        int PollEvent(Event& event);
        static std::queue<std::shared_ptr<Event>> eventQueue;
    };
}
