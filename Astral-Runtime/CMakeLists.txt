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
        Core/EngineLoopEvents.h
        Core/SystemManager.h
        Core/SystemManager.cpp
        Window/WindowManager.cpp
        Window/WindowManager.h
        Debug/MemoryTracking/NewDeleteOverrides.h
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
        "Debug/Utilities/ConsoleLogFile.cpp"
        "Debug/Utilities/ConsoleLogFile.h"
        "Debug/Instrumentation/ProfilerLogFile.cpp"
        "Debug/Instrumentation/ProfilerLogFile.h"
        ECS/ECSManager.cpp
        ECS/ECSManager.h
        ECS/Old/ECS.cpp
        ECS/Old/ECS.h
        ECS/Old/Entity.cpp
        ECS/Old/Entity.h
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
        Debug/Utilities/Asserts.cpp
        Debug/Utilities/Asserts.h
        Debug/Utilities/Loggers.cpp
        Debug/Utilities/Loggers.h
        Debug/Utilities/ConsoleColors.cpp
        Debug/Utilities/ConsoleColors.h
        Debug/Utilities/Error.cpp
        Debug/Utilities/Error.h
        ECS/Old/ECSTypes.h
        Debug/Instrumentation/FunctionProfiler.h
        Debug/Instrumentation/FunctionProfiler.cpp
        Debug/ImGui/Components/WindowComponents.cpp
        Debug/ImGui/Components/WindowComponents.h
        Debug/ImGui/Components/RendererComponents.cpp
        Debug/ImGui/Components/RendererComponents.h
        Debug/ImGui/Components/BuildConfigComponents.cpp
        Debug/ImGui/Components/BuildConfigComponents.h
        Debug/ImGui/Components/ECSComponents.cpp
        Debug/ImGui/Components/ECSComponents.h
        Debug/ImGui/Components/InputStateComponents.cpp
        Debug/ImGui/Components/InputStateComponents.h
        Debug/ImGui/Components/SystemInfoComponents.cpp
        Debug/ImGui/Components/SystemInfoComponents.h
        Debug/MemoryTracking/MemoryMetrics.cpp
        Debug/MemoryTracking/MemoryMetrics.h
        Debug/MemoryTracking/NewDeleteOverrides.cpp
        Debug/ImGui/Components/MemoryComponents.cpp
        Debug/ImGui/Components/MemoryComponents.h
        Debug/MemoryTracking/NoTrackingSTLAllocator.h
        Core/Time/Clock.h
        Core/Time/Clock.cpp
        Core/Time/DeltaTime.cpp
        Core/Time/DeltaTime.h
        Debug/ImGui/Components/EngineComponents.cpp
        Debug/ImGui/Components/EngineComponents.h

        Core/Memory/Allocators/StackBasedLinearAllocator.h
        Core/Memory/Allocators/StackBasedLinearAllocator_impl.h
        Core/Memory/Allocators/StackAllocator.h
        Core/Memory/Allocators/STLAllocators/STLStackAllocator.h
        Core/Memory/Allocators/STLAllocators/STLStackBasedLinearAllocator.h
        Core/Memory/Allocators/FrameAllocator.h
        Core/Memory/Allocators/DoubleBufferedAllocator.h
        Core/Memory/Allocators/PoolAllocator.h
        Core/Memory/Pools/ObjectPool.h
        Core/Memory/Allocators/AllocatorUtils.h
        Core/Memory/Allocators/LinearAllocator.h
        Core/Memory/Allocators/STLAllocators/STLLinearAllocator.h
        Core/Memory/Allocators/AllocatorUtils.cpp
        Core/Memory/Allocators/LinearAllocation.cpp
        Core/Memory/Allocators/StackAllocator.cpp
        Core/Memory/Allocators/FrameAllocator.cpp
        Core/Memory/Allocators/DoubleBufferedAllocator.cpp
        Core/Memory/Allocators/PoolAllocator.cpp
        Core/Memory/Allocators/RingAllocator.cpp
        Core/Memory/Allocators/RingAllocator.h

        Renderer/Cameras/OrthographicCamera.cpp
        Renderer/Cameras/OrthographicCamera.h
        Core/Memory/Allocators/AlignedAllocator.cpp
        Core/Memory/Allocators/AlignedAllocator.h

        Debug/MemoryTracking/MemoryTracker.cpp
        Debug/MemoryTracking/MemoryTracker.h

        Debug/MemoryTracking/Visualization/Components/LoadFileComponent.cpp
        Debug/MemoryTracking/Visualization/Components/LoadFileComponent.h
        Debug/MemoryTracking/Visualization/Components/SceneMetricsGraphComponents.cpp
        Debug/MemoryTracking/Visualization/Components/SceneMetricsGraphComponents.h
        Debug/MemoryTracking/Visualization/Panels/AllocationsListPanel.cpp
        Debug/MemoryTracking/Visualization/Panels/AllocationsListPanel.h
        Debug/MemoryTracking/Visualization/Panels/AllocationStacktracePanel.cpp
        Debug/MemoryTracking/Visualization/Panels/AllocationStacktracePanel.h
        Debug/MemoryTracking/Visualization/Panels/DataPointInfoPanel.cpp
        Debug/MemoryTracking/Visualization/Panels/DataPointInfoPanel.h
        Debug/MemoryTracking/Visualization/Panels/GraphSelectorPanel.cpp
        Debug/MemoryTracking/Visualization/Panels/GraphSelectorPanel.h
        Debug/MemoryTracking/Visualization/Panels/MemoryGraphPanel.cpp
        Debug/MemoryTracking/Visualization/Panels/MemoryGraphPanel.h
        Debug/MemoryTracking/Visualization/MemoryDebugWindow.cpp
        Debug/MemoryTracking/Visualization/MemoryDebugWindow.h

        Debug/MemoryTracking/Serialization/SceneMetricsExporter.cpp
        Debug/MemoryTracking/Serialization/SceneMetricsExporter.h
        Debug/MemoryTracking/Serialization/SceneMetricsImporter.cpp
        Debug/MemoryTracking/Serialization/SceneMetricsImporter.h
        Debug/MemoryTracking/Serialization/SceneMetricsStorage.cpp
        Debug/MemoryTracking/Serialization/SceneMetricsStorage.h
        Debug/MemoryTracking/Serialization/MemoryMetricsSnapshot.h

        Debug/MemoryTracking/GlobalAllocationStorage.cpp
        Debug/MemoryTracking/GlobalAllocationStorage.h
        Debug/MemoryTracking/MemoryMetrics.cpp
        Debug/MemoryTracking/MemoryMetrics.h
        Debug/MemoryTracking/NewDeleteOverrides.cpp
        Debug/MemoryTracking/NewDeleteOverrides.h
        ECS/ECS.cpp
        ECS/ECS.h
        Core/FixedIntegerTypes.h
        ECS/Entity.h
        ECS/ECSErrorHandling.h
        ECS/ECS_Impl.h
        ECS/ComponentPool.h
        ECS/ComponentPoolSet.h
        ECS/ComponentPoolSet_Impl.h


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
target_link_libraries(AstralEngine PRIVATE stb_image)
target_link_libraries(AstralEngine PRIVATE cpuinfo)
target_link_libraries(AstralEngine PRIVATE ImPlot)
target_link_libraries(AstralEngine PRIVATE ImGuiFileDialog)
target_link_libraries(AstralEngine PUBLIC msgpack-cxx)

target_link_libraries(AstralEngine PRIVATE cpptrace::cpptrace)
target_compile_definitions(AstralEngine PRIVATE -DCPPTRACE_STATIC_DEFINE)


if (APPLE)
    target_link_libraries(AstralEngine PRIVATE -lc++)
endif ()

add_dependencies(AstralEngine
        ImGui
        GLAD
        glm
        stb_image
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
target_include_directories(AstralEngine PUBLIC "${CMAKE_SOURCE_DIR}/ThirdPartyLibraries/stb_image")
target_include_directories(AstralEngine PUBLIC "${CMAKE_SOURCE_DIR}/ThirdPartyLibraries/msgpack/include")
target_include_directories(AstralEngine PUBLIC "${CMAKE_SOURCE_DIR}/ThirdPartyLibraries/ImFileDialog/")
target_include_directories(AstralEngine PUBLIC "${CMAKE_SOURCE_DIR}/ThirdPartyLibraries/cpptrace/include")

target_include_directories(AstralEngine PUBLIC "${CMAKE_SOURCE_DIR}/ThirdPartyLibraries/")



# Precompiled Header
#target_precompile_headers(AstralEngine PUBLIC "pch.h")



# Defining Macros

if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    target_compile_definitions(AstralEngine PUBLIC -DASTRAL_DEBUG_BUILD)
elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
    target_compile_definitions(AstralEngine PUBLIC -DASTRAL_RELEASE_BUILD)
elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "MinSizeRel")
    target_compile_definitions(AstralEngine PUBLIC -DASTRAL_MINIMUM_SIZE_RELEASE_BUILD)
elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "RelWithDebInfo")
    target_compile_definitions(AstralEngine PUBLIC -DASTRAL_RELEASE_WITH_DEBUG_INFO_BUILD)
elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "Default")
    target_compile_definitions(AstralEngine PUBLIC -DASTRAL_DEBUG_BUILD)
endif()

target_compile_definitions(AstralEngine PUBLIC -DGLFW_INCLUDE_NONE)
target_compile_definitions(AstralEngine PUBLIC -DIMGUI_IMPL_OPENGL_LOADER_CUSTOM)
if (APPLE)
    target_compile_definitions(AstralEngine PUBLIC -DASTRAL_PLATFORM_MACOS)
elseif (WIN32)
    target_compile_definitions(AstralEngine PUBLIC -DASTRAL_PLATFORM_WINDOWS)
endif ()

target_compile_definitions(AstralEngine PUBLIC -DTURN_ON_LOGGING_CONSOLE_TO_FILE)
target_compile_definitions(AstralEngine PUBLIC -DROOT_DIR="${CMAKE_SOURCE_DIR}/")
target_compile_definitions(AstralEngine PUBLIC -DLOG_FILE_DIR="${CMAKE_SOURCE_DIR}/Logs/")
target_compile_definitions(AstralEngine PUBLIC -DSHADER_DIR="${CMAKE_CURRENT_SOURCE_DIR}/Renderer/Shaders/")
target_compile_definitions(AstralEngine PUBLIC -DENGINE_BUILD_PROFILE="${CMAKE_BUILD_TYPE}/")