//
// Created by Andrew Fagan on 4/17/24.
//
#pragma once

#include "Ayla/EventSystem/Event.h"


namespace Ayla{

    class EventDispatcher {
    public:
        static std::shared_ptr<Event> takeEventFromQueue();
        static void addEventToQueue(std::shared_ptr<Event> event);

    private:
        static std::queue<std::shared_ptr<Event>> eventQueue;
    };
}
