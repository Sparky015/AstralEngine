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

namespace Astral {

    class VulkanDevice : public Device
    {
    public:
        explicit VulkanDevice(VulkanPhysicalDevice physicalDevice, uint32 queueFamilyIndex, VkSurfaceKHR windowSurface);
        ~VulkanDevice() override;

        Swapchain* CreateSwapchain() override;
        CommandBuffer* AllocateCommandBuffer() override;
        CommandQueue* GetCommandQueue() override;



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

        VulkanPhysicalDevice m_PhysicalDevice;
        VkDevice m_Device;
        uint32 m_QueueFamilyIndex;
        VkSurfaceKHR m_WindowSurface;
        VkCommandPool m_CommandPool;
    };

}
