//
// Created by Andrew Fagan on 3/27/24.
//

#pragma once

#include <vector>
#include <cstdio>

namespace Events{

    struct EmptyArgs{};

    //
    // Summary:
    //  EventHandler is a class that is used to manage events
    template<typename... EventArgs>
    class EventHandler {
    public:
        // The function pointer signature that needs to be adhered when subscribing to events
        typedef void (*SubscriberFunctionPointer)(EventArgs...);
        struct EmptyArgs{};
        //
        // Summary:
        //     Raises the event
        // Parameter:
        //  eventArgs:
        //      The event data that will be passed along to the subscriber functions
        void raiseEvent(EventArgs... eventArgs) const {
            executeAllSubscriberFunctions(eventArgs...);
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

    private:

        std::vector<SubscriberFunctionPointer> subscriberVector;

        //
        // Summary:
        //      Runs all the functions that are subscribed to the event
        // Parameter:
        //  eventArgs:
        //      The event data that will be passed along to the subscriber functions
        void executeAllSubscriberFunctions(EventArgs... eventArgs) const {
            for (const SubscriberFunctionPointer function_pointer : subscriberVector) {
                function_pointer(eventArgs...);
            }
        }
    };
}

