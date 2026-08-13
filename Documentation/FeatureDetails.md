### Feature Details

---

#### Physically-Based Vulkan HDR Renderer

Created a Vulkan based renderer that uses the Cook-Torrence BRDF to implement physically based rendering as well as Image
Based Lighting for the environment. The renderer supports two modes, forward and deferred. The forward renderer consists
of a pre depth pass, cascaded shadow map pass, lighting pass, environment map pass, tone mapping pass, and the deferred renderer consists of a
geometry pass, cascaded shadow map pass, lighting pass, environment map pass, and tone mapping pass. Forward uses MSAA x4 while
deferred does not have AA at the moment (looking into TAA in the future). Additionally, both paths use compute shaders to calculate irradiance
and prefiltered environment maps one time for each environment map that is set.

#### Frame/Render Graph

Designed and implemented Frame/Render Graph to manage render pass resources and execution as well as memory barriers for
synchronization. The Frame Graph allows the user to define any render passes and attachment specs for each render pass, and
the Frame Graph will create all the necessary textures, render passes, frame buffers, and descriptor sets for each render pass,
and facilitate the synchronization needed between render passes. It also culls any render passes that don't contribute
to the final output image. The user can also recreate the Render Graph as many times as they want during rendering
and the render graph will manage the creation of new resources the user requested as well as making sure the resources
being used on the GPU do not get deleted.

#### Renderer Hardware Interface (RHI)

Created an abstraction layer to manage renderer API usage in order to support multiple rendering APIs in the future
(looking into implemented DX12 in the future) and allow for easy API usage for higher level systems while allowing for
switching APIs without any diverging code in the high level systems.

#### Editor

Built an editor that I can compose new game scenes with and allow for saving scenes and switching to different scenes without using a
different executable. Users can compose scenes by adding and removing entities, adding components to entities and changing
entity component data, changing environment settings like the environment map, exposure and ambient light modifier, changing
camera properties, and more!

#### Scene System

Designed and implemented a scene system that will allow users to compose scenes with entities, environment maps, and more
and save the scene to a file and load scenes from files. Right now it is loading whole scenes at a time and storing them
in the asset cache, but in the future, I will implement a way to unload scenes from the asset cache.

#### Asset Manager

Created an asset manager that can load in assets from files and cache their data to improve loading times
and memory efficiency. Currently, the asset loads are single threaded, but in the future, I want to look into
multithreaded asset loading to improve loading times as well as not blocking the main thread in order to have a
smooth UI/UX.

#### Academy Color Encoding System (ACES) Color Workflow

ACES 2.0 has been implemented for high quality color management, resulting in more vibrant and accurate colors.
All rendering is done in the ACEScg space with the ACES filmic look being applied. For the input display transform, all
color inputs being converted from sRGB to AP1 primaries to place them in the ACEScg space. After rendering is done,
I use a OCIO baked lut that contains the Reference Rendering Transform and the Output Display Transform which converts
the final colors from the ACEScg space to the sRGB space with gamma applied while also applying tone mapping for standard
dynamic range displays.

#### Custom Allocators

This includes tailored allocators for the engine to help reduce allocations where it is possible and improve performance
when the situation allows for it.

A list of the custom allocators follows:
TLSF allocator, linear allocator, slab allocator, pool allocator, stack allocator, custom alignment allocator, and ring buffer/allocator

You can find more detailed information about this (including the why's) [here](Documentation/OlderDocs/Astral-Runtime/Core/Memory%20Allocators/Information.md).

#### Memory Profiling Tool Suite

This includes real time memory allocation stats, a scene-based memory profiling with file exports and visualizer tool,
and scope-based allocation profiling tool.

You can find more detailed information about this (including the why's) [here](Documentation/OlderDocs/Astral-Runtime/Profiling%20Tools/Memory%20Tracking%20&%20Visualization/Information.md).

- Note that this is only available in debug builds, and that I am still polishing and optimizing this feature, but it is functionally done.

#### Visual Scope Profiler

This provides the user a macro to profile a scope to know how long it takes to complete as well as how many allocations
took place in the scope. It then outputs this data to a json file that can be loaded into Chrome's trace tool or perfetto's
trace tool to view visually.

