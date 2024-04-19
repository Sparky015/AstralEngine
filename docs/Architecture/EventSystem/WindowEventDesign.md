# Window Event System Design

### The Plan

I need to connect the window and its assortment of different events to the application.
To do this, I will have a callback listener function in the Application class, which will
be set via a function of the window class. From then on, whenever I take events from
the window, I can create an event for the corresponding window event, and
make a function call (via the callback function that was set by the application class 
(Basically a callback function for the dispatcher)) which will pass the event i just made
in the GenericWindow class. That way, I can pass specific events to the Application and
the application can handle filtering of the events to handle what it wants.

When it comes to buffering events, instead of passing an event to a callback function,
I can pass it into a buffer to be processed at a later time. At that later time, the events
will be pushed to the dispatcher callback function one by one at which dispatcher will 
look at what type the event was and push the event (which holds the data) along to the
corresponding event callback function.