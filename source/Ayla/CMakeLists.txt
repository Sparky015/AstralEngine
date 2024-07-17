


# Create the static library
add_library(Ayla STATIC
        Core/Application.cpp
        Core/Application.h
        Core/CoreDefinitions.h
        Core/EntryPoint.h
        Events/ApplicationEvent.h
        Events/Event.cpp
        Events/Event.h
        Events/EventDispatcher.h
        Events/KeyEvent.h
        Events/MouseEvent.h
        Events/EventDispatcher.cpp
        aypch.cpp
        aypch.h
        Window/Platform/Generic/GenericWindow.cpp
        Window/Platform/Generic/GenericWindow.h
        Core/Layers/Layer.cpp
        Core/Layers/Layer.h
        Core/Layers/LayerStack.cpp
        Core/Layers/LayerStack.h
        ImGui/ImGuiLayer.cpp
        ImGui/ImGuiLayer.h
        Input/InputLayer.cpp
        Input/InputLayer.h
        Input/InputState.cpp
        Input/InputState.h
        Input/Keycodes.h
        Debug/DebugLayer.cpp
        Debug/DebugLayer.h
        Core/Time/Clock.cpp
        Core/Time/Clock.h
        Input/KeycodeTranslation/GLFWTranslation.cpp
        Input/KeycodeTranslation/GLFWTranslation.h
        Renderer/Platform/OpenGL/imgui_impl_opengl3.cpp
        Renderer/Platform/OpenGL/imgui_impl_opengl3.h
        Window/Platform/Generic/imgui_impl_glfw.cpp
        Window/Platform/Generic/imgui_impl_glfw.h
        Window/Window.h
        Client/UserSetup.cpp
        Client/UserSetup.h
        Core/Time/FTimer.cpp
        Core/Time/FTimer.h
        Core/Time/TimeLiterals.h
        Debug/DebugMacros.h
        Debug/DebugMacros.cpp
)



#       ### Ayla Includes ###
target_include_directories(Ayla PUBLIC ${CMAKE_SOURCE_DIR}/source)
target_include_directories(Ayla INTERFACE ${CMAKE_SOURCE_DIR}/include)



#           Include library headers
target_include_directories(Ayla PRIVATE ${CMAKE_SOURCE_DIR}/vendor/GLFW/include)
target_include_directories(Ayla PRIVATE ${CMAKE_SOURCE_DIR}/vendor/glm)



#           Link Libraries to Ayla
target_link_libraries(Ayla PRIVATE GLAD)
target_link_libraries(Ayla PUBLIC ImGui)


# Link directories for Cmake (Point to where the lib files are for GLFW)
if (APPLE)
    target_link_directories(Ayla PRIVATE ${CMAKE_SOURCE_DIR}/vendor/GLFW/mac-lib)
elseif (WIN32)
    target_link_directories(Ayla PUBLIC ${CMAKE_SOURCE_DIR}/vendor/GLFW/windows-lib)
endif ()


if (APPLE)
    target_link_libraries(Ayla PRIVATE glfw.3)
elseif (WIN32)
    target_link_libraries(Ayla PUBLIC glfw3)
endif ()



# Defining Macros
target_compile_definitions(Ayla PRIVATE -DGLFW_INCLUDE_NONE)
target_compile_definitions(Ayla PRIVATE -DIMGUI_IMPL_OPENGL_LOADER_CUSTOM)



# Defining Precompiled Headers
target_precompile_headers(Ayla PRIVATE ${CMAKE_SOURCE_DIR}/source/Ayla/aypch.h)



# Recursive globs to get a list of all source and header files.
file(GLOB_RECURSE HEADER_LIST CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/source/*.hpp" "${CMAKE_SOURCE_DIR}/source/*.h")
file(GLOB_RECURSE SOURCE_LIST CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/source/*.cpp")

foreach(FILE ${HEADER_LIST} ${SOURCE_LIST})
    get_filename_component(PARENT_DIR "${FILE}" PATH)
    string(REPLACE "${CMAKE_SOURCE_DIR}" "" RELATIVE_PATH "${PARENT_DIR}")
    source_group("${RELATIVE_PATH}" FILES "${FILE}")
endforeach()


