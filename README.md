# Astral Engine

[![CI Build](https://github.com/Sparky015/AstralEngine/actions/workflows/cmake-multi-platform-development-ci.yml/badge.svg)](https://github.com/Sparky015/AstralEngine/actions/workflows/cmake-multi-platform-development-ci.yml)

---------------
Welcome to the Astral project codebase!

![PancakeWithSmore](Documentation/Assets/Videos/PancakesWithSmore_Short.gif)

### Notable Features


#### Rendering Engine
* Physically-Based HDR Renderer
* Frame/Render Graph
* Vulkan and Metal 4 Support with a Renderer Hardware Interface (RHI) 
* Multithreaded Asset Manager
* Editor and Scene Serialization System

#### Performance and Memory Profiling Toolkit
* Memory Profiling Tool Suite
* Visual Scope Profiler
* Custom Allocators
* Thread Pool
* Custom Reader-Biased RW Lock

Find more details on the above features [here](Documentation/FeatureDetails.md)

Visit my website for pictures, and higher quality videos: [https://sparky015.github.io](https://sparky015.github.io)

![Bistro](Documentation/Assets/Videos/Bistro_Short.gif)

### How to Build

-----

Supported Compilers: MSVC, AppleClang, and Clang
Supported Platforms: Windows and macOS   

Astral uses CMake for the build system.

In addition to the methods below, you can also use an IDE that supports building with CMake like CLion or Visual Studio. Before running the project,
make sure to switch the target to your desired target that you want to run.

Requirements include:
- git
- CMake ver. 3.28+ (Note: The below examples require cmake to be available on the command line)
- A supported C++20 compiler
- 64-bit CPU
- Vulkan SDK (if targeting Vulkan) 
- Windows: GPU support for Vulkan 1.3
- macOS: GPU support for Metal 4 if targeting Metal 4 backend, or support for Metal 3 if targeting Vulkan backend


Note: If targeting Vulkan, the project will not build without the Vulkan SDK installed on your computer. You will also need the debug versions of the libraries installed.
      The version needed is 1.3.296.0. You can run the CheckVulkanSDK.py script (in the Scripts folder) to see if you have the libraries needed present and the correct Vulkan SDK version installed.


Use ```git clone --recursive https://github.com/Sparky015/AstralEngine.git``` to download the project. Do not use the 'Download Zip' option!

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



### Branches

-----

- main      --> Latest development
- bug-X     --> Branches for bug fix work to be done on
- feature-X --> Branches for feature work to be done on
- doc-X     --> Branches for changing or adding documentation

