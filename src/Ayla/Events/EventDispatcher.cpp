//
// Created by Andrew Fagan on 4/18/24.
//
#include "Ayla/Events/EventDispatcher.h"

namespace Ayla {

        // TODO: Set callbacks for the dispatcher (Tie to Application??)

// PUBLIC

        void EventDispatcher::addEventToQueue(Event& event){
            eventQueue.push(std::make_shared<Event>(std::move(event)));
        }

        void EventDispatcher::dispatchAllEvents() {
            for (int element = 0; element < eventQueue.size(); element++){
                dispatchEvent( takeEventFromQueue());
            }
        }

        void EventDispatcher::dispatchBlockingEvent(Event& dispatchingEvent) {
            Event event = std::move(dispatchingEvent);
            // Filter out event through layers
        }

// PRIVATE

        std::shared_ptr<Event> EventDispatcher::takeEventFromQueue(){
            if (eventQueue.empty()) { return nullptr; }
            std::shared_ptr<Event> event = std::move(eventQueue.front());
            eventQueue.pop();
            return event;
        }

        void EventDispatcher::dispatchEvent(const std::shared_ptr<Event>& dispatchingEventPtr) {
            Event event = std::move(*dispatchingEventPtr);
            // Filter out event through layers


        }

        int EventDispatcher::PollEvent(Event& event){
            auto eventPtr = EventDispatcher::takeEventFromQueue();
            if (eventPtr == nullptr) {return 0;}

            event = std::move(*(eventPtr));
            return 1;
        }

        std::queue<std::shared_ptr<Event>> EventDispatcher::eventQueue;

};