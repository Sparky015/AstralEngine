

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


add_library(AstralEngine
        Core/CoreMacroDefinitions.h
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
        Core/Memory/Tracking/AllocationTracker.h
        Window/WindowEvents.h
        Debug/ImGui/ImGuiManager.h
        Debug/ImGui/ImGuiManager.cpp
        Debug/ImGui/ImGuiDependencies/imgui_impl_glfw.cpp
        Debug/ImGui/ImGuiDependencies/imgui_impl_glfw.h
        Debug/ImGui/ImGuiDependencies/imgui_impl_opengl3.cpp
        Debug/ImGui/ImGuiDependencies/imgui_impl_opengl3.h
        Debug/ImGui/ImGuiEvents.h
        Debug/EngineDebugInfoLogger.h
        Debug/EngineDebugInfoLogger.cpp
        Debug/LogFiles/ConsoleLogFile.cpp
        Debug/LogFiles/ConsoleLogFile.h
        Debug/LogFiles/ProfilerLogFile.cpp
        Debug/LogFiles/ProfilerLogFile.h
        ECS/ECSManager.cpp
        ECS/ECSManager.h
        ECS/ECS.cpp
        ECS/ECS.h
        ECS/Entity.cpp
        ECS/Entity.h
        ECS/Components/Transform.h
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
        Core/Math/Math.h
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
        Astral.h
        Input/InputState.h
        Input/InputState.cpp
        Input/Keycodes.h
        Input/KeycodeConversions.h
        Input/KeycodeConversions.cpp
        ECS/Systems/RenderingSystem.cpp
        ECS/Systems/RenderingSystem.h
        Debug/Instrumentation/ScopeProfiler.cpp
        Debug/Instrumentation/ScopeProfiler.h
        Debug/Macros/Asserts.cpp
        Debug/Macros/Asserts.h
        Debug/Macros/Loggers.cpp
        Debug/Macros/Loggers.h
        Debug/Macros/ConsoleColors.cpp
        Debug/Macros/ConsoleColors.h
        Debug/Macros/Error.cpp
        Debug/Macros/Error.h
        ECS/ECSTypes.h
        Debug/Instrumentation/FunctionProfiler.h
        Debug/Instrumentation/FunctionProfiler.cpp
        Debug/ImGui/Components/WindowComponents.cpp
        Debug/ImGui/Components/WindowComponents.h
        Debug/ImGui/Components/RendererComponents.cpp
        Debug/ImGui/Components/RendererComponents.h
        Renderer/RendererEvents.h
        Debug/ImGui/Components/BuildConfigComponents.cpp
        Debug/ImGui/Components/BuildConfigComponents.h
        Debug/ImGui/Components/ECSComponents.cpp
        Debug/ImGui/Components/ECSComponents.h
        Debug/ImGui/Components/InputStateComponents.cpp
        Debug/ImGui/Components/InputStateComponents.h
        Debug/ImGui/Components/SystemInfoComponents.cpp
        Debug/ImGui/Components/SystemInfoComponents.h
        Core/Memory/Tracking/MemoryMetricsManager.cpp
        Core/Memory/Tracking/MemoryMetricsManager.h
        Core/Memory/Tracking/AllocationTracker.cpp
        Debug/ImGui/Components/MemoryComponents.cpp
        Debug/ImGui/Components/MemoryComponents.h
        Core/Memory/Tracking/PointerAllocationSizeMap.h
        Core/Memory/Tracking/PointerAllocationSizeMap.cpp
        Core/Memory/Allocators/NoTrackingAllocator.h
        Core/Memory/Allocators/LinearAllocator.h
        Core/Time/Clock.h
        Core/Time/Clock.cpp
        Core/Time/DeltaTime.cpp
        Core/Time/DeltaTime.h

)




# Link directories for Cmake (Point to where the lib files are for GLFW)
if (APPLE)
    target_link_directories(AstralEngine PUBLIC ${CMAKE_SOURCE_DIR}/ThirdPartyLibraries/GLFW/mac-lib)
elseif (WIN32)
    target_link_directories(AstralEngine PUBLIC ${CMAKE_SOURCE_DIR}/ThirdPartyLibraries/GLFW/windows-lib)
endif ()



# Linking Libraries
target_link_libraries(AstralEngine PRIVATE ImGui)
target_link_libraries(AstralEngine PRIVATE GLAD)
target_link_libraries(AstralEngine PRIVATE glm)
target_link_libraries(AstralEngine PRIVATE std_image)
target_link_libraries(AstralEngine PRIVATE cpuinfo)

add_dependencies(AstralEngine
        ImGui
        GLAD
        glm
        std_image
        cpuinfo
)

if (APPLE)
    target_link_libraries(AstralEngine PRIVATE glfw.3)
elseif (WIN32)
    target_link_libraries(AstralEngine PRIVATE glfw3)
endif ()



# Include Directories
target_include_directories(AstralEngine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(AstralEngine PUBLIC "${CMAKE_SOURCE_DIR}/ThirdPartyLibraries/cpuinfo/include")
target_include_directories(AstralEngine PUBLIC "${CMAKE_SOURCE_DIR}/ThirdPartyLibraries/GLFW/include")
target_include_directories(AstralEngine PUBLIC "${CMAKE_SOURCE_DIR}/ThirdPartyLibraries/GLAD")
target_include_directories(AstralEngine PUBLIC "${CMAKE_SOURCE_DIR}/ThirdPartyLibraries/glm")
target_include_directories(AstralEngine PUBLIC "${CMAKE_SOURCE_DIR}/ThirdPartyLibraries/ImGui")



# Precompiled Header
target_precompile_headers(AstralEngine PRIVATE "pch.h")



# Defining Macros
target_compile_definitions(AstralEngine PRIVATE -DGLFW_INCLUDE_NONE)
target_compile_definitions(AstralEngine PRIVATE -DIMGUI_IMPL_OPENGL_LOADER_CUSTOM)
if (APPLE)
    target_compile_definitions(AstralEngine PRIVATE -DPLATFORM_MACOS)
elseif (WIN32)
    target_compile_definitions(AstralEngine PRIVATE -DPLATFORM_WINDOWS)
endif ()
target_compile_definitions(AstralEngine PRIVATE -DTURN_ON_LOGGING_CONSOLE_TO_FILE)
target_compile_definitions(AstralEngine PUBLIC -DROOT_DIR="${CMAKE_SOURCE_DIR}")
target_compile_definitions(AstralEngine PUBLIC -DSHADER_DIR="${CMAKE_CURRENT_SOURCE_DIR}/Renderer/Shaders/")
target_compile_definitions(AstralEngine PUBLIC -DENGINE_BUILD_PROFILE="${CMAKE_BUILD_TYPE}")


