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
        ECS/ECS.cpp
        ECS/ECS.h
        ECS/Entity.cpp
        ECS/Entity.h
        ECS/Components/Transform.h
        ECS/Components/Sprite.h
        Renderer/RendererManager.cpp
        Renderer/RendererManager.h
        Renderer/RHI/Platform/OpenGL/Resources/OpenGLVertexBuffer.h
        Renderer/RHI/Platform/OpenGL/Resources/OpenGLVertexBuffer.cpp
        Renderer/RHI/Platform/OpenGL/Resources/OpenGLVertexArrayObject.h
        Renderer/RHI/Platform/OpenGL/Resources/OpenGLVertexArrayObject.cpp
        Renderer/RHI/Platform/OpenGL/Resources/OpenGLIndexBuffer.h
        Renderer/RHI/Platform/OpenGL/Resources/OpenGLIndexBuffer.cpp
        Renderer/RHI/Platform/OpenGL/Resources/OpenGLShaderProgram.cpp
        Renderer/RHI/Platform/OpenGL/Resources/OpenGLShaderProgram.h
        Renderer/Shaders/ShaderSource.h
        Renderer/Shaders/ShaderSource.cpp
        Renderer/RHI/Resources/VertexBuffer.cpp
        Renderer/RHI/Resources/VertexBuffer.h
        Renderer/RHI/Resources/IndexBuffer.cpp
        Renderer/RHI/Resources/IndexBuffer.h
        Renderer/SceneRenderer.cpp
        Renderer/SceneRenderer.h
        Renderer/RHI/Platform/OpenGL/OpenGLRendererContext.h
        Renderer/RHI/Platform/OpenGL/OpenGLRendererContext.cpp
        Renderer/RHI/RendererContext.h
        Renderer/RHI/RendererContext.cpp
        Window/Window.cpp
        Window/Window.h
        Window/Platform/Generic/GenericWindow.cpp
        Window/Platform/Generic/GenericWindow.h
        Renderer/RHI/Resources/VertexArrayObject.cpp
        Renderer/RHI/Resources/VertexArrayObject.h
        Renderer/RHI/Resources/Texture.cpp
        Renderer/RHI/Resources/Texture.h
        Renderer/RHI/Platform/OpenGL/Resources/OpenGLTexture.cpp
        Renderer/RHI/Platform/OpenGL/Resources/OpenGLTexture.h
        Renderer/RHI/Resources/BufferLayout.h
        Core/Math/Math.h
        Renderer/RHI/Resources/Shader.h
        Renderer/RHI/RendererAPI.cpp
        Renderer/RHI/RendererAPI.h
        Renderer/RHI/RendererCommands.cpp
        Renderer/RHI/RendererCommands.h
        Renderer/RHI/Platform/OpenGL/OpenGLRendererCommands.cpp
        Renderer/RHI/Platform/OpenGL/OpenGLRendererCommands.h
        Renderer/RHI/Platform/OpenGL/OpenGLErrorTracking.h
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
        ECS/ECSTypes.h
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
        Core/Memory/Allocators/StackBasedLinearAllocator.h
        Core/Time/Clock.h
        Core/Time/Clock.cpp
        Core/Time/DeltaTime.cpp
        Core/Time/DeltaTime.h
        Debug/ImGui/Components/EngineComponents.cpp
        Debug/ImGui/Components/EngineComponents.h

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

        Renderer/RHI/Platform/Vulkan/VulkanRendererContext.cpp
        Renderer/RHI/Platform/Vulkan/VulkanRendererContext.h
        Renderer/RHI/Platform/Vulkan/Resources/VulkanPhysicalDevices.cpp
        Renderer/RHI/Platform/Vulkan/Resources/VulkanPhysicalDevices.h
        Renderer/RHI/Resources/Sampler.h
        Renderer/RHI/Resources/PipelineStateObject.h
        Renderer/RHI/Resources/RenderTarget.h
        Renderer/RHI/Resources/Framebuffer.h
        Renderer/RHI/Resources/Framebuffer.cpp
        Renderer/RHI/Resources/DescriptorSetLayout.h
        Renderer/RHI/Resources/Device.h
        Renderer/RHI/Resources/DescriptorSet.h
        Renderer/RHI/Resources/DescriptorPool.h
        Renderer/RHI/Resources/CommandQueue.h
        Renderer/RHI/Resources/Buffer.h
        Renderer/RHI/Resources/CommandBuffer.h
        Renderer/RHI/Resources/Swapchain.h
        Renderer/RHI/Resources/Semaphore.h
        Renderer/RHI/Platform/Vulkan/VulkanRendererCommands.h
        Renderer/RHI/Platform/Vulkan/VulkanRendererCommands.cpp
        Renderer/RHI/Platform/Vulkan/Resources/VulkanDevice.h
        Renderer/RHI/Platform/Vulkan/Resources/VulkanDevice.cpp
        Renderer/RHI/Platform/Vulkan/Resources/VulkanCommandBuffer.h
        Renderer/RHI/Platform/Vulkan/Resources/VulkanCommandBuffer.cpp
        Renderer/RHI/Platform/Vulkan/Resources/VulkanSwapchain.h
        Renderer/RHI/Platform/Vulkan/Resources/VulkanSwapchain.cpp
        Renderer/RHI/Platform/Vulkan/Resources/VulkanCommandQueue.cpp
        Renderer/RHI/Platform/Vulkan/Resources/VulkanCommandQueue.cpp
        Renderer/RHI/Platform/Vulkan/Resources/VulkanRenderTarget.cpp
        Renderer/RHI/Platform/Vulkan/Resources/VulkanRenderTarget.h
        Renderer/RHI/Platform/Vulkan/Resources/VulkanRenderTarget.cpp
        Renderer/RHI/Platform/Vulkan/Resources/VulkanRenderTarget.h
        Renderer/RHI/Common/GraphicsSmartPointers.h
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



#                         ######   VULKAN SET UP   ######

if (WIN32)
    target_link_directories(AstralEngine PUBLIC "$ENV{VULKAN_SDK}/Lib")
    target_link_libraries(AstralEngine PUBLIC
            vulkan-1
            SPIRV-Toolsd
            SPIRV-Tools-diffd
            SPIRV-Tools-optd
            glslangd
            glslang-default-resource-limitsd
    )
elseif (APPLE)
    target_link_directories(AstralEngine PUBLIC "$ENV{VULKAN_SDK}/lib")
    target_link_libraries(AstralEngine PUBLIC
            vulkan.1
            SPIRV-Tools
            SPIRV-Tools-diff
            SPIRV-Tools-opt
            glslang
            glslang-default-resource-limits
    )
endif()




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

message(STATUS "Vulkan SDK Path: $ENV{VULKAN_SDK}")

target_include_directories(AstralEngine PUBLIC "$ENV{VULKAN_SDK}/include")


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

    target_compile_definitions(AstralEngine PUBLIC -DASTRAL_DEFAULT_RENDERING_API_VULKAN)
    target_compile_definitions(AstralEngine PUBLIC -DASTRAL_OPENGL_AVAILABLE)
    target_compile_definitions(AstralEngine PUBLIC -DASTRAL_VULKAN_AVAILABLE)

elseif (WIN32)
    target_compile_definitions(AstralEngine PUBLIC -DASTRAL_PLATFORM_WINDOWS)

    target_compile_definitions(AstralEngine PUBLIC -DASTRAL_DEFAULT_RENDERING_API_VULKAN)
    target_compile_definitions(AstralEngine PUBLIC -DASTRAL_OPENGL_AVAILABLE)
    target_compile_definitions(AstralEngine PUBLIC -DASTRAL_VULKAN_AVAILABLE)

elseif (LINUX)
    target_compile_definitions(AstralEngine PUBLIC -DASTRAL_PLATFORM_LINUX)

    target_compile_definitions(AstralEngine PUBLIC -DASTRAL_DEFAULT_RENDERING_API_OPENGL)
    target_compile_definitions(AstralEngine PUBLIC -DASTRAL_OPENGL_AVAILABLE)
#    target_compile_definitions(AstralEngine PUBLIC -DASTRAL_VULKAN_AVAILABLE)

endif ()

target_compile_definitions(AstralEngine PUBLIC -DTURN_ON_LOGGING_CONSOLE_TO_FILE)
target_compile_definitions(AstralEngine PUBLIC -DROOT_DIR="${CMAKE_SOURCE_DIR}/")
target_compile_definitions(AstralEngine PUBLIC -DLOG_FILE_DIR="${CMAKE_SOURCE_DIR}/Logs/")
target_compile_definitions(AstralEngine PUBLIC -DSHADER_DIR="${CMAKE_CURRENT_SOURCE_DIR}/Renderer/Shaders/")
target_compile_definitions(AstralEngine PUBLIC -DENGINE_BUILD_PROFILE="${CMAKE_BUILD_TYPE}/")

