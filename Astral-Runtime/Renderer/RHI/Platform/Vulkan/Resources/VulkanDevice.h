/**
* @file VulkanDevice.h
* @author Andrew Fagan
* @date 5/13/25
*/

#pragma once

#include "Core/FixedIntegerTypes.h"
#include "Renderer/RHI/Resources/Device.h"
#include "VulkanPhysicalDevices.h"

#include <vulkan/vulkan_core.h>
#include <GLFW/glfw3.h>

#include "Renderer/RHI/Resources/IndexBuffer.h"

namespace Astral {

    struct VulkanDeviceDesc
    {
        VulkanPhysicalDevice PhysicalDevice;
        uint32 QueueFamilyIndex;
        VkSurfaceKHR WindowSurface;
        GLFWwindow* Window;
    };

    class VulkanDevice : public Device
    {
    public:
        explicit VulkanDevice(const VulkanDeviceDesc& desc);
        ~VulkanDevice() override;

        void Init() override;

        Swapchain& GetSwapchain() override { return *m_Swapchain; }
        CommandBufferHandle AllocateCommandBuffer() override;
        CommandQueueHandle GetPrimaryCommandQueue() override;
        CommandQueueHandle GetAsyncCommandQueue() override;
        RenderPassHandle CreateRenderPass() override;
        FramebufferHandle CreateFramebuffer(RenderPassHandle renderPassHandle) override;
        ShaderHandle CreateShader(const ShaderSource& shaderSource) override;
        PipelineStateHandle CreateGraphicsPipelineState(const GraphicsPipelineStateCreateInfo& pipelineStateCreateInfo) override;
        PipelineStateHandle CreateComputePipelineState(const ComputePipelineStateCreateInfo& computePipelineStateCreateInfo) override;
        VertexBufferHandle CreateVertexBuffer(void* verticeData, uint32 sizeInBytes, VertexBufferLayout& bufferLayout) override;
        IndexBufferHandle CreateIndexBuffer(uint32* indiceData, uint32 sizeInBytes) override;
        BufferHandle CreateStorageBuffer(void* data, uint32 size) override;
        BufferHandle CreateUniformBuffer(void* data, uint32 size) override;
        DescriptorSetHandle CreateDescriptorSet() override;
        TextureHandle CreateTexture(const TextureCreateInfo& textureCreateInfo) override;
        TextureHandle CreateCubemap(const TextureCreateInfo& textureCreateInfo) override;
        TextureHandle Create3DTexture(const TextureCreateInfo& textureCreateInfo) override;
        TextureHandle Create1DTexture(const TextureCreateInfo& textureCreateInfo) override;
        TextureHandle Create2DTextureArray(const TextureCreateInfo& textureCreateInfo) override;

        bool IsBlitSupportedByFormat(ImageFormat imageFormat) override;
        bool IsAnisotropySupported() override;
        float GetMaxAnisotropySupported() override;

        void WaitIdle() override;

        void* GetNativeHandle() override { return m_Device; }

        VulkanDevice(const VulkanDevice&) = delete;
        VulkanDevice& operator=(const VulkanDevice&) = delete;
        VulkanDevice(VulkanDevice&&) = delete;
        VulkanDevice& operator=(VulkanDevice&&) = delete;

    private:

        void CreateDevice();
        void DestroyDevice();

        void CreateCommandPool();
        void DestroyMemoryPool();

        GraphicsOwnedPtr<Swapchain> CreateSwapchain(uint32 numberOfImages) override;
        void DestroySwapchain();

        VulkanPhysicalDevice m_PhysicalDevice;
        uint32 m_QueueFamilyIndex;
        VkSurfaceKHR m_WindowSurface;
        GLFWwindow* m_Window;

        VkDevice m_Device;
        VkCommandPool m_CommandPool;
        GraphicsOwnedPtr<Swapchain> m_Swapchain;
    };

}
