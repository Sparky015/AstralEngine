/**
* @file VulkanDevice.h
* @author Andrew Fagan
* @date 5/13/25
*/

#pragma once

#include "Core/CoreMacroDefinitions.h"
#include "Renderer/RHI/Resources/Device.h"
#include "VulkanPhysicalDevices.h"

#include <vulkan/vulkan_core.h>
#include <GLFW/glfw3.h>

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

        Swapchain& GetSwapchain() override { return *m_Swapchain; }
        CommandBufferHandle AllocateCommandBuffer() override;
        CommandQueueHandle GetCommandQueue() override;
        RenderPassHandle CreateRenderPass(RenderTargetHandle renderTargetHandle) override;
        FramebufferHandle CreateFramebuffer(RenderPassHandle renderPassHandle, RenderTargetHandle renderTargetHandle) override;
        ShaderHandle CreateShader(const ShaderSource& shaderSource) override;
        PipelineStateObjectHandle CreatePipelineStateObject(RenderPassHandle renderPassHandle, ShaderHandle vertexShader, ShaderHandle fragmentShader) override;

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
