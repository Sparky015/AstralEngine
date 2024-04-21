//
// Created by sparky on 4/20/2024.
//
//class Event {
//public:
//
//    //TODO: Add an Event dispatcher
//    //TODO: Add filtering in the event dispatcher for certain Event types
//
//    // The function pointer signature that needs to be adhered when subscribing to events
//    typedef void (*SubscriberFunctionPointer)(Event&);
//    struct EmptyArgs{};
//
//    //
//    // Summary:
//    //     Raises the event which pushes the event to the event dispatcher.
//    void raiseEventToDispatcher();
//
//
//    // Summary:
//    //     Executes all the listener functions of the event
//    // Parameter:
//    //  eventArgs:
//    //      The event data that will be passed along to the subscriber functions
//    void raiseEvent(Event& eventArgs);
//
//
//    //
//    // Summary:
//    //      Subscribes a function pointer to the event
//    // Parameters:
//    //  subscriberFunction:
//    //      The pointer to the function being subscribed (which also adheres to the
//    //      SubscriberFunctionPointer signature).
//    void subscribe(SubscriberFunctionPointer subscriberFunction);
//
//    // Like 'subscribe' but as an operator (like c#)
//    void operator+=(SubscriberFunctionPointer subscriberFunction);
//
//    //
//    // Summary:
//    //      Unsubscribes a function pointer from the event
//    // Parameters:
//    //  subscriberFunction:
//    //      The pointer to the function being unsubscribed (which also adheres to the
//    //      SubscriberFunctionPointer signature).
//    void unsubscribe(SubscriberFunctionPointer subscriberFunction);
//
//    // Like 'unsubscribe' but as an operator (like c#)
//    void operator-=(SubscriberFunctionPointer subscriberFunction);
//
//    virtual int getEventType();
//
//protected:
//    bool _handled = false;
//
//private:
//
//    std::vector<SubscriberFunctionPointer> subscriberVector;
//
//    //
//    // Summary:
//    //      Runs all the functions that are subscribed to the event
//    // Parameter:
//    //  eventArgs:
//    //      The event data that will be passed along to the subscriber functions
//    void executeAllSubscriberFunctions(Event& eventArgs);
//
//};
