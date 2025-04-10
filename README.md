# Astral Engine

---------------
Welcome to the Astral Engine project codebase!

This project is a Work-In-Progress with no official first release.

It is continuously being updated and improved as I can get to more things.

Project Lifetime: Late October 2024 - Present

In the future, Astral Engine will become a 3D engine targeting open world environments with focus on particles
and the environment, but there is a lot of stuff to do to get there.

###
### Notable Features

----

###
#### Custom Allocators

This includes tailored allocators for the engine to help reduce allocations where it is possible and improve performance
when the situation allows for it.

A list of the custom allocators follows:
Stack allocator, linear allocator, custom alignment allocator, frame allocator, double buffered allocator, pool allocator,
ring allocator/buffer, and stack-based linear allocator (plus an object pool class).

You can find more detailed information about this (including the why's) [here](Documentation/Astral-Runtime/Memory%20Allocators/Information.md).

Future allocators to be written: Slab Allocator

- Note that I am still polishing this feature, but it is functionally done.

###
#### Memory Profiling Tool Suite

This includes real time memory allocation stats, a scene-based memory profiling with file exports and visualizer tool, 
and scope-based allocation profiling tool

You can find more detailed information about this (including the why's) [here](Documentation/Astral-Runtime/Memory%20Tracking%20&%20Visualization/Information.md).

- Note that I am still polishing and optimizing this feature, but it is functionally done.

###
#### Visual Scope Profiler

This provides the user a macro to profile a scope to know how long it takes to complete as well as how many allocations
took place in the scope. It then outputs this data to a json file that can be loaded into Chrome's trace tool or perfetto's
trace tool to view visually.

###
### Work-In-Progress Features

----

* Editor (Early In-Progress)

I am working to build up an editor that I can compose new game scenes with
and allow for saving scenes and switching to different scenes without using a
different executable.

* Entity-Component-System (ECS) (70% done)


I am working to create an asset manager that can load in assets from files and cache their data to improve loading times
and memory efficiency.

* Renderer (Early In-Progress)

I am working on implementing the ability to import 3D/2D meshes. Currently, the renderer only
supports hardcoded data which ends up being quads. I am also working on setting up Vulkan for 
use later on (I am currently using OpenGL 4.1)

* Asset Manager (40% done)

I am working on writing an asset manager that can simply cache raw asset data for now.

* Scene System (Early In-Progress)

I am working on designing a scene system that will allow me to save scenes to files and load scenes from files. I am
integrating the ECS system and the asset manager to define a scene from which I can serialize and deserialize.

* Shaders and Materials (Early In-Progress)

I am working on designing an extendable material system for shaders, 
so I can add a Material component into the ECS and remove the Rendering System (ECS) calls out of the
client/user code.


###
### Screenshot of Current Engine State

---


![CurrentEngineState](Documentation/Astral-Runtime/Overall%20Engine%20Runtime/Pictures/CurrentEngineState.png)
This picture contains the Chess sample project as well as the engine debug menu. Note that the chess sample
project is not completely finished and that there is no checks or castling.

###
### Roadmap

-----

1. Entity-Component-System (ECS)
2. Asset Manager
3. Game Scenes with serialization and deserialization
4. Editor Integration
5. Native Scripting
6. Material System
7. PBR Renderer
8. Vulkan Renderer
9. GPU Particle System  <--- Current dream goal

###
### How to Build

-----

Supported Compilers: MSVC, AppleClang, and Clang
Supported Platforms: Windows and macOS    (Linux coming in the future)

Astral uses CMake for the build system.

In additional to the methods below, you can also use an IDE that supports building with CMake like CLion or Visual Studio.

Requirements include:
- git
- CMake ver. 3.28+ (Note: The below examples require cmake to be available on the command line)
- A supported C++20 compiler
- 64-bit CPU

#### Windows

1. Open a terminal at the root of the project
2. Enter the following shell code to build the project
   ```powershell
   mkdir build
   cd build
   cmake -DCMAKE_BUILD_TYPE=Release ..
   cmake --build .
   ```
   Note that the default build is release. If you want a debug build, change the Release
   to a Debug in this line -> cmake -DCMAKE_BUILD_TYPE=[Desired Build Config] ..
3. Navigate to the bin directory
   ```powershell
   cd bin
   ```
4. Choose which executable you want to run
   ```powershell
   dir
   .\[enter_executable_name].exe
   ```
   Note that to use the executables containing unit tests, you must build them as a debug build


#### MacOS

1. Open a terminal at the root of the project
2. Enter the following shell code to build the project
   ```bash
   mkdir build
   cd build
   cmake -DCMAKE_BUILD_TYPE=Release ..
   cmake --build .
   ```
   Note that the default build is release. If you want a debug build, change the Release
   to a Debug in this line -> cmake -DCMAKE_BUILD_TYPE=[Desired Build Config] .. 
3. Navigate to the bin directory
   ```bash
   cd bin
   ```
4. Choose which executable you want to run
   ```bash
   ls
   ./[enter_executable_name]
   ```
   Note that to use the executables containing unit tests, you must build them as a debug build 



###
### Branches

-----

- main --> The branch for releases   (no actual releases have happened yet)
- development -> All work is done in this branch or a branch based from it
- feature-X -> Branches for feature work to be done on
- experimental-X -> Branches for to explore the potential and limits of a new feature idea
  - experimental-vulkan -> Branch where the vulkan renderer is being explored and written and refactors to the renderer
                           structure.


###
### Dependencies and Third Party Libraries

-----

- GLFW: For a cross-platform window
- GLAD: For loading OpenGL 
- cpuinfo: For detecting the cpu name, amount of cores, and other info
- glm: For math 
- googletest: For unit testing
- imgui: For debug menus and the editor UI
- ImGuiFileDialog: For a file explorer to choose files (temporary, looking for cross-platform native file explorer library)
- ImPlot: For graphing memory profiling data
- msgpack: For serialization and deserialization
- stb_image: For loading image files
- cpptrace: For generating stacktraces (temporary, waiting for C++23 stacktraces to be implemented)


###
### Current C++ Version (C++20)

-----

This project is using C++20 currently and mainly for [[unlikely]], [[likely]], consteval, and constexpr improvements (maybe modules in the future).


I am looking to switch to C++23 when the stacktraces feature is actually implemented by
all the major compilers. Also, std::unreachable would be useful.


###
### Testing Environment

---- 

#### Tested IDEs:

macOS: CLion and Xcode     
Windows: CLion and Visual Studio   

#### Hardware used for Testing: 

macOS is tested using a MacBook M1 Pro       
Windows is tested using a PC with a Ryzen 5600X and Nvidia RTX 3070 Ti

#### Tested Compiler Versions:

MacOS: AppleClang 16.0.0, Clang 19.1.7      
Windows: MSVC 19.43

