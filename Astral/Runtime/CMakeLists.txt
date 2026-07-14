
set(ENGINE_SOURCE
        pch.h
        pch.cpp
        Core/Engine.h
        Core/Engine.cpp
        Core/EngineLoopEvents.h
        Core/SystemManager.h
        Core/SystemManager.cpp
        Window/WindowManager.cpp
        Window/WindowManager.h
        Window/WindowEvents.h
        Debug/ImGui/ImGuiManager.h
        Debug/ImGui/ImGuiManager.cpp
        Debug/ImGui/ImGuiEvents.h
        Debug/EngineDebugInfoLogger.h
        Debug/EngineDebugInfoLogger.cpp
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
        Renderer/Cameras/Camera.cpp
        Renderer/Cameras/Camera.h

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

        Core/SmartPointers.h
        Debug/ImGui/ImGuiDependencies/imgui_impl_glfw.h
        Debug/ImGui/ImGuiDependencies/imgui_impl_glfw.cpp

        Renderer/RHI/Platform/Vulkan/VulkanRendererContext.cpp
        Renderer/RHI/Platform/Vulkan/VulkanRendererContext.h
        Renderer/RHI/Platform/Vulkan/Resources/VulkanPhysicalDevices.cpp
        Renderer/RHI/Platform/Vulkan/Resources/VulkanPhysicalDevices.h
        Renderer/RHI/Resources/Sampler.h
        Renderer/RHI/Resources/PipelineState.h
        Renderer/RHI/Resources/RenderTarget.h
        Renderer/RHI/Resources/Device.h
        Renderer/RHI/Resources/DescriptorSet.h
        Renderer/RHI/Resources/CommandQueue.h
        Renderer/RHI/Resources/Buffer.h
        Renderer/RHI/Resources/CommandBuffer.h
        Renderer/RHI/Resources/Swapchain.h
        Renderer/RHI/Common/GraphicsSmartPointers.h
        Renderer/Common/Mesh.h
        Renderer/Common/Material.h
        Renderer/RHI/Resources/Shader.cpp
        Asset/Loaders/MaterialLoader.cpp
        Asset/Loaders/ShaderLoader.cpp
        Renderer/RHI/Common/ImageLayouts.h
        Renderer/RHI/Common/ImageFormats.h
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
        Renderer/SceneRendererImpl.cpp
        Renderer/SceneRendererImpl.h
        Renderer/RHI/Common/PipelineStageFlags.h
        Renderer/RenderGraph/RenderGraph.cpp
        Renderer/RenderGraph/RenderGraph.h
        Renderer/RenderGraph/RenderGraphPass.cpp
        Renderer/RenderGraph/RenderGraphPass.h
        Renderer/RHI/Common/PipelineBarriers.h
        Renderer/RHI/Common/ImageSubResourceRange.h
        Renderer/RHI/Common/DependencyFlags.h
        Renderer/RHI/Common/ImageFormats.cpp
        Asset/Ref.h
        Asset/Loaders/EnvironmentMapLoader.h
        Asset/Loaders/EnvironmentMapLoader.cpp
        Asset/Loaders/CubeLUTLoader.cpp
        Renderer/RHI/Common/SamplerSettings.h
        Renderer/RHI/Common/PipelineRasterizerState.h
        Renderer/RHI/Common/MemoryTypes.h
        Job/JobManager.cpp
        Job/JobManager.h
        Renderer/DrawList.cpp
        Renderer/DrawList.h

        Core/Hashing/Hashes.cpp
        Core/Hashing/Hashes.h
        Renderer/RHI/Common/BufferUsageFlags.h
        Renderer/RHI/Common/ShaderStage.h

        Renderer/RenderPasses/RenderPassExecutionContext.h
        Renderer/RenderPasses/CascadedShadowMapRenderPass.cpp
        Renderer/RenderPasses/CascadedShadowMapRenderPass.h
        Renderer/RenderPasses/EnvironmentMapPass.cpp
        Renderer/RenderPasses/EnvironmentMapPass.h
        Renderer/RenderPasses/DepthRenderPass.cpp
        Renderer/RenderPasses/DepthRenderPass.h
        Renderer/RenderPasses/ToneMappingPass.cpp
        Renderer/RenderPasses/ToneMappingPass.h
        Renderer/RenderPasses/DeferredGeometryRenderPass.cpp
        Renderer/RenderPasses/DeferredGeometryRenderPass.h
        Renderer/RenderPasses/DeferredLightingRenderPass.cpp
        Renderer/RenderPasses/DeferredLightingRenderPass.h
        Renderer/RenderPasses/ForwardLightingRenderPass.cpp
        Renderer/RenderPasses/ForwardLightingRenderPass.h
        Renderer/RenderingPaths/RenderingPath.h
        Renderer/RenderingPaths/DeferredRenderingPath.cpp
        Renderer/RenderingPaths/DeferredRenderingPath.h
        Renderer/RenderingPaths/ForwardRenderingPath.cpp
        Renderer/RenderingPaths/ForwardRenderingPath.h


)

set(VULKAN_RHI_FILES
    Renderer/RHI/Platform/Vulkan/VulkanRendererContext.cpp
    Renderer/RHI/Platform/Vulkan/VulkanRendererContext.h
    Renderer/RHI/Platform/Vulkan/Resources/VulkanPhysicalDevices.cpp
    Renderer/RHI/Platform/Vulkan/Resources/VulkanPhysicalDevices.h
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
    Renderer/RHI/Platform/Vulkan/Resources/VulkanRenderpass.cpp
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
    Renderer/RHI/Platform/Vulkan/Common/VkEnumConversions.h
    Renderer/RHI/Platform/Vulkan/Common/VkEnumConversions.cpp
    Renderer/RHI/Platform/Vulkan/Resources/VulkanComputePipelineState.cpp
    Debug/ImGui/ImGuiDependencies/imgui_impl_vulkan.h
    Debug/ImGui/ImGuiDependencies/imgui_impl_vulkan.cpp
)

set(METAL_RHI_FILES
    Renderer/RHI/Platform/Metal/MetalRendererContext.mm
    Renderer/RHI/Platform/Metal/MetalRendererContext.h
    Renderer/RHI/Platform/Metal/Common/MTLEnumConversions.cpp
    Renderer/RHI/Platform/Metal/Common/MTLEnumConversions.h
    Renderer/RHI/Platform/Metal/Resources/MetalDevice.cpp
    Renderer/RHI/Platform/Metal/Resources/MetalDevice.h
    Renderer/RHI/Platform/Metal/Resources/MetalSwapchain.cpp
    Renderer/RHI/Platform/Metal/Resources/MetalSwapchain.h
    Renderer/RHI/Platform/Metal/Resources/MetalBuffer.cpp
    Renderer/RHI/Platform/Metal/Resources/MetalBuffer.h
    Renderer/RHI/Platform/Metal/Resources/MetalVertexBuffer.cpp
    Renderer/RHI/Platform/Metal/Resources/MetalVertexBuffer.h
    Renderer/RHI/Platform/Metal/Resources/MetalIndexBuffer.cpp
    Renderer/RHI/Platform/Metal/Resources/MetalIndexBuffer.h
    Renderer/RHI/Platform/Metal/Resources/MetalTexture.cpp
    Renderer/RHI/Platform/Metal/Resources/MetalTexture.h
    Renderer/RHI/Platform/Metal/Resources/MetalShader.cpp
    Renderer/RHI/Platform/Metal/Resources/MetalShader.h
    Renderer/RHI/Platform/Metal/Resources/MetalDescriptorSet.cpp
    Renderer/RHI/Platform/Metal/Resources/MetalDescriptorSet.h
    Renderer/RHI/Platform/Metal/Resources/MetalGraphicsPipeline.cpp
    Renderer/RHI/Platform/Metal/Resources/MetalGraphicsPipeline.h
    Renderer/RHI/Platform/Metal/Resources/MetalComputePipeline.cpp
    Renderer/RHI/Platform/Metal/Resources/MetalComputePipeline.h
    Renderer/RHI/Platform/Metal/Resources/MetalRenderPass.cpp
    Renderer/RHI/Platform/Metal/Resources/MetalRenderPass.h
    Renderer/RHI/Platform/Metal/Resources/MetalCommandBuffer.cpp
    Renderer/RHI/Platform/Metal/Resources/MetalCommandBuffer.h
    Renderer/RHI/Platform/Metal/Resources/MetalCommandQueue.cpp
    Renderer/RHI/Platform/Metal/Resources/MetalCommandQueue.h
    Renderer/RHI/Platform/Metal/Resources/MetalRenderTarget.cpp
    Renderer/RHI/Platform/Metal/Resources/MetalRenderTarget.h
    Debug/ImGui/ImGuiDependencies/imgui_impl_metal.h
    Debug/ImGui/ImGuiDependencies/imgui_impl_metal.mm
    Renderer/RHI/Platform/Metal/MetalRendererCommands.cpp
    Renderer/RHI/Platform/Metal/MetalRendererCommands.h
)

set(D3D12_RHI_FILES
    Renderer/RHI/Platform/D3D12/D3D12RendererContext.cpp
    Renderer/RHI/Platform/D3D12/D3D12RendererContext.h
    Renderer/RHI/Platform/D3D12/Resources/D3D12Device.cpp
    Renderer/RHI/Platform/D3D12/Resources/D3D12Device.h
    Renderer/RHI/Platform/D3D12/Resources/D3D12Swapchain.cpp
    Renderer/RHI/Platform/D3D12/Resources/D3D12Swapchain.h
        Asset/AsyncLoadRegistry.cpp
        Asset/AsyncLoadRegistry.h
)

if (WIN32 OR APPLE OR LINUX)
    if (TARGET VulkanDependencies) # If VulkanDependencies target exists, then Vulkan is available
        list(APPEND ENGINE_SOURCE ${VULKAN_RHI_FILES})
    endif ()
endif()

if (APPLE)
    if (TARGET Metal4) # If Metal4 target exists, then Metal is available
        list(APPEND ENGINE_SOURCE ${METAL_RHI_FILES})
    endif ()
endif()

if (WIN32)
    if (TARGET D3D12Dependencies) # If D3D12 target exists, then D3D12 is available
        list(APPEND ENGINE_SOURCE ${D3D12_RHI_FILES})
    endif()
endif()


add_library(AstralEngine ${ENGINE_SOURCE})

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
target_link_libraries(AstralEngine PUBLIC AstralCore)
target_link_libraries(AstralEngine PUBLIC AstralProfilerAPI)

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
        AstralCore
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

if (WIN32 OR APPLE OR LINUX)
    if (TARGET VulkanDependencies) # If VulkanDependencies target exists, then Vulkan is available
        target_link_libraries(AstralEngine PUBLIC VulkanDependencies)
        target_compile_definitions(AstralEngine PUBLIC -DASTRAL_VULKAN_AVAILABLE)
    endif ()
endif()

#                         ######   METAL SET UP   ######

if (APPLE)
    if (TARGET Metal4) # If Metal4 target exists, then Metal is available
        target_link_libraries(AstralEngine PUBLIC Metal4)
        target_compile_definitions(AstralEngine PUBLIC -DASTRAL_METAL_AVAILABLE)
    endif ()
endif()

#                         ######   D3D12 SET UP   ######

if (WIN32)
    if (TARGET D3D12Dependencies) # If D3D12Dependencies target exists, then D3D12 is available
        target_link_libraries(AstralEngine PUBLIC D3D12Dependencies)
        target_compile_definitions(AstralEngine PUBLIC -DASTRAL_D3D12_AVAILABLE)
    endif()
endif()


# Include Directories
target_include_directories(AstralEngine PUBLIC .)

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
#    target_compile_definitions(AstralEngine PUBLIC -DASTRAL_DEFAULT_RENDERING_API_METAL)

elseif (WIN32)
    target_compile_definitions(AstralEngine PUBLIC -DASTRAL_PLATFORM_WINDOWS)
    target_compile_definitions(AstralEngine PUBLIC -DASTRAL_DEFAULT_RENDERING_API_VULKAN)

elseif (LINUX)
    target_compile_definitions(AstralEngine PUBLIC -DASTRAL_PLATFORM_LINUX)
    target_compile_definitions(AstralEngine PUBLIC -DASTRAL_DEFAULT_RENDERING_API_VULKAN)

endif ()

target_compile_definitions(AstralEngine PUBLIC -DROOT_DIR="${CMAKE_SOURCE_DIR}/")
target_compile_definitions(AstralEngine PUBLIC -DLOG_FILE_DIR="${CMAKE_SOURCE_DIR}/Logs/")
target_compile_definitions(AstralEngine PUBLIC -DASTRAL_RUNTIME_DIR="${CMAKE_CURRENT_SOURCE_DIR}")
target_compile_definitions(AstralEngine PUBLIC -DSHADER_DIR="${CMAKE_CURRENT_SOURCE_DIR}/Renderer/Shaders/")
target_compile_definitions(AstralEngine PUBLIC -DASTRAL_RENDERER_ASSETS_DIR="${CMAKE_CURRENT_SOURCE_DIR}/Renderer/Assets")
target_compile_definitions(AstralEngine PUBLIC -DENGINE_BUILD_PROFILE="${CMAKE_BUILD_TYPE}")