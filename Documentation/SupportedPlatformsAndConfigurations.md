### Current C++ Version (C++20)

-----

This project is using C++20 currently and mainly for [[unlikely]], [[likely]], consteval, and constexpr improvements


I am looking to switch to C++23 when the stacktraces feature is actually implemented by
all the major compilers. Also, std::unreachable would be useful.


### Supported Compilers
* MSVC
* AppleClang
* Clang

### Supported Platforms
* Windows
* macOS

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