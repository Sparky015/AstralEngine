cmake_minimum_required(VERSION 3.28)
project(AstralEngine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

add_subdirectory(ThirdPartyLibraries)
add_subdirectory(Source/AstralEngine)
add_subdirectory(Source/Chess)

add_subdirectory(Tests)

