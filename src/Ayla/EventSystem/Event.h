//
// Created by Andrew Fagan on 3/27/24.
//

#pragma once
#include "Ayla/aypch.h"

namespace Ayla{

    enum EventTypes {NONE,
            WINDOW_CLOSE, WINDOW_RESIZE, WINDOW_FOCUS, WINDOW_LOST_FOCUS, WINDOW_MOVED,
            APP_TICK, APP_UPDATE, APP_RENDER,
            KEY_PRESSED, KEY_RELEASED,
            MOUSE_BUTTON_PRESSED, MOUSE_BUTTON_RELEASED, MOUSE_SCROLLED
    };

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
        void raiseEventToDispatcher();


        // Summary:
        //     Executes all the listener functions of the event
        // Parameter:
        //  eventArgs:
        //      The event data that will be passed along to the subscriber functions
        void raiseEvent(Event& eventArgs);


        //
        // Summary:
        //      Subscribes a function pointer to the event
        // Parameters:
        //  subscriberFunction:
        //      The pointer to the function being subscribed (which also adheres to the
        //      SubscriberFunctionPointer signature).
        void subscribe(SubscriberFunctionPointer subscriberFunction);

        // Like 'subscribe' but as an operator (like c#)
        void operator+=(SubscriberFunctionPointer subscriberFunction);

        //
        // Summary:
        //      Unsubscribes a function pointer from the event
        // Parameters:
        //  subscriberFunction:
        //      The pointer to the function being unsubscribed (which also adheres to the
        //      SubscriberFunctionPointer signature).
        void unsubscribe(SubscriberFunctionPointer subscriberFunction);

        // Like 'unsubscribe' but as an operator (like c#)
        void operator-=(SubscriberFunctionPointer subscriberFunction);

        virtual int getEventType();



    private:

        std::vector<SubscriberFunctionPointer> subscriberVector;

        //
        // Summary:
        //      Runs all the functions that are subscribed to the event
        // Parameter:
        //  eventArgs:
        //      The event data that will be passed along to the subscriber functions
        void executeAllSubscriberFunctions(Event& eventArgs);

    };


    int PollEvent(Event&);

}




