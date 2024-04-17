# Structuring Conventions

### The Plan

* Use a function that abstracts the implementation from the actual use. That way I can change the implementation without
having to change every single occurrence. That also means that the data handling and storing will need to be isolated from the API
and be kept in its own little unit/subsystem/class.

###

* Each subsystem will maintain its own API that other subsystems will use (probably through events?)
* All major talking between systems will need its own event? (maybe?)

###

#### When adding new functionalities:
* Make it auto-testable
* Make everything be able to operate by on its own
* Ask how expandable the functionality is
* Ask how modular is the functionality can it be taken and plugged into a different game and work
* Is it dependent on something else outside the function to work?

### The Engine API

Expose certain functions that allow functionality for the system or subsystem. Alya_API will be the umbrella API from which the engine will be ran from.
Then maybe if you need a certain system, you can include the header for that system's API.

#### The Ayla API

Use the macro AYLA_API in order to include a function or class in the overall API.
That way it is visible to the user.