/**
* @file VulkanDevice.h
* @author Andrew Fagan
* @date 5/13/25
*/

#pragma once

#include "Core/FixedIntegerTypes.h"
#include "Renderer/RHI/Resources/Device.h"
#include "VulkanPhysicalDevices.h"
#include "Renderer/RHI/Resources/IndexBuffer.h"

#include <vulkan/vulkan_core.h>
#include <GLFW/glfw3.h>

namespace Astral {

    /**
     * @brief A description of how to build a Vulkan device
     */
    struct VulkanDeviceDesc
    {
        VulkanPhysicalDevice PhysicalDevice;
        uint32 QueueFamilyIndex;
        VkSurfaceKHR WindowSurface;
        GLFWwindow* Window;
    };

    /**
     * @brief A wrapper around a Vulkan device supporting extra convenience functions
     */
    class VulkanDevice : public Device
    {
    public:
        explicit VulkanDevice(const VulkanDeviceDesc& desc);
        ~VulkanDevice() override;

        /**
         * @brief Initializes the Metal device
         */
        void Init() override;

        /**
         * @brief Gets the swapchain instance
         */
        Swapchain& GetSwapchain() override { return *m_Swapchain; }

        /**
         * @brief Allocates a command buffer from the metal rendering context
         */
        CommandBufferHandle AllocateCommandBuffer() override;

        /**
         * @brief Gets the primary command queue from the metal rendering context
         */
        CommandQueueHandle GetPrimaryCommandQueue() override;

        /**
         * @brief Gets a dedicated command queue for asynchronous operations
         */
        CommandQueueHandle GetAsyncCommandQueue() override;

        /**
         * @brief Creates an empty render pass object
         * @return The created empty render pass object
         */
        RenderPassHandle CreateRenderPass() override;

        /**
         * @brief Creates a shader object from the shader source
         * @param shaderSource The source code of the shader
         * @return The shader object
         */
        ShaderHandle CreateShader(const ShaderSource& shaderSource) override;

        /**
         * @brief Creates a graphics pipeline state
         * @param pipelineStateCreateInfo A description of how to create the graphics pipeline
         * @return A graphics pipeline state handle
         */
        PipelineStateHandle CreateGraphicsPipelineState(const GraphicsPipelineStateCreateInfo& pipelineStateCreateInfo) override;

        /**
         * @brief Creates a compute pipeline state
         * @param computePipelineStateCreateInfo A description of how to create the compute pipeline
         * @return A compute pipeline state handle
         */
        PipelineStateHandle CreateComputePipelineState(const ComputePipelineStateCreateInfo& computePipelineStateCreateInfo) override;

        /**
         * @brief Creates a vertex buffer
         * @param vertexData The vertex data to populate the buffer with (or nullptr if the buffer should not be populated with data)
         * @param sizeInBytes The length of the buffer to create
         * @param bufferLayout The vertex attribute layout of the buffer
         * @param memoryType The GPU memory type to create the buffer with
         * @return A handle to the created vertex buffer
         */
        VertexBufferHandle CreateVertexBuffer(void* vertexData, uint32 sizeInBytes, VertexBufferLayout& bufferLayout, GPUMemoryType memoryType) override;

        /**
         * @brief Creates an index buffer
         * @param indexData The index data to populate the buffer with (or nullptr if the buffer should not be populated with data)
         * @param sizeInBytes The length of the buffer to create
         * @param memoryType The GPU memory type to create the buffer with
         * @return A handle to the created index buffer
         */
        IndexBufferHandle CreateIndexBuffer(uint32* indexData, uint32 sizeInBytes, GPUMemoryType memoryType) override;

        /**
         * @brief Creates a storage buffer
         * @param data The index data to populate the buffer with (or nullptr if the buffer should not be populated with data)
         * @param size The length of the buffer to create
         * @param memoryType The GPU memory type to create the buffer with
         * @return A handle to the created index buffer
         */
        BufferHandle CreateStorageBuffer(void* data, uint32 size, GPUMemoryType memoryType) override;

        /**
         * @brief Creates a uniform buffer
         * @param data The uniform data to populate the buffer with (or nullptr if the buffer should not be populated with data)
         * @param size The length of the buffer to create
         * @param memoryType The GPU memory type to create the buffer with
         * @return A handle to the created uniform buffer
         */
        BufferHandle CreateUniformBuffer(void* data, uint32 size, GPUMemoryType memoryType) override;

        /**
         * @brief Creates an empty descriptor set object
         * @return The created empty descriptor set object
         */
        DescriptorSetHandle CreateDescriptorSet() override;

        /**
         * @brief Creates a 2D texture object
         * @param textureCreateInfo Info to create the texture with
         * @return The created texture object
         */
        TextureHandle CreateTexture(const TextureCreateInfo& textureCreateInfo) override;

        /**
         * @brief Creates a cubemap texture object
         * @param textureCreateInfo Info to create the cubemap texture with
         * @return The created cubemap texture object
         */
        TextureHandle CreateCubemap(const TextureCreateInfo& textureCreateInfo) override;

        /**
         * @brief Creates a 3D texture object
         * @param textureCreateInfo Info to create the 3D texture with
         * @return The created 3D texture object
         */
        TextureHandle Create3DTexture(const TextureCreateInfo& textureCreateInfo) override;

        /**
         * @brief Creates a 1D texture object
         * @param textureCreateInfo Info to create the 1D texture with
         * @return The created 1D texture object
         */
        TextureHandle Create1DTexture(const TextureCreateInfo& textureCreateInfo) override;

        /**
         * @brief Creates a 2D texture array object
         * @param textureCreateInfo Info to create the 2D texture array with
         * @return The created 2D texture array object
         */
        TextureHandle Create2DTextureArray(const TextureCreateInfo& textureCreateInfo) override;

        /**
         * @brief Checks if anisotropy is supported by the GPU device
         * @return True if anisotropy is supported by the GPU device, false otherwise
         */
        bool IsBlitSupportedByFormat(ImageFormat imageFormat) override;

        /**
         * @brief Checks if anisotropy is supported by the GPU device
         * @return True if anisotropy is supported by the GPU device, false otherwise
         */
        bool IsAnisotropySupported() override;

        /**
         * @brief Gets the max anisotropy supported by the GPU device
         * @return The max anisotropy supported by the GPU device
         */
        float GetMaxAnisotropySupported() override;

        /**
         * @brief Gets the rendering API name and version
         * @return The rendering API name and version
         */
        std::string_view GetRenderingAPI() override;

        /**
         * @brief Gets the GPU's vendor name
         * @return The GPU's vendor name
         */
        std::string_view GetGPUVendor() override;

        /**
         * @brief Gets the graphics processor name
         * @return The graphics processor name
         */
        std::string_view GetGraphicsProcessorName() override;

        /**
         * @brief Blocks the current CPU thread until the GPU finishes all of its work
         */
        void WaitIdle() override;

        /**
         * @brief Gets the native graphics API handle of the device
         * @return The native graphics API handle of the device (MTL::Device*)
         */
        void* GetNativeHandle() override { return m_Device; }

        VulkanDevice(const VulkanDevice&) = delete;
        VulkanDevice& operator=(const VulkanDevice&) = delete;
        VulkanDevice(VulkanDevice&&) = delete;
        VulkanDevice& operator=(VulkanDevice&&) = delete;

    private:

        /**
         * @brief Creates the vk device object
         */
        void CreateDevice();

        /**
         * @brief Destroys the vk device object
         */
        void DestroyDevice();

        /**
         * @brief Allocates a swapchain instance
         */
        GraphicsOwnedPtr<Swapchain> CreateSwapchain(uint32 numberOfImages) override;

        /**
         * @brief Destroys the swapchain instance
         */
        void DestroySwapchain();

        VulkanPhysicalDevice m_PhysicalDevice;
        uint32 m_QueueFamilyIndex;
        VkSurfaceKHR m_WindowSurface;
        GLFWwindow* m_Window;

        VkDevice m_Device;
        GraphicsOwnedPtr<Swapchain> m_Swapchain;
    };

}
