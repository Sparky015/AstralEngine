cmake_minimum_required(VERSION 3.28)
project(Astral-Runtime)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if (APPLE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall ")
else ()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

add_subdirectory(ThirdPartyLibraries)
add_subdirectory(Astral-Runtime)
add_subdirectory(Astral-Editor)
add_subdirectory(Samples/Chess)

add_subdirectory(Tests)


####
#### Current Fixes Needed
#
#-----
#
#Decide on an error handling strategy and implement it consistently throughout the engine
#Fix the namespace spaghetti mess
#Add the missing documentation