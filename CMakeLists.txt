cmake_minimum_required(VERSION 3.28)
project(Ayla)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


#       ### Ayla ###
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/include/classes)


#       ### Google Test ###
add_subdirectory(${CMAKE_SOURCE_DIR}/vendor/googletest-main/)
include_directories(${CMAKE_SOURCE_DIR}/vendor/googletest-main/googletest/include/)


#       ### SDL2 ###
set(SDL2_DIR ${CMAKE_SOURCE_DIR}/vendor/SDL/SDL2-windows/cmake)
# (1/2) ^^^ Fun fact I learned while making this (for future reference), you can use 'set' to set a path to point to the SDL2Config.cmake (or sdl2-config.cmake).
# The SDL2 files comes with a cmake configuration file SDL2Config.cmake which sets up SDL2_INCLUDE_DIRS and SDL2_LIBRARIES.
find_package(SDL2 HINTS ${SDL2_DIR} REQUIRED)
# (2/2) so when you call find_package(SDL2), it searches for SDL2Config.cmake (or sdl2-config.cmake), and that file is responsible for
# setting up all the necessary variables (like SDL2_LIBRARIES and SDL2_INCLUDE_DIRS) and targets.
# Also, the naming of the package is based on how the cmake config file is named. It is SDL2 in this case because the config file is SDL2Config.cmake.
# So its basically the package name is * where the config file is named *Config.cmake (or *-config.cmake).




if (WIN32)
    add_definitions(-DAYLA_PLATFORM_WINDOWS)
elseif (APPLE)
    add_definitions(-DAYLA_PLATFORM_APPLE)
endif()

if(BUILD_SHARED_LIBS)
    add_definitions(-DAYLA_BUILD_SHARED)
endif()

include_directories(${SDL2_INCLUDE_DIRS})
include_directories(${CMAKE_SOURCE_DIR}/vendor/GLFW/include)


if (APPLE)
    link_directories(${CMAKE_SOURCE_DIR}/vendor/GLFW/mac-lib)
elseif (WIN32)
    link_directories(${CMAKE_SOURCE_DIR}/vendor/GLFW/windows-lib)
endif ()

add_executable(Ayla src/Ayla/EntryPoint/WindowLoop.cpp
        src/Ayla/EntryPoint/EntryPoint.cpp
        src/Ayla/Application.cpp
        include/Ayla/Application.h
        include/Ayla.h)
add_library(AylaEngine SHARED src/Ayla/EntryPoint/WindowLoop.cpp)

target_link_libraries(Ayla ${SDL2_LIBRARIES})

if (APPLE)
    target_link_libraries(Ayla glfw.3)
elseif (WIN32)
    target_link_libraries(Ayla glfw3)
endif ()




#       ## Google Test ###
target_link_libraries(Ayla GTest::gtest_main)







enable_testing()


add_executable(all_tests tests/hello_test.cpp
        src/Ayla/Application.cpp
        include/Ayla/Application.h)
add_executable(hello_test tests/hello_test.cpp)


target_link_libraries(all_tests GTest::gtest_main)
target_link_libraries(hello_test GTest::gtest_main)


include(GoogleTest)
gtest_discover_tests(all_tests)
gtest_discover_tests(hello_test)










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
