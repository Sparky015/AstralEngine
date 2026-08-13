#### Render Graph Problem Analysis

What am I working to fix?
- I need a way to easily create new render passes without having to manually create all the resources, framebuffers, render passes,
descriptor sets and memory barriers.

Important: Don't solve problems I don't have. Trying to predict future problems and
trying to solve them just makes more problems and complicates things.

Context:

Why am I solving this problem?
- I am solving this problem to improve development speed and iteration times by automating the creation of textures, 
framebuffers, render passes, descriptor sets and memory barriers for every render pass.

Identify and Balance Constraints:

How much research do I have to do on this?
- Not much beyond my initial research. The concept of the render graph is simple, but actually implementing it is the 
hard part.

What is the performance budget?
- There isn't a performance budget necessarily, but I should not be noticing render graph commands taking up time when
profiling rendering code.

What is the latency/overhead requirements?
- There should not be any noticeable latency.

Will this be handled differently in different builds (Debug vs Release vs Dist)?
- There should be no difference with different builds in terms of behavior.


Value:

Identify specific, concrete, measurable value.
- The value in the render graph is being able to quickly develop new render passes without the time-consuming resource
set up. It improves development time and iteration time. Another big plus is that you can modify rendering set up easily.
Since the resource creation is automated, you can easily just modify the rendering path by creating a new render graph 
and everything updates. For example, you can change the texture quality for shadow maps at runtime or change the rendering
path from forward to deferred at runtime just by recreating the render graph.

How much time is this problem worth?
- (future me, I think I spent around 1-2 weeks creating the initial implementation. It was extremely worth it. It allowed
me to quickly implement new rendering techniques and focus on the rendering techniques rather than time-consuming manual 
resource creation and memory synchronization)

Cost:

What is the cost of solving the problem?
- Predicted development time cost: (future me, I think I spent around 1-2 weeks creating the initial implementation)
- Maintenance cost: I am going to add new features as needed to support new rendering techniques in the future. 

Platform:

How will this handle the platform targets?
- This wouldn't involve any platform specific code as it operates over the RHI.

How will this interact with shared resources (RAM, SSD)?
- The render graph holds handles to all the resources it creates to manage the lifetime of those resources. This ensures
that the resources are not prematurely destroyed while being used by the GPU.

Data:

What is read and written over time? What is the access pattern?
- After the initial render graph build (built once and kept for many frames until a change is needed), nothing is being written and only reads the read input attachments of each render pass
for transitioning memory layouts. 


Possible Future Features:
1. Adding support for buffers as a render pass resource 
2. Adding support for texture aliasing