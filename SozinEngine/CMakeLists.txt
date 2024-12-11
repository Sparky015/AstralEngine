

if ("${CMAKE_BUILD_TYPE}" STREQUAL "Source/Debug")
    add_definitions(-DDEBUG_BUILD)
elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
    add_definitions(-DRELEASE_BUILD)
elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "MinSizeRel")
    add_definitions(-DMINIMUM_SIZE_RELEASE_BUILD)
elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "RelWithDebInfo")
    add_definitions(-DRELEASE_WITH_DEBUG_INFO_BUILD)
elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "Default")
    add_definitions(-DRELEASE_WITH_DEBUG_INFO_BUILD)
endif()


add_library(SozinEngine
        Source/Core/CoreMacroDefinitions.h
        Source/Debug/DebugMacros.h
        Source/Debug/DebugMacros.cpp
        Source/pch.h
        Source/pch.cpp
        Source/Core/Events/EventBus.h
        Source/Core/Events/EventListener.h
        Source/Core/Events/EventPublisher.h
        Source/Input/InputManager.cpp
        Source/Input/InputManager.h
        Source/Core/Engine.h
        Source/Core/Engine.cpp
        Source/Core/SystemEvents.h
        Source/Core/SystemManager.h
        Source/Core/SystemManager.cpp
        Source/Window/WindowManager.cpp
        Source/Window/WindowManager.h
        Source/Debug/AllocationTracker.h
        Source/Window/WindowEvents.h
        Source/Debug/DebugManager.h
        Source/Debug/DebugManager.cpp
        Source/Debug/ImGuiDependencies/imgui_impl_glfw.cpp
        Source/Debug/ImGuiDependencies/imgui_impl_glfw.h
        Source/Debug/ImGuiDependencies/imgui_impl_opengl3.cpp
        Source/Debug/ImGuiDependencies/imgui_impl_opengl3.h
        Source/Debug/DebugEvents.h
        Source/Debug/EngineDebugInfoLogger.h
        Source/Debug/EngineDebugInfoLogger.cpp
        Source/Debug/ConsoleLogFile.cpp
        Source/Debug/ConsoleLogFile.h
        Source/Debug/ProfilerLogFile.cpp
        Source/Debug/ProfilerLogFile.h
        Source/ECS/ECSManager.cpp
        Source/ECS/ECSManager.h
        Source/ECS/ECS.cpp
        Source/ECS/ECS.h
        Source/ECS/Entity.cpp
        Source/ECS/Entity.h
        Source/ECS/Components/Transform.h
        Source/ECS/ECSTypes.h
        Source/ECS/Components/Sprite.h
        Source/Renderer/RendererManager.cpp
        Source/Renderer/RendererManager.h
        Source/Renderer/Platform/OpenGL/Primitives/OpenGLVertexBuffer.h
        Source/Renderer/Platform/OpenGL/Primitives/OpenGLVertexBuffer.cpp
        Source/Renderer/Platform/OpenGL/Primitives/OpenGLVertexArrayObject.h
        Source/Renderer/Platform/OpenGL/Primitives/OpenGLVertexArrayObject.cpp
        Source/Renderer/Platform/OpenGL/Primitives/OpenGLIndexBuffer.h
        Source/Renderer/Platform/OpenGL/Primitives/OpenGLIndexBuffer.cpp
        Source/Renderer/Platform/OpenGL/Shaders/OpenGLShaderProgram.cpp
        Source/Renderer/Platform/OpenGL/Shaders/OpenGLShaderProgram.h
        Source/Renderer/Shaders/ShaderSource.h
        Source/Renderer/Shaders/ShaderSource.cpp
        Source/Renderer/Primitives/VertexBuffer.cpp
        Source/Renderer/Primitives/VertexBuffer.h
        Source/Renderer/Primitives/IndexBuffer.cpp
        Source/Renderer/Primitives/IndexBuffer.h
        Source/Renderer/Renderer.cpp
        Source/Renderer/Renderer.h
        Source/Renderer/Platform/OpenGL/OpenGLRenderingContext.h
        Source/Renderer/Platform/OpenGL/OpenGLRenderingContext.cpp
        Source/Renderer/RendererContext.h
        Source/Renderer/RendererContext.cpp
        Source/Window/Window.cpp
        Source/Window/Window.h
        Source/Window/Platform/Generic/GenericWindow.cpp
        Source/Window/Platform/Generic/GenericWindow.h
        Source/Renderer/Primitives/VertexArrayObject.cpp
        Source/Renderer/Primitives/VertexArrayObject.h
        Source/Renderer/Primitives/Texture.cpp
        Source/Renderer/Primitives/Texture.h
        Source/Renderer/Platform/OpenGL/Primitives/OpenGLTexture.cpp
        Source/Renderer/Platform/OpenGL/Primitives/OpenGLTexture.h
        Source/Renderer/Primitives/BufferLayout.cpp
        Source/Renderer/Primitives/BufferLayout.h
        Source/Core/Math.h
        Source/Renderer/Shaders/ShaderProgram.cpp
        Source/Renderer/Shaders/ShaderProgram.h
        Source/Renderer/RendererCommands.cpp
        Source/Renderer/RendererCommands.h
        Source/Renderer/RendererAPI.cpp
        Source/Renderer/RendererAPI.h
        Source/Renderer/Platform/OpenGL/OpenGLRendererAPI.cpp
        Source/Renderer/Platform/OpenGL/OpenGLRendererAPI.h
        Source/Renderer/Platform/OpenGL/OpenGLErrorTracking.h
        Source/Core/ApplicationModule.h
        Source/main.cpp
        Source/Sozin.h
        Source/Input/InputState.h
        Source/Input/InputState.cpp
        Source/Input/Keycodes.h
        Source/Input/KeycodeConversions.h
        Source/Input/KeycodeConversions.cpp
        Source/ECS/Systems/RenderingSystem.cpp
        Source/ECS/Systems/RenderingSystem.h
)




# Link directories for Cmake (Point to where the lib files are for GLFW)
if (APPLE)
    target_link_directories(SozinEngine PUBLIC ${CMAKE_SOURCE_DIR}/ThirdPartyLibraries/GLFW/mac-lib)
elseif (WIN32)
    target_link_directories(SozinEngine PUBLIC ${CMAKE_SOURCE_DIR}/ThirdPartyLibraries/GLFW/windows-lib)
endif ()



# Linking Libraries
target_link_libraries(SozinEngine PRIVATE ImGui)
target_link_libraries(SozinEngine PRIVATE GLAD)
target_link_libraries(SozinEngine PRIVATE glm)
target_link_libraries(SozinEngine PRIVATE std_image)

if (APPLE)
    target_link_libraries(SozinEngine PRIVATE glfw.3)
elseif (WIN32)
    target_link_libraries(SozinEngine PRIVATE glfw3)
endif ()



# Include Directories
target_include_directories(SozinEngine PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/Source")
target_include_directories(SozinEngine PRIVATE "${CMAKE_SOURCE_DIR}/ThirdPartyLibraries/GLFW/include")
target_include_directories(SozinEngine PRIVATE "${CMAKE_SOURCE_DIR}/ThirdPartyLibraries/GLAD")
target_include_directories(SozinEngine PUBLIC "${CMAKE_SOURCE_DIR}/ThirdPartyLibraries/glm")



# Precompiled Header
target_precompile_headers(SozinEngine PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/Source/pch.h")



# Defining Macros
target_compile_definitions(SozinEngine PRIVATE -DGLFW_INCLUDE_NONE)
target_compile_definitions(SozinEngine PRIVATE -DIMGUI_IMPL_OPENGL_LOADER_CUSTOM)
if (APPLE)
    target_compile_definitions(SozinEngine PRIVATE -DPLATFORM_MACOS)
elseif (WIN32)
    target_compile_definitions(SozinEngine PRIVATE -DPLATFORM_WINDOWS)
endif ()
target_compile_definitions(SozinEngine PRIVATE -DTURN_ON_LOGGING_CONSOLE_TO_FILE)
target_compile_definitions(SozinEngine PUBLIC -DROOT_DIR="${CMAKE_SOURCE_DIR}")
target_compile_definitions(SozinEngine PUBLIC -DSHADER_DIR="${CMAKE_CURRENT_SOURCE_DIR}/Source/Renderer/Shaders/")


add_subdirectory(Tests)