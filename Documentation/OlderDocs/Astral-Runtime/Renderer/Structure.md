
## Structure of Renderer

The Renderer is structured in such a way to allow for quick iterations with abstractions to simplify the complexity
of what actually goes on. This is accomplished through two main things, the Render Graph and the RHI. The Render Graph
simplifies a lot of the complexities with multi pass rendering. All you have to do to set up a multi pass rendering set up
is to just define the properties of the attachments created by the render pass and which attachments should be linked to the
render pass that were created by other render passes. And with just that, the Render Graph creates the actual textures for the 
attachments, the framebuffers that the write attachments are placed in, the descriptor sets for read attachments of a pass, and the
render pass objects for every render pass. Furthermore, layout transitions and memory barriers are handled by the Render Graph,
so the user doesn't need to worry about that and can focus on creating and iterating on the render passes they need faster.
Additionally, the Render Graph supports redefining the rendering path and settings dynamically, so pass definitions and settings
can be changed at runtime and reflected in the viewport right away without the user needing to edit any the code themselves.
The other big thing that simplifies complexity of the renderer for the user is the Rendering Hardware Interface (RHI). The RHI
abstracts graphics API usage into a more general API that can be used to call multiple different graphics API backends (although 
currently, I only support Vulkan as a backend). It's very useful because beyond the abstraction of the graphics APIs calls, it also
provides state tracking and metadata about objects that be queried to coordinate action in render passes or other functions.
In practice, I use a combination of the Render Graph and RHI to set up a rendering passes and acquire the swapchain image, set up pipelines, 
bind descriptor sets, record draw calls into the command buffer, and submit a command buffer to a queue to be ran on the GPU.



