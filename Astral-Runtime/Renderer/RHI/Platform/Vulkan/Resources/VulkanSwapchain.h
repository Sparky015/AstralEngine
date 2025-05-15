/**
* @file VulkanSwapchain.h
* @author Andrew Fagan
* @date 5/13/25
*/

#pragma once

#include "Renderer/RHI/Resources/Swapchain.h"
#include "VulkanPhysicalDevices.h"

#include <vulkan/vulkan_core.h>

#include "Renderer/RHI/Resources/Device.h"

namespace Astral {

    struct VulkanSwapchainDesc
    {
        Device& Device;
        VkPhysicalDevice PhysicalDevice;
        VkSurfaceKHR WindowSurface;
        uint32 QueueFamilyIndex;
    };

    class VulkanSwapchain : public Swapchain
    {
    public:
        VulkanSwapchain(const VulkanSwapchainDesc& vulkanSwapchainDesc);
        ~VulkanSwapchain();

        uint32 AcquireNextImage() override;

    private:
        uint32 ChooseNumSwapchainImages(const VkSurfaceCapabilitiesKHR& capabilities);
        VkPresentModeKHR ChoosePresentMode(const std::vector<VkPresentModeKHR>& presentModes);
        VkSurfaceFormatKHR ChooseSurfaceFormatAndColorSpace(const std::vector<VkSurfaceFormatKHR>& availableFormats);
        VkImageView CreateImageView(VkDevice device, VkImage image, VkFormat format, VkImageAspectFlags aspectFlags,
                                VkImageViewType viewType, uint32 layerCount, uint32 mipLevels);

        void CreateSwapchain();
        void DestroySwapchain();

        void CreateSemaphores(Device& device);

        VkDevice m_Device;
        VkSurfaceKHR m_WindowSurface;
        VulkanPhysicalDevice m_SelectedPhysicalDevice;
        uint32 m_QueueFamilyIndex;
        VkSwapchainKHR m_Swapchain;
        std::vector<VkImage> m_Images;
        std::vector<VkImageView> m_ImageViews;
        std::vector<VkSemaphore> m_RenderCompleteSemaphores;
        std::vector<VkSemaphore> m_PresentCompleteSemaphores;
        uint32 m_NumberOfSwapchainImages;
    };

}
