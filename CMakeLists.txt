cmake_minimum_required(VERSION 3.28)
project(AstralEngine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if (APPLE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -fsanitize=address")
else ()
#    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
endif()

add_subdirectory(ThirdPartyLibraries)
add_subdirectory(AstralEngine)
add_subdirectory(Samples/Chess)



