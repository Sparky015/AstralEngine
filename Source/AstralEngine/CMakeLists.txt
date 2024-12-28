

if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
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
        Core/CoreMacroDefinitions.h
        Debug/DebugMacros.h
        Debug/DebugMacros.cpp
        pch.h
        pch.cpp
        Core/Events/EventBus.h
        Core/Events/EventListener.h
        Core/Events/EventPublisher.h
        Input/InputManager.cpp
        Input/InputManager.h
        Core/Engine.h
        Core/Engine.cpp
        Core/SystemEvents.h
        Core/SystemManager.h
        Core/SystemManager.cpp
        Window/WindowManager.cpp
        Window/WindowManager.h
        Debug/AllocationTracker.h
        Window/WindowEvents.h
        Debug/DebugManager.h
        Debug/DebugManager.cpp
        Debug/ImGuiDependencies/imgui_impl_glfw.cpp
        Debug/ImGuiDependencies/imgui_impl_glfw.h
        Debug/ImGuiDependencies/imgui_impl_opengl3.cpp
        Debug/ImGuiDependencies/imgui_impl_opengl3.h
        Debug/DebugEvents.h
        Debug/Loggers/EngineDebugInfoLogger.h
        Debug/Loggers/EngineDebugInfoLogger.cpp
        Debug/Loggers/ConsoleLogFile.cpp
        Debug/Loggers/ConsoleLogFile.h
        Debug/Loggers/ProfilerLogFile.cpp
        Debug/Loggers/ProfilerLogFile.h
        ECS/ECSManager.cpp
        ECS/ECSManager.h
        ECS/ECS.cpp
        ECS/ECS.h
        ECS/Entity.cpp
        ECS/Entity.h
        ECS/Components/Transform.h
        ECS/ECSTypes.h
        ECS/Components/Sprite.h
        Renderer/RendererManager.cpp
        Renderer/RendererManager.h
        Renderer/Platform/OpenGL/Primitives/OpenGLVertexBuffer.h
        Renderer/Platform/OpenGL/Primitives/OpenGLVertexBuffer.cpp
        Renderer/Platform/OpenGL/Primitives/OpenGLVertexArrayObject.h
        Renderer/Platform/OpenGL/Primitives/OpenGLVertexArrayObject.cpp
        Renderer/Platform/OpenGL/Primitives/OpenGLIndexBuffer.h
        Renderer/Platform/OpenGL/Primitives/OpenGLIndexBuffer.cpp
        Renderer/Platform/OpenGL/Shaders/OpenGLShaderProgram.cpp
        Renderer/Platform/OpenGL/Shaders/OpenGLShaderProgram.h
        Renderer/Shaders/ShaderSource.h
        Renderer/Shaders/ShaderSource.cpp
        Renderer/Primitives/VertexBuffer.cpp
        Renderer/Primitives/VertexBuffer.h
        Renderer/Primitives/IndexBuffer.cpp
        Renderer/Primitives/IndexBuffer.h
        Renderer/Renderer.cpp
        Renderer/Renderer.h
        Renderer/Platform/OpenGL/OpenGLRenderingContext.h
        Renderer/Platform/OpenGL/OpenGLRenderingContext.cpp
        Renderer/RendererContext.h
        Renderer/RendererContext.cpp
        Window/Window.cpp
        Window/Window.h
        Window/Platform/Generic/GenericWindow.cpp
        Window/Platform/Generic/GenericWindow.h
        Renderer/Primitives/VertexArrayObject.cpp
        Renderer/Primitives/VertexArrayObject.h
        Renderer/Primitives/Texture.cpp
        Renderer/Primitives/Texture.h
        Renderer/Platform/OpenGL/Primitives/OpenGLTexture.cpp
        Renderer/Platform/OpenGL/Primitives/OpenGLTexture.h
        Renderer/Primitives/BufferLayout.cpp
        Renderer/Primitives/BufferLayout.h
        Core/Math.h
        Renderer/Shaders/ShaderProgram.cpp
        Renderer/Shaders/ShaderProgram.h
        Renderer/RendererCommands.cpp
        Renderer/RendererCommands.h
        Renderer/RendererAPI.cpp
        Renderer/RendererAPI.h
        Renderer/Platform/OpenGL/OpenGLRendererAPI.cpp
        Renderer/Platform/OpenGL/OpenGLRendererAPI.h
        Renderer/Platform/OpenGL/OpenGLErrorTracking.h
        Core/ApplicationModule.h
        main.cpp
        Sozin.h
        Input/InputState.h
        Input/InputState.cpp
        Input/Keycodes.h
        Input/KeycodeConversions.h
        Input/KeycodeConversions.cpp
        ECS/Systems/RenderingSystem.cpp
        ECS/Systems/RenderingSystem.h
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
target_include_directories(SozinEngine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(SozinEngine PUBLIC "${CMAKE_SOURCE_DIR}/ThirdPartyLibraries/GLFW/include")
target_include_directories(SozinEngine PUBLIC "${CMAKE_SOURCE_DIR}/ThirdPartyLibraries/GLAD")
target_include_directories(SozinEngine PUBLIC "${CMAKE_SOURCE_DIR}/ThirdPartyLibraries/glm")



# Precompiled Header
target_precompile_headers(SozinEngine PRIVATE "pch.h")



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
target_compile_definitions(SozinEngine PUBLIC -DSHADER_DIR="${CMAKE_CURRENT_SOURCE_DIR}/Renderer/Shaders/")
