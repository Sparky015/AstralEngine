//
// Created by Andrew Fagan on 4/18/24.
//
#include "Ayla/Events/EventDispatcher.h"

namespace Ayla::Events {

        // TODO: Set callbacks for the dispatcher (Tie to Application??)

// PUBLIC

        void EventDispatcher::addEventToQueue(IEvent& event){
            //eventQueue.push(std::make_shared<IEvent>(std::move(event)));
        }

        void EventDispatcher::dispatchAllEvents() {
            for (int element = 0; element < eventQueue.size(); element++){
                dispatchEvent( takeEventFromQueue());
            }
        }

        void EventDispatcher::dispatchBlockingEvent(IEvent& dispatchingEvent) {

            // Filter out event through layers
        }

// PRIVATE

        std::shared_ptr<IEvent> EventDispatcher::takeEventFromQueue(){
            if (eventQueue.empty()) { return nullptr; }
            std::shared_ptr<IEvent> event = std::move(eventQueue.front());
            eventQueue.pop();
            return event;
        }

        void EventDispatcher::dispatchEvent(const std::shared_ptr<IEvent>& dispatchingEventPtr) {
            //IEvent event = std::move(*dispatchingEventPtr);
            // Filter out event through layers


        }

        int EventDispatcher::PollEvent(IEvent& event){
            auto eventPtr = EventDispatcher::takeEventFromQueue();
            if (eventPtr == nullptr) {return 0;}

            event = std::move(*(eventPtr));
            return 1;
        }

        std::queue<std::shared_ptr<IEvent>> EventDispatcher::eventQueue;

};