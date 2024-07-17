

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


