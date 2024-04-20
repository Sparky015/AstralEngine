//
// Created by Andrew Fagan on 4/18/24.
//


#include "Ayla/EventSystem/Event.h"
#include "Ayla/EventSystem/EventDispatcher.h"

namespace Ayla{

    //TODO: Add an Event dispatcher
    //TODO: Add filtering in the event dispatcher for certain Event types


    //
    // Summary:
    //     Raises the event which pushes the event to the event dispatcher.
    void Event::raiseEventToDispatcher() {
        std::shared_ptr<Event> eventPtr = std::make_shared<Event>();
        EventDispatcher::addEventToQueue(eventPtr);
    }


    // TODO: maybe make this some sort of private function but available to event dispatcher
    // Summary:
    //     Executes all the listener functions of the event
    // Parameter:
    //  eventArgs:
    //      The event data that will be passed along to the subscriber functions
    void Event::raiseEvent(Event& eventArgs) {
        executeAllSubscriberFunctions(eventArgs);
    }


    //
    // Summary:
    //      Subscribes a function pointer to the event
    // Parameters:
    //  subscriberFunction:
    //      The pointer to the function being subscribed (which also adheres to the
    //      SubscriberFunctionPointer signature).
    void Event::subscribe(const SubscriberFunctionPointer subscriberFunction) {
        subscriberVector.push_back(subscriberFunction);
    }
    // Like 'subscribe' but as an operator (like c#)
    void Event::operator+=(SubscriberFunctionPointer subscriberFunction){
        subscriberVector.push_back(subscriberFunction);
    }


    //
    // Summary:
    //      Unsubscribes a function pointer from the event
    // Parameters:
    //  subscriberFunction:
    //      The pointer to the function being unsubscribed (which also adheres to the
    //      SubscriberFunctionPointer signature).
    void Event::unsubscribe(const SubscriberFunctionPointer subscriberFunction) {
        subscriberVector.erase(std::remove(subscriberVector.begin(), subscriberVector.end(), subscriberFunction), subscriberVector.end());
    }
    // Like 'unsubscribe' but as an operator (like c#)
    void Event::operator-=(SubscriberFunctionPointer subscriberFunction){
        subscriberVector.erase(std::remove(subscriberVector.begin(), subscriberVector.end(), subscriberFunction), subscriberVector.end());
    }



   int Event::getEventType() { return NONE;}


    //
    // Summary:
    //      Runs all the functions that are subscribed to the event
    // Parameter:
    //  eventArgs:
    //      The event data that will be passed along to the subscriber functions
    void Event::executeAllSubscriberFunctions(Event& eventArgs) {
        for (const SubscriberFunctionPointer function_pointer : subscriberVector) {
            function_pointer(eventArgs);
        }
    }




}