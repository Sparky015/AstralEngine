cmake_minimum_required(VERSION 3.28)
project(Ayla)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)



# Set platform macros
if (WIN32)
    add_definitions(-DAYLA_PLATFORM_WINDOWS)
elseif (APPLE)
    add_definitions(-DAYLA_PLATFORM_MACOS)
else ()
    add_definitions(-DAYLA_PLATFORM_UNKNOWN)
endif()



if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    add_definitions(-DAYLA_DEBUG_BUILD)
elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
    add_definitions(-DAYLA_RELEASE_BUILD)
elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "MinSizeRel")
    add_definitions(-DAYLA_MINIMUM_SIZE_RELEASE_BUILD)
elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "RelWithDebInfo")
    add_definitions(-DAYLA_RELEASE_WITH_DEBUG_INFO_BUILD)
elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "Default")
    add_definitions(-DAYLA_RELEASE_WITH_DEBUG_INFO_BUILD)
endif()



#       ### Sub Directories ###
add_subdirectory(vendor/GLAD)
add_subdirectory(vendor/ImGui)
add_subdirectory(source/Ayla)
add_subdirectory(example-user-project)



# Recursive globs to get a list of all source and header files.
file(GLOB_RECURSE HEADER_LIST CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/source/*.hpp" "${CMAKE_SOURCE_DIR}/source/*.h")
file(GLOB_RECURSE SOURCE_LIST CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/source/*.cpp")

foreach(FILE ${HEADER_LIST} ${SOURCE_LIST})
    get_filename_component(PARENT_DIR "${FILE}" PATH)
    string(REPLACE "${CMAKE_SOURCE_DIR}" "" GROUP "${PARENT_DIR}")
    string(REPLACE "/" "\\" GROUP "${GROUP}")
    source_group("${GROUP}" FILES "${FILE}")
endforeach()
























#[[         ------ The Old Ways Used Along the Way ------

```cmake
include_directories(${CMAKE_SOURCE_DIR}/lib/SDL/include)
# ^^^ Fun fact I learned not while making this (for future reference), 'include_directories' tells cmake to also check a given directory
#     for header files
link_directories(${CMAKE_SOURCE_DIR}/lib/SDL/build/build/.libs)
# ^^^ Fun fact I learned while making this (for future reference), 'link_directories' tells cmake where to look for library files
#     so it can eventually link them to the executable later when you call the 'target_link_libraries' command.
#target_link_libraries(Ayla SDL2)
# ^^^ Fun fact I learned while making this (for future reference), the 'SDL2' comes from the file it finds that is always prefixed
#     with lib (libSDL2.dylib). That is how you know what the exact name of the library you are targeting is.
```

        ---------------  ---------------  ---------------  ---------------  ---------------

Just to be clear, I already knew this and it was obvious but 'add_executable' makes the executable for the project/other targets.


Apple was so easy to set SDL2 up even though I didn't use the normal .framework files.
Windows sucked to set up. So many errors and changes made.

        ---------------  ---------------  ---------------  ---------------  ---------------

I FINALLY GOT THEM BOTH (MAC AND WINDOWS) TO BUILD AND MAKE A WINDOW!!!!!!!!
YESSSSSSS!!!
I CAN FINALLY START!!!
        - Saturday, April 13 (5:37 PM)

        ---------------  ---------------  ---------------  ---------------  ---------------

#]]
