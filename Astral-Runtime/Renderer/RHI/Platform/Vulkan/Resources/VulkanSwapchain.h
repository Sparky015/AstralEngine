/**
* @file VulkanSwapchain.h
* @author Andrew Fagan
* @date 5/13/25
*/

#pragma once

#include "Core/Events/EventListener.h"
#include "Renderer/RHI/Resources/Swapchain.h"
#include "Renderer/RHI/Resources/RenderTarget.h"
#include "Window/WindowEvents.h"
#include "VulkanDevice.h"
#include "VulkanPhysicalDevices.h"

#include <vulkan/vulkan_core.h>



namespace Astral {

    struct VulkanSwapchainDesc
    {
        VkDevice Device;
        VulkanPhysicalDevice PhysicalDevice;
        VkSurfaceKHR WindowSurface;
        uint32 QueueFamilyIndex;
        uint32 NumberOfSwapchainImages;
    };

    class VulkanSwapchain : public Swapchain
    {
    public:
        VulkanSwapchain(const VulkanSwapchainDesc& vulkanSwapchainDesc);
        ~VulkanSwapchain() override;

        RenderTargetHandle AcquireNextImage() override;

        uint32 GetNumberOfImages() override { return m_NumberOfSwapchainImages; }
        std::vector<RenderTargetHandle>& GetRenderTargets() override { return m_RenderTargets; }

        void RecreateSwapchain(uint32 width, uint32 height) override;

        void* GetNativeHandle() override { return m_Swapchain; }

    private:
        uint32 ChooseNumSwapchainImages(const VkSurfaceCapabilitiesKHR& capabilities);
        VkPresentModeKHR ChoosePresentMode(const std::vector<VkPresentModeKHR>& presentModes);
        VkSurfaceFormatKHR ChooseSurfaceFormatAndColorSpace(const std::vector<VkSurfaceFormatKHR>& availableFormats);
        VkImageView CreateImageView(VkDevice device, VkImage image, VkFormat format, VkImageAspectFlags aspectFlags,
                                VkImageViewType viewType, uint32 layerCount, uint32 mipLevels);

        void CreateSwapchain(uint32 imageWidth, uint32 imageHeight);
        void DestroySwapchain();

        void CreateSemaphores();
        void DestroySemaphores();

        void CreateFences();
        void DestroyFences();

        void CreateRenderTargets();

        VkDevice m_Device;
        VulkanPhysicalDevice m_SelectedPhysicalDevice;
        VkSurfaceKHR m_WindowSurface;
        uint32 m_QueueFamilyIndex;
        uint32 m_NumberOfSwapchainImages;

        VkSwapchainKHR m_Swapchain;
        ImageFormat m_SwapchainImageFormat;
        std::vector<VkImage> m_Images;
        std::vector<VkImageView> m_ImageViews;

        std::vector<VkSemaphore> m_RenderCompleteSemaphores;
        std::vector<VkSemaphore> m_ImageAvailableSemaphores;
        std::vector<VkFence> m_Fences;

        uint32 m_CurrentSemaphorePairIndex;

        std::vector<RenderTargetHandle> m_RenderTargets;
    };

}
