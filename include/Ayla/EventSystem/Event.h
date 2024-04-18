//
// Created by Andrew Fagan on 3/27/24.
//

#pragma once

#include <vector>
#include <cstdio>

namespace Ayla{


    enum EventTypes {NONE, INPUT_EVENT};

    //
    // Summary:
    //  Event is a base class that is used to manage events

    class Event {
    public:

        //TODO: Add an Event dispatcher
        //TODO: Add filtering in the event dispatcher for certain Event types

        // The function pointer signature that needs to be adhered when subscribing to events
        typedef void (*SubscriberFunctionPointer)(Event&);
        struct EmptyArgs{};

        //
        // Summary:
        //     Raises the event which pushes the event to the event dispatcher.
        void raiseEventToDispatcher() const {
            // eventDispatcher.addEvent(this);  From dispatcher then take in a reference to an Event type.
        }

        // TODO: maybe make this some sort of private function but availible to event dispatcher
        // Summary:
        //     Executes all the listener functions of the event
        // Parameter:
        //  eventArgs:
        //      The event data that will be passed along to the subscriber functions
        void raiseEvent(Event& eventArgs) const {
            executeAllSubscriberFunctions(eventArgs);
        }


        //
        // Summary:
        //      Subscribes a function pointer to the event
        // Parameters:
        //  subscriberFunction:
        //      The pointer to the function being subscribed (which also adheres to the
        //      SubscriberFunctionPointer signature).
        void subscribe(const SubscriberFunctionPointer subscriberFunction) {
            subscriberVector.push_back(subscriberFunction);
        }

        // Like 'subscribe' but as an operator (like c#)
        void operator+=(SubscriberFunctionPointer subscriberFunction){
            subscriberVector.push_back(subscriberFunction);
        };

        //
        // Summary:
        //      Unsubscribes a function pointer from the event
        // Parameters:
        //  subscriberFunction:
        //      The pointer to the function being unsubscribed (which also adheres to the
        //      SubscriberFunctionPointer signature).
        void unsubscribe(const SubscriberFunctionPointer subscriberFunction) {
            subscriberVector.erase(std::remove(subscriberVector.begin(), subscriberVector.end(), subscriberFunction), subscriberVector.end());
        }

        // Like 'unsubscribe' but as an operator (like c#)
        void operator-=(SubscriberFunctionPointer subscriberFunction){
            subscriberVector.erase(std::remove(subscriberVector.begin(), subscriberVector.end(), subscriberFunction), subscriberVector.end());
        };

        virtual int getEventType() { return NONE;}



    private:

        std::vector<SubscriberFunctionPointer> subscriberVector;

        //
        // Summary:
        //      Runs all the functions that are subscribed to the event
        // Parameter:
        //  eventArgs:
        //      The event data that will be passed along to the subscriber functions
        void executeAllSubscriberFunctions(Event& eventArgs) const {
            for (const SubscriberFunctionPointer function_pointer : subscriberVector) {
                function_pointer(eventArgs);
            }
        }
    };
}




