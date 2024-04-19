//
// Created by Andrew Fagan on 4/18/24.
//
#include <Ayla/EventSystem/EventDispatcher.h>

namespace Ayla {

        std::shared_ptr<Event> EventDispatcher::takeEventFromQueue(){
            if (eventQueue.empty()) { return nullptr; }
            std::shared_ptr<Event> event = std::move(eventQueue.front());
            eventQueue.pop();
            return event;
        }

        void EventDispatcher::addEventToQueue(std::shared_ptr<Event> event){
            eventQueue.push(std::move(event));
        }

        std::queue<std::shared_ptr<Event>> EventDispatcher::eventQueue;

};