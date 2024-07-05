cmake_minimum_required(VERSION 3.28)
project(Ayla)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


#       ### Google Test ###
add_subdirectory(${CMAKE_SOURCE_DIR}/vendor/googletest-main/)
include_directories(${CMAKE_SOURCE_DIR}/vendor/googletest-main/googletest/include/)


# Set platform macros
if (WIN32)
    add_definitions(-DAYLA_PLATFORM_WINDOWS)
elseif (APPLE)
    add_definitions(-DAYLA_PLATFORM_APPLE)
else ()
    add_definitions(-DUNKNOWN_PLATFORM)
endif()




link_directories(${CMAKE_SOURCE_DIR}/cmake-build-debug)

#       ### Third Part Libraries ###
add_library(GLAD STATIC vendor/GLAD/glad.c)
target_include_directories(GLAD PUBLIC vendor/GLAD)

add_library(ImGui STATIC vendor/ImGui/imgui.cpp
        vendor/ImGui/imgui_demo.cpp
        vendor/ImGui/imgui_draw.cpp
        vendor/ImGui/imgui_widgets.cpp
        vendor/ImGui/imgui_tables.cpp)
target_include_directories(ImGui PUBLIC ${CMAKE_SOURCE_DIR}/vendor/ImGui)



# Create the static library
add_library(Ayla STATIC
        src/Ayla/Core/Application.cpp
        src/Ayla/Events/Event.cpp
        src/Ayla/Events/EventDispatcher.cpp
        src/Ayla/aypch.cpp
        src/Ayla/Window/Platform/Generic/GenericWindow.cpp
        src/Ayla/Layers/Layer.cpp
        src/Ayla/Layers/LayerStack.cpp
        src/Ayla/ImGui/ImGuiLayer.cpp
        src/Ayla/Input/InputLayer.cpp
        src/Ayla/Input/InputState.cpp
        src/Ayla/Debug/DebugLayer.cpp
        src/Ayla/Core/Time/Time.cpp
        src/Ayla/Input/KeycodeTranslation/GLFWTranslation.cpp
        src/Ayla/Renderer/Platform/OpenGL/imgui_impl_opengl3.cpp
        src/Ayla/Window/Platform/Generic/imgui_impl_glfw.cpp
        src/Ayla/Client/UserSetup.cpp
)

#       ### Ayla Includes ###
target_include_directories(Ayla PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_include_directories(Ayla PUBLIC ${CMAKE_SOURCE_DIR}/src)

#           Include library headers
target_include_directories(Ayla PUBLIC ${CMAKE_SOURCE_DIR}/vendor/GLFW/include)
target_include_directories(Ayla PUBLIC ${CMAKE_SOURCE_DIR}/vendor/ImGui)
target_include_directories(Ayla PUBLIC ${CMAKE_SOURCE_DIR}/vendor/glm)

#           Link Libraries to Ayla
target_link_libraries(Ayla GLAD)
target_link_libraries(Ayla ImGui)
if (APPLE)
    target_link_libraries(Ayla glfw.3)
elseif (WIN32)
    target_link_libraries(Ayla glfw3)
endif ()

# Link directories for Cmake (Point to where the lib files are for GLFW)
if (APPLE)
    target_link_directories(Ayla PUBLIC ${CMAKE_SOURCE_DIR}/vendor/GLFW/mac-lib)
elseif (WIN32)
    target_link_directories(Ayla PUBLIC ${CMAKE_SOURCE_DIR}/vendor/GLFW/windows-lib)
endif ()



# Defining Macros
target_compile_definitions(Ayla PRIVATE -DGLFW_INCLUDE_NONE)
target_compile_definitions(Ayla PRIVATE -DIMGUI_IMPL_OPENGL_LOADER_CUSTOM "glad/glad.h")


# Defining Precompiled Headers
target_precompile_headers(Ayla PRIVATE ${CMAKE_SOURCE_DIR}/src/Ayla/aypch.h)

add_executable(ClientGame
        client/src/main.cpp
        client/src/ExampleLayer.cpp)
target_compile_definitions(ClientGame PRIVATE -DGLFW_INCLUDE_NONE)

target_link_libraries(ClientGame Ayla)












#       ## Google Test ###
#enable_testing()
#
#include_directories(${CMAKE_SOURCE_DIR}/tests)
#
#add_executable(all_tests tests/hello_test.cpp
#        tests/test_EventSystem.cpp)
#
#target_link_libraries(all_tests Ayla)
#
#target_link_libraries(all_tests GTest::gtest_main)
#
#
#
#include(GoogleTest)
#gtest_discover_tests(all_tests)











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
