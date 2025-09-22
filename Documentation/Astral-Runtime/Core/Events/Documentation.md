## Event System 


The Event System is the glue connecting many of the sub systems of the engine. I implemented the Event system with a 
decoupled architecture in mind. My goal with this iteration was to avoid RTTI while keeping a simple set up for the
Event Bus storing listener callbacks. The solution I came up with was a templated global singleton for each type of event.
This avoid RTTI and made the setup really simple and easy to understand, and it meets my needs well. However, the downsides
to this design is the static lifetime of the event bus. It doesn't give me any control over the lifetime of the listeners
in still in the event bus, if any. Furthermore, the decentralized and disjointed setup makes it hard to reflect any stats
back from the Event system like number of listeners for each event type and the frequency of events being published. I think
the pros outweigh the cons for now considering the need to quickly develop minimum viable products and keep the complexity relatively
small, so that I can get to the features I want to get to before the fall. Then, after I keep minimum viable products set up
horizontally and a working build across all systems, I can start to iterate over each system and dive deeper into technical
optimizations.
