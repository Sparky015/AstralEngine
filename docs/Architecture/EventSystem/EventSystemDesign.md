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
In the constructor, when an event is raised, it will pass its own event class to the dispatcher 
and then the dispatcher will call a TriggerEventFunctions of from the base event class when it wants 
to handle it


The dispatcher class will have a method to add an event to the buffer which will pass in a reference to the event class



### Implementation

Initially I previously made an Event class that had subscribe and unsubscribe functions and then a raiseEvent function which triggered the
event. I used templates in a function signature so that I could add and take away listener functions at which the template served as a 
variable event arguments field. However, this way only supported blocking events because I couldn't make a dispatcher that would be able
to store all the different Event templated types in one vector.

So I looked for a different design to support an event buffer. Initially, I thought about using inheritance to make a class to derive from an
abstract base Event class that override the adding/subtracting functions and storing of the functions so that I could specify my own function 
signature for the listener functions that got passed in to allow for differing event arguments. However, you can't have a different 
function signature for the pure virtual function you are overriding. 

Then, I came to the realization, after a long time of thinking and observing how events work from the user-end in SDL, I didn't HAVE to 
have event arguments flow through the parameter fields of the functions but I could use the base class Event to pass around the events 
to other classes/functions because it is a type that ALL the event types have. Then I could make a derived class that specified more
data that was specific to a certain event type. This way I can store all of the events no matter the type, but it can still have event data 
that differs from other event types.

For retrieving an event, I can have a function from the dispatcher that will take the next event off the queue and std::move into a
user- event via a reference.

