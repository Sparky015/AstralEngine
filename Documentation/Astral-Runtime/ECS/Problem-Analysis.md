#### ECS Problem Analysis

What am I working to fix?
- I need a way to store game objects and their data that can be read from and acted upon

Important: Don't solve problems I don't have. Trying to predict future problems and 
trying to solve them just makes more problems and complicates things.

Context:

Why am I solving this problem?
- Storing game object data is necessary to be able to add objects into a scene. I can't avoid this.

Identify and Balance Constraints:

How much time to I have to make this?
- Two weeks at the max

How much research do I have to do on this?
- A few days of research is necessary to be able to implement the ECS efficiently on a technical basis. I am already
familiar with the concept of an ECS.

How much memory can this take up?
- The ECS itself should not take up that much memory compared to other systems like the asset manager. It should be
using asset handles from the asset manager to avoid storing duplicates of asset data. It also means that the ECS itself
won't be storing too much data. Of course the ECS memory usage will be variable and will depend on how many entities are
created. The components themselves should have a small memory footprint to be able to fill as many components in the cache
line as possible.

What is the performance budget?
- I am looking to have an efficient ECS implementation but not the fastest that it could be. I am more constrained on time
and don't necessarily NEED to have a highly optimized ECS because I don't plan on having too many entities at the moment.
Of course this doesn't mean I am not going to make an efficient ECS. This implementation will not be even close to a bottleneck 
in terms of speed. Reading from the ECS should have as small as overhead as possible. It is hard to give an exact number, but
I plan to support at least 10,000 entities with no issues with performance. 

What is the latency/overhead requirements?
- The reading of component views should have as little overhead as possible. Removing should create practically zero overhead.
All memory initialization should be done during engine initialization and not on the addition of a new component type. 

Will this be handled differently in different builds (Debug vs Release vs Dist)?
- There should be no difference with different builds in terms of behavior.

What are the inputs and the outputs?
- The input is component data and the output is an optimized storage of the components that can be efficiently iterated over.  

Value:

Identify specific, concrete, measurable value.
- The value of the ECS is that it allows for optimized and efficient data transformation of all the game objects in a scene.
Storing all the objects in one place also allows for easily integrating the ECS into the editor for modifying entity data.

How much time is this problem worth?
- The problem is definitely worth the two weeks that I plan to spend on it. It is probably worth more time than that, but
I have other more interesting things I want to get to by the end of the summer, so I need to quickly solve this.

What is the value of solving this problem?
- The value in solving this problem is that I can store entity data efficiently while at the same time giving me a path
to integrate viewing and modifying entity data into the editor. It also allows me to learn more about the ECS, optimization,
templated metaprogramming, and whatever other issues I encounter in the process. 

Cost:

What is the cost of solving the problem?
- Predicted development time cost: 2 weeks (hopefully)
- Maintenance cost: Eventually, the ECS will have to be optimized further as I don't plan to go too deep, so there's that.

Platform:

How will this handle the platform targets?
- This shouldn't involve any OS specific code, however I should keep in mind of the CPU cache line size and potential
optimizations around that.

How will this interact with shared resources (RAM, SSD)?
- The ECS will be only available from the main thread, so there should not be any contesting over trying to read the
entity data (unless the situation changes down the line).

Data:

What is read and written over time? What is the access pattern?
- Data will mostly be read over the course of the program in the form of systems acting on all the components of a certain 
type. Most of the writing will be done when the user is changing the data in the editor, so not at the runtime. 
However, some systems might have interactions between two components, so that the value of one component changes the 
data value of the other which means there could be some writing to every entity with a certain set of components. Some systems
might read multiple components of the same entity. The typical access pattern would involve a system requesting a view of
a certain component or two and iterating over the component view and reading the data of each component. 

When is data accessed statistically relative to other data?
- Many systems will access two components at the same time, but it depends on the system. Because of this, it might be
good to check out changing the order of each system in terms of execution because some of the systems will have overlap
in terms of the components used, so some of the previously read component data might still be cached if there aren't
too many entities.

What data values are common? Outliers?
- The transform component is the most common component that will be reused. Some uncommon components include the camera
component or maybe some user introduced component.

