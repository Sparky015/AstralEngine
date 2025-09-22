#### Render Graph Research Analysis

A render graph (or frame graph) is a system that manages the resources and execution of render passes in a renderer.
There are many optimizations that a render graph can bring as well as things that it simplifies for the user such as
texture aliasing, async work, memory barriers, pass culling, and resource creation and management.

Pluses:
- Memory usage optimizations (texture aliasing which is the reuse of textures that render passes use in a transitory manner)
- Automated resource (textures, framebuffers, descriptor sets, etc) management
- Automated memory barrier placement (the render graph knows the whole context of the render path, so it knows where memory barriers are needed)
- Async queue/compute work (since the render graph knows the order of execution in the render path, it knows when and where barriers are needed for synchronization)

Minuses:
- Complexity for implementing and debugging render pass execution
- Less fine control over how exactly render passes and memory barriers are managed


#### Implementation Overview:

I focused on just setting up the bare-bones render graph and being able to define render passes that the render graph
can go through and build up a directed acyclic graph. After that I use topological sorting (a variation of Kahn's algorithm)
to select the order of render pass execution. For simplicity, I implemented the render graph to work for only one graphics
queue and not supporting texture aliasing or compute shaders (in the render graph) yet. I wanted to get a basic version up and running that I can use and iterate 
on without the complexities of the optimization opportunities that render graphs bring, so that's what I did. My current
implementation supports pass culling, automated memory barriers, automated resource creation, and resource management (like
resizing all textures based on the window size when the window is resized and lifetime management)


###
#### References Used:

---

- Presentation, "FrameGraph: Extensible Rendering Architecture in Frostbite" (https://gdcvault.com/play/1024612/FrameGraph-Extensible-Rendering-Architecture-in)
- Presentation, "The Road to Baldur's Gate 3" (https://youtu.be/zuDjcoabX7U?si=QUTK9fxhbwv0avxx&t=2508)
