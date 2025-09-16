#### Rendering Hardware Interface (RHI) Problem Analysis

What am I working to fix?
- I need a way to abstract graphics API usage to where I can use more than one graphics API in the future. Building
a RHI allows for convenient API usage while keeping state tracking and logic to one place in the codebase.

Important: Don't solve problems I don't have. Trying to predict future problems and
trying to solve them just makes more problems and complicates things.

Context:

Why am I solving this problem?
- I need to have this abstraction set up in the beginning, even if I only use one graphics API for now, to start to develop
the RHI into a powerful abstraction that can make writing new render passes easy and quick. An RHI would cover tons of topics
and logic from a graphics API, and it would be best to have it set up now so that I can continually add to it as needed.

Identify and Balance Constraints:

How much research do I have to do on this?
- I don't need much research to start this. I will learn as a go and make changes as I go. Building an RHI all at once would
be a massive challenge, so I am going to learn Vulkan concurrently and add things to the RHI as I need to use it with Vulkan

What is the performance budget?
- The implementation should just be a wrapper around the graphics API calls with state tracking, so the actual abstraction
should be fast and low overhead to reduce CPU overhead.

What is the latency/overhead requirements?
- The goal is to keep the overhead to a minimal, but if switching graphics APIs at runtime is necessary, virtual functions
will need to be used. This introduces a slight overhead. However, in the future, if you don't need to switch graphics APIs at runtime and
can ship using one graphics API for a build, you could use preprocessor defines to remove the inheritance set up and define an alias 
of the base class to the specific graphics API class (like Texture as an alias for VulkanTexture). This would remove the
cost of virtual functions which would remove most of the overhead from the RHI.

Will this be handled differently in different builds (Debug vs Release vs Dist)?
- There should be no difference with different builds in terms of behavior. Maybe for a Dist build, I would restrict
the build to only use one graphics API to reduce overhead in the RHI and therefore reduce any CPU overhead as I make 
10,000+ API calls every frame.


Value:

Identify specific, concrete, measurable value.
- The RHI will save me time developing new rendering passes as the RHI handles all the logic and contextual decisions
while allowing for an easy interface to use.

How much time is this problem worth?
- As much time as I need to make the RHI usable for a minimum viable product. No more than a month at once. The goal
is to do bits and pieces every once in a while as I need new features.

Cost:

What is the cost of solving the problem?
- Predicted development time cost: (me from future): it took 3 weeks to get to a textured quad) + as needed development for months (Just for Vulkan)
- Maintenance cost: As I need more and customization, I will add it to the RHI. Also, I need to add unit tests for the whole
RHI which will take a lot of time. Furthermore, it will take significant amount of time in the future when I need to add more graphics
APIs to the RHI, but this is necessary.

Platform:

How will this handle the platform targets?
- API selection will be based on which platform the Engine is running on. Preprocessor defines will be set declaring
which APIs are available based on the platform and the default API selection for the platform. For example, Vulkan is 
available for macOS (MoltenVK), Windows, and Linux. And in the future when I have other graphics APIs, Metal will be 
on only macOS and DirectX12 will be on only Windows.

How will this interact with shared resources (RAM, SSD)?
- Currently, the RHI compiles shader modules from shader source that is loaded from the SSD and cached, but besides that,
the RHI does not interact with the SSD. In RAM, RHI objects are stored using a handle system which are std::shared_ptr's.

Data:

What is read and written over time? What is the access pattern?
- With the RHI, the main thing being read and written as far as cacheing would be RHI object handles. Each RHI object mainly
just contains metadata and state tracking for the object which is not commonly accessed after initialization. I think the main bottleneck as
far as cacheing would be the RHI object instances as they are shared pointers, so they are all over memory. In the future, allocating
object instances should be from a slab allocator to keep RHI objects close together in memory. This would help with cache efficiency 
for the smaller RHI objects.


Possible Future Features:
1. Preprocessor defines to remove virtual function calls (restricts to 1 graphics API)
2. Expand to DirectX12
3. Expand to Metal
4. Move RHI object allocation to a custom allocator and ref tracking set up