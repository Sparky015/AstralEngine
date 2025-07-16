


# Create the static library
add_library(Solas STATIC
        Core/Application.cpp
        Core/Application.h
        Core/CoreDefinitions.h
        Core/EntryPoint.h
        Events/EventTypes/ApplicationEvent.h
        Events/Event.cpp
        Events/Event.h
        Events/EventTypes/KeyEvent.h
        Events/EventTypes/MouseEvent.h
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
        Renderer/RendererLayer.cpp
        Renderer/RendererLayer.h
        Core/EntryPoint.cpp
        Debug/LogUsedToolsAndLibraries.cpp
        Debug/LogUsedToolsAndLibraries.h
        Renderer/Shaders/ParseShaders.cpp
        Renderer/Shaders/ParseShaders.h
        Renderer/Shaders/ShaderProgram.cpp
        Renderer/Shaders/ShaderProgram.h
        Renderer/Renderer.cpp
        Renderer/Renderer.h
        Renderer/Platform/OpenGL/OpenGLRenderer.cpp
        Renderer/Platform/OpenGL/OpenGLRenderer.h
        Renderer/Abstractions/VertexBuffer.cpp
        Renderer/Abstractions/VertexBuffer.h
        Renderer/Abstractions/VertexArrayObject.cpp
        Renderer/Abstractions/VertexArrayObject.h
        Renderer/Abstractions/IndexBufferObject.cpp
        Renderer/Abstractions/IndexBufferObject.h
        Events/EventBus.h
        Events/EventListener.h
        Events/EventPublisher.h
        Events/EventTypes/EventTypes.h
        Renderer/Abstractions/BufferLayout.cpp
        Renderer/Abstractions/BufferLayout.h
        Renderer/RendererContext.h
        Renderer/OpenGLRenderingContext.cpp
        Renderer/OpenGLRenderingContext.h

)



#       ### Ayla Includes ###
target_include_directories(Solas PUBLIC ${CMAKE_SOURCE_DIR}/source)
target_include_directories(Solas INTERFACE ${CMAKE_SOURCE_DIR}/include)



#           Include library headers
target_include_directories(Solas PRIVATE ${CMAKE_SOURCE_DIR}/vendor/GLFW/include)
target_include_directories(Solas PRIVATE ${CMAKE_SOURCE_DIR}/vendor/glm)



#           Link Libraries to Ayla
target_link_libraries(Solas PRIVATE GLAD)
target_link_libraries(Solas PUBLIC ImGui)


# Link directories for Cmake (Point to where the lib files are for GLFW)
if (APPLE)
    target_link_directories(Solas PUBLIC ${CMAKE_SOURCE_DIR}/vendor/GLFW/mac-lib)
elseif (WIN32)
    target_link_directories(Solas PUBLIC ${CMAKE_SOURCE_DIR}/vendor/GLFW/windows-lib)
endif ()


if (APPLE)
    target_link_libraries(Solas PRIVATE glfw.3)
elseif (WIN32)
    target_link_libraries(Solas PRIVATE glfw3)
endif ()



# Defining Macros
target_compile_definitions(Solas PRIVATE -DGLFW_INCLUDE_NONE)
target_compile_definitions(Solas PRIVATE -DIMGUI_IMPL_OPENGL_LOADER_CUSTOM)
target_compile_definitions(Solas PRIVATE -DSHADER_DIR="${CMAKE_CURRENT_SOURCE_DIR}/Renderer/Shaders/")


# Defining Precompiled Headers
target_precompile_headers(Solas PRIVATE ${CMAKE_SOURCE_DIR}/source/Solas/aypch.h)



# Recursive globs to get a list of all source and header files.
file(GLOB_RECURSE HEADER_LIST CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/source/*.hpp" "${CMAKE_SOURCE_DIR}/source/*.h")
file(GLOB_RECURSE SOURCE_LIST CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/source/*.cpp")

foreach(FILE ${HEADER_LIST} ${SOURCE_LIST})
    get_filename_component(PARENT_DIR "${FILE}" PATH)
    string(REPLACE "${CMAKE_SOURCE_DIR}" "" RELATIVE_PATH "${PARENT_DIR}")
    source_group("${RELATIVE_PATH}" FILES "${FILE}")
endforeach()


