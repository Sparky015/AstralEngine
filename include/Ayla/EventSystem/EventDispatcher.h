//
// Created by Andrew Fagan on 4/17/24.
//
#pragma once

#include <Ayla/EventSystem/Event.h>
#include <queue>

namespace Ayla{

    class EventDispatcher {
    public:

    private:
        Event& takeEventFromQueue(){
            Event& event = eventQueue.front();
            eventQueue.pop();
            return event;
        }

        void addEventToQueue(Event& event){
            eventQueue.push(event);
        }

        std::queue<Event> eventQueue;
    };
}
