# Event System Design



### The Plan

There will be an Event class which will act as a base class for more specific event types to derive from. It will handle subscribing 
and unsubscribing listeners and passing event data along to the listeners.

For event dispatcher, it will take a type Event and call a GetType method which 
will relate to an enum of all the event types.
###
The event class will need a pure virtual function added for returning the event type.
( I like this one more cause the dispatcher can just call the function and compare over having to know the event type class)

OR

I could make a static int that holds the type for event type.
###
In the constructor, when an event is raised, it will pass its own event class add it to the dispatcher 
and then the dispatcher will call a TriggerEventFunctions of from the base event class when it wants 
to handle it


The dispatcher class will have a method to add an event to the buffer which will pass in a reference to the event class