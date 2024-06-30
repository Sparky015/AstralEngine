# Event Dispatcher Planning

The event dispatcher will be the main callback to where events will be passed to. Events from this function then filter out events to be passed down or handled.
( Maybe each subsystem will have its own dispatcher so they can be chained down the line when they pertain to certain systems. Also, I could have like a filter function
that can be passed into the dispatcher to define how filtering will work.)