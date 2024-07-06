# Window Events 

### The Plan

Each implementation of the Window class will handle the events for the window. The application will have an update function
that will dispatch all the events that it receives. Each Window class implementation has to implement a SetCallback function that 
the Window can call to pass events to the dispatcher.

