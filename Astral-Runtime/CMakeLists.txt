add_library(AstralEngine
        pch.h
        pch.cpp
        Core/Events/EventBus.h
        Core/Events/EventListener.h
        Core/Events/EventPublisher.h
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
        Debug/ImGui/ImGuiDependencies/imgui_impl_vulkan.h
        Debug/ImGui/ImGuiDependencies/imgui_impl_vulkan.cpp
        Debug/ImGui/ImGuiEvents.h
        Debug/EngineDebugInfoLogger.h
        Debug/EngineDebugInfoLogger.cpp
        Debug/Utilities/ConsoleLogFile.cpp
        Debug/Utilities/ConsoleLogFile.h
        Debug/Instrumentation/ProfilerLogFile.cpp
        Debug/Instrumentation/ProfilerLogFile.h
        Scenes/SceneManager.cpp
        Scenes/SceneManager.h
        ECS/Components/TransformComponent.h
        ECS/Components/SpriteComponent.h
        Renderer/RendererManager.cpp
        Renderer/RendererManager.h
        Renderer/RHI/Resources/ShaderSource.h
        Renderer/RHI/Resources/ShaderSource.cpp
        Renderer/RHI/Resources/VertexBuffer.cpp
        Renderer/RHI/Resources/VertexBuffer.h
        Renderer/RHI/Resources/IndexBuffer.cpp
        Renderer/RHI/Resources/IndexBuffer.h
        Renderer/SceneRenderer.cpp
        Renderer/SceneRenderer.h
        Renderer/RHI/RendererContext.h
        Renderer/RHI/RendererContext.cpp
        Window/Window.cpp
        Window/Window.h
        Window/Platform/Generic/GenericWindow.cpp
        Window/Platform/Generic/GenericWindow.h
        Renderer/RHI/Resources/Texture.cpp
        Renderer/RHI/Resources/Texture.h
        Renderer/RHI/Resources/VertexBufferLayout.cpp
        Renderer/RHI/Resources/VertexBufferLayout.h
        Core/Math/Math.h
        Renderer/RHI/Resources/Shader.h
        Renderer/RHI/RendererAPI.cpp
        Renderer/RHI/RendererAPI.h
        Renderer/RHI/RendererCommands.cpp
        Renderer/RHI/RendererCommands.h
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

        Renderer/Cameras/Camera.cpp
        Renderer/Cameras/Camera.h
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
        Asset/AssetRegistry.h
        Asset/Asset.h
        Asset/AssetRegistry.cpp
        Asset/AssetErrorCodes.h
        Asset/AssetRegistry_Impl.h
        Asset/AssetRegistryStats.h
        Asset/AssetManager.h
        Asset/AssetManager.cpp
        Asset/Loaders/TextureLoader.h
        Asset/Loaders/TextureLoader.cpp
        Debug/ImGui/Components/AssetStatsComponents.h
        Debug/ImGui/Components/AssetStatsComponents.cpp
        Debug/MemoryTracking/GenericRegionAllocator.cpp
        Debug/MemoryTracking/GenericRegionAllocator.h
        Debug/MemoryTracking/MallocFreeOverrides.h
        Debug/MemoryTracking/MallocFreeOverrides.cpp
        Debug/MemoryTracking/RegionTrackingAllocators/WindowTrackingAllocators.h
        Debug/MemoryTracking/RegionTrackingAllocators/WindowTrackingAllocators.cpp
        Core/SmartPointers.h



        Renderer/RHI/Platform/Vulkan/VulkanRendererContext.cpp
        Renderer/RHI/Platform/Vulkan/VulkanRendererContext.h
        Renderer/RHI/Platform/Vulkan/Resources/VulkanPhysicalDevices.cpp
        Renderer/RHI/Platform/Vulkan/Resources/VulkanPhysicalDevices.h
        Renderer/RHI/Resources/Sampler.h
        Renderer/RHI/Resources/PipelineState.h
        Renderer/RHI/Resources/RenderTarget.h
        Renderer/RHI/Resources/Framebuffer.h
        Renderer/RHI/Resources/Device.h
        Renderer/RHI/Resources/DescriptorSet.h
        Renderer/RHI/Resources/CommandQueue.h
        Renderer/RHI/Resources/Buffer.h
        Renderer/RHI/Resources/CommandBuffer.h
        Renderer/RHI/Resources/Swapchain.h
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
        Renderer/RHI/Platform/Vulkan/Resources/VulkanRenderpass.cpp
        Renderer/RHI/Platform/Vulkan/Resources/VulkanFramebuffer.h
        Renderer/RHI/Platform/Vulkan/Resources/VulkanFramebuffer.cpp
        Renderer/RHI/Platform/Vulkan/Resources/VulkanShader.cpp
        Renderer/RHI/Platform/Vulkan/Resources/VulkanShader.h
        Renderer/RHI/Platform/Vulkan/Resources/VulkanPipelineState.cpp
        Renderer/RHI/Platform/Vulkan/Resources/VulkanPipelineState.h
        Renderer/RHI/Platform/Vulkan/Resources/VulkanBuffer.cpp
        Renderer/RHI/Platform/Vulkan/Resources/VulkanBuffer.h
        Renderer/RHI/Platform/Vulkan/Resources/VulkanVertexBuffer.cpp
        Renderer/RHI/Platform/Vulkan/Resources/VulkanVertexBuffer.h
        Renderer/RHI/Platform/Vulkan/Resources/VulkanDescriptorSet.cpp
        Renderer/RHI/Platform/Vulkan/Resources/VulkanDescriptorSet.h
        Renderer/RHI/Platform/Vulkan/Resources/VulkanDescriptorSet.h
        Renderer/RHI/Platform/Vulkan/Resources/VulkanDescriptorSet.cpp
        Renderer/RHI/Platform/Vulkan/Resources/VulkanIndexBuffer.cpp
        Renderer/RHI/Platform/Vulkan/Resources/VulkanIndexBuffer.h
        Renderer/RHI/Platform/Vulkan/Resources/VulkanTexture.cpp
        Renderer/RHI/Platform/Vulkan/Resources/VulkanTexture.h
        Renderer/Common/Mesh.h
        Renderer/Common/Material.h
        Renderer/RHI/Resources/Shader.cpp
        Asset/Loaders/MaterialLoader.cpp
        Asset/Loaders/ShaderLoader.cpp
        Renderer/RHI/Common/ImageLayouts.h
        Renderer/RHI/Common/ImageFormats.h
        Renderer/RHI/Platform/Vulkan/Common/VkEnumConversions.h
        Renderer/RHI/Platform/Vulkan/Common/VkEnumConversions.cpp
        Renderer/RHI/Common/AccessFlags.h
        Renderer/Common/RendererEvents.h
        Asset/Loaders/MeshLoader.cpp
        Asset/Loaders/MeshLoader.cpp
        Scenes/Scene.h
        Scenes/Scene.h
        Asset/Loaders/SceneLoader.cpp
        Asset/Loaders/SceneLoader.h
        Renderer/RHI/Resources/DescriptorSet.cpp
        Renderer/RHI/Resources/DescriptorSet.cpp
        Renderer/RHI/Resources/PipelineStateCache.cpp
        Renderer/RHI/Resources/PipelineStateCache.h
        Core/Utility/Hashing/HashCombiner.cpp
        Core/Utility/Hashing/HashCombiner.h
        Core/Utility/Hashing/Hashes.cpp
        Core/Utility/Hashing/Hashes.h
        Renderer/SceneRendererImpl.cpp
        Renderer/SceneRendererImpl.h
        Renderer/RHI/Common/PipelineStageFlags.h
        Renderer/RenderGraph/RenderGraph.cpp
        Renderer/RenderGraph/RenderGraph.h
        Renderer/RenderGraph/RenderGraphPass.cpp
        Renderer/RenderGraph/RenderGraphPass.h
        Core/Containers/DirectedGraph.cpp
        Core/Containers/DirectedGraph.h
        Renderer/RHI/Common/PipelineBarriers.h
        Renderer/RHI/Common/ImageSubresourceRange.h
        Renderer/RHI/Common/DependencyFlags.h
        Renderer/RHI/Common/ImageFormats.cpp
        Asset/Ref.h
        Core/Containers/Bitmap.h
        Core/Containers/Bitmap_Impl.h
        Core/Containers/DirectedGraphVertex.h
        Core/Containers/DirectedGraphEdge.h
        Asset/Loaders/EnvironmentMapLoader.h
        Asset/Loaders/EnvironmentMapLoader.cpp
        Renderer/RHI/Platform/Vulkan/Resources/VulkanComputePipelineState.cpp
        Asset/Loaders/CubeLUTLoader.cpp
        Renderer/RHI/Common/SamplerSettings.h
        Renderer/RHI/Common/PipelineRasterizerState.h
        Renderer/RHI/Common/MemoryTypes.h
        Job/JobManager.cpp
        Job/JobManager.h
        Job/ThreadPool.cpp
        Job/ThreadPool.h
        Job/ThreadPool_Impl.h
)


# Flags for just the Astral Engine and all libraries and executables that link to it
if (APPLE)
    target_compile_options(AstralEngine PRIVATE -Wall -Wextra)#-fsanitize=address
else ()
    target_compile_options(AstralEngine PRIVATE -W1)
endif()


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
target_link_libraries(AstralEngine PRIVATE nfd)
target_link_libraries(AstralEngine PUBLIC msgpack-cxx)
target_link_libraries(AstralEngine PUBLIC assimp)
target_link_libraries(AstralEngine PUBLIC gli)
target_link_libraries(AstralEngine PRIVATE yaml-cpp)




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
        assimp
        nfd
        gli
        yaml-cpp
)

if (APPLE)
    target_link_libraries(AstralEngine PRIVATE glfw.3)
elseif (WIN32)
    target_link_libraries(AstralEngine PRIVATE glfw3)
endif ()



#                         ######   VULKAN SET UP   ######

if (WIN32)
    target_link_directories(AstralEngine PUBLIC "$ENV{VULKAN_SDK}/Lib")
    if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
        target_link_libraries(AstralEngine PUBLIC
                vulkan-1
                SPIRV-Toolsd
                SPIRV-Tools-diffd
                SPIRV-Tools-optd
                glslangd
                glslang-default-resource-limitsd
        )
    else ()
        target_link_libraries(AstralEngine PUBLIC
                vulkan-1
                SPIRV-Tools
                SPIRV-Tools-diff
                SPIRV-Tools-opt
                glslang
                glslang-default-resource-limits
        )
    endif ()
elseif (APPLE)
    target_link_directories(AstralEngine PUBLIC "$ENV{VULKAN_SDK}/lib")
    target_link_libraries(AstralEngine PUBLIC
            vulkan.1
            glslang
            glslang-default-resource-limits
            SPIRV-Tools
            SPIRV-Tools-diff
            SPIRV-Tools-opt
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
target_include_directories(AstralEngine PUBLIC "${CMAKE_SOURCE_DIR}/ThirdPartyLibraries/nativefiledialog-extended/src/include")
target_include_directories(AstralEngine PUBLIC "${CMAKE_SOURCE_DIR}/ThirdPartyLibraries/assimp/include")
target_include_directories(AstralEngine PUBLIC "${CMAKE_SOURCE_DIR}/ThirdPartyLibraries/gli/")
target_include_directories(AstralEngine PUBLIC "${CMAKE_SOURCE_DIR}/ThirdPartyLibraries/yaml-cpp/")


target_include_directories(AstralEngine PUBLIC "${CMAKE_SOURCE_DIR}/ThirdPartyLibraries/")

message(STATUS "Vulkan SDK Path: $ENV{VULKAN_SDK}")

target_include_directories(AstralEngine PUBLIC "$ENV{VULKAN_SDK}/include")


# Precompiled Header
#target_precompile_headers(AstralEngine PUBLIC "pch.h")



# Defining Macros

if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")    # DEBUG
    target_compile_definitions(AstralEngine PUBLIC -DASTRAL_DEBUG_BUILD
    )

elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "Release")    # Release
    target_compile_definitions(AstralEngine PUBLIC
            -DASTRAL_RELEASE_BUILD
            -DTURN_OFF_DEBUG_MENU
            -DTURN_OFF_DEBUG_MACROS
            -DTURN_OFF_PROFILER_MACRO
            -DTURN_OFF_LOGGING_CONSOLE_TO_FILE
            -DASTRAL_DISABLE_RENDERING_API_VALIDATION
            -DASTRAL_DISABLE_MEMORY_TRACKING
    )

elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "MinSizeRel")    # MinSizeRel
    target_compile_definitions(AstralEngine PUBLIC
            -DASTRAL_MINIMUM_SIZE_RELEASE_BUILD
    )

elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "RelWithDebInfo")    # RelWithDebInfo
    target_compile_definitions(AstralEngine PUBLIC
            -DASTRAL_RELEASE_WITH_DEBUG_INFO_BUILD
            -DTURN_OFF_LOGGING_CONSOLE_TO_FILE
#            -DTURN_OFF_DEBUG_MACROS
            -DASTRAL_DISABLE_RENDERING_API_VALIDATION
            -DASTRAL_DISABLE_MEMORY_TRACKING
    )

elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "Default")    # Default
    target_compile_definitions(AstralEngine PUBLIC
            -DASTRAL_DEBUG_BUILD

    )

endif()




target_compile_definitions(AstralEngine PUBLIC -DGLFW_INCLUDE_NONE)
target_compile_definitions(AstralEngine PUBLIC -DIMGUI_IMPL_OPENGL_LOADER_CUSTOM)

if (APPLE)
    target_compile_definitions(AstralEngine PUBLIC -DASTRAL_PLATFORM_MACOS)

    target_compile_definitions(AstralEngine PUBLIC -DASTRAL_DEFAULT_RENDERING_API_VULKAN)
    target_compile_definitions(AstralEngine PUBLIC -DASTRAL_VULKAN_AVAILABLE)

elseif (WIN32)
    target_compile_definitions(AstralEngine PUBLIC -DASTRAL_PLATFORM_WINDOWS)

    target_compile_definitions(AstralEngine PUBLIC -DASTRAL_DEFAULT_RENDERING_API_VULKAN)
    target_compile_definitions(AstralEngine PUBLIC -DASTRAL_VULKAN_AVAILABLE)

elseif (LINUX)
    target_compile_definitions(AstralEngine PUBLIC -DASTRAL_PLATFORM_LINUX)

    target_compile_definitions(AstralEngine PUBLIC -DASTRAL_DEFAULT_RENDERING_API_VULKAN)
    target_compile_definitions(AstralEngine PUBLIC -DASTRAL_VULKAN_AVAILABLE)

endif ()

target_compile_definitions(AstralEngine PUBLIC -DROOT_DIR="${CMAKE_SOURCE_DIR}/")
target_compile_definitions(AstralEngine PUBLIC -DLOG_FILE_DIR="${CMAKE_SOURCE_DIR}/Logs/")
target_compile_definitions(AstralEngine PUBLIC -DASTRAL_RUNTIME_DIR="${CMAKE_CURRENT_SOURCE_DIR}/")
target_compile_definitions(AstralEngine PUBLIC -DSHADER_DIR="${CMAKE_CURRENT_SOURCE_DIR}/Renderer/Shaders/")
target_compile_definitions(AstralEngine PUBLIC -DASTRAL_RENDERER_ASSETS_DIR="${CMAKE_CURRENT_SOURCE_DIR}/Renderer/Assets/")
target_compile_definitions(AstralEngine PUBLIC -DENGINE_BUILD_PROFILE="${CMAKE_BUILD_TYPE}")