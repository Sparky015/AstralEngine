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

    /**
     * @brief Information to create a VulkanSwapchain
     */
    struct VulkanSwapchainDesc
    {
        VkDevice Device;
        VulkanPhysicalDevice PhysicalDevice;
        VkSurfaceKHR WindowSurface;
        uint32 QueueFamilyIndex;
        uint32 NumberOfSwapchainImages;
    };

    /**
     * @brief Vulkan Backend RHI Swapchain Object
     */
    class VulkanSwapchain : public Swapchain
    {
    public:
        VulkanSwapchain(const VulkanSwapchainDesc& vulkanSwapchainDesc);
        ~VulkanSwapchain() override;

        /**
         * @brief  Acquires the next render target from the swapchain and returns it
         * @return The acquired render target
         */
        RenderTargetHandle AcquireNextImage() override;

        /**
         * @brief  Gets the number of render targets in the swapchain
         * @return The number of render targets in the swapchain
         */
        uint32 GetNumberOfImages() override { return m_NumberOfSwapchainImages; }

        /**
         * @brief  Gets the render targets in the swapchain
         * @return The render targets in the swapchain
         */
        std::vector<RenderTargetHandle>& GetRenderTargets() override { return m_RenderTargets; }

        /**
         * @brief Recreates the swapchain with the specified dimensions
         * @param width  The width of the new swapchain images
         * @param height The height of the new swapchain images
         */
        void RecreateSwapchain(uint32 width, uint32 height) override;

        /**
         * @brief  Gets the native swapchain handle
         * @return The native swapchain handle
         * @note   The void pointer maps to the native vulkan swapchain handle (VkSwapchainKHR)
         */
        void* GetNativeHandle() override { return m_Swapchain; }

    private:

        /**
         * @brief Determines the number of swapchain images for what the surface is capable of
         * @param capabilities The capabilities of the surface
         * @return The selected number of swapchain images
         */
        uint32 ChooseNumSwapchainImages(const VkSurfaceCapabilitiesKHR& capabilities);

        /**
         * @brief Chooses the present mode that is valid for the physical device
         * @param presentModes All the present modes the physical device is capable of
         * @return The selected present mode
         */
        VkPresentModeKHR ChoosePresentMode(const std::vector<VkPresentModeKHR>& presentModes);

        /**
         * @brief Chooses the surface format and color space for the swapchain images that is valid for the physical device
         * @param availableFormats All the surface formats the physical device is capable of
         * @return The selected surface format
         */
        VkSurfaceFormatKHR ChooseSurfaceFormatAndColorSpace(const std::vector<VkSurfaceFormatKHR>& availableFormats);

        /**
         * @brief Creates a view of a swapchain image
         * @param device The logical device
         * @param image The swapchain image
         * @param format The format of the swapchain image
         * @param aspectFlags The aspect flags of the swapchain image
         * @param viewType The view type
         * @param layerCount The layer count to create a view of
         * @param mipLevels The mip levels to create a view of
         */
        VkImageView CreateImageView(VkDevice device, VkImage image, VkFormat format, VkImageAspectFlags aspectFlags,
                                VkImageViewType viewType, uint32 layerCount, uint32 mipLevels);

        /**
         * @brief Creates (or recreates) the swapchain
         * @param imageWidth The width of the swapchain images
         * @param imageWidth The height of the swapchain images
         */
        void CreateSwapchain(uint32 imageWidth, uint32 imageHeight);

        /**
         * @brief Destroys the swapchain
         */
        void DestroySwapchain();

        /**
         * @brief Creates semaphores for the swapchain images
         */
        void CreateSemaphores();

        /**
         * @brief Destroys semaphores for the swapchain images
         */
        void DestroySemaphores();

        /**
         * @brief Creates fences for the swapchain images
         */
        void CreateFences();

        /**
         * @brief Destroys fences for the swapchain images
         */
        void DestroyFences();

        /**
         * @brief Creates render target instances for the swapchain images
         */
        void CreateRenderTargets();

        /**
         * @brief Destroys the render target instances for the swapchain images
         */
        void DestroyRenderTargets();

        VkDevice m_Device;
        VulkanPhysicalDevice m_SelectedPhysicalDevice;
        VkSurfaceKHR m_WindowSurface;
        uint32 m_QueueFamilyIndex;
        uint32 m_NumberOfSwapchainImages;

        VkSwapchainKHR m_Swapchain;
        ImageFormat m_SwapchainImageFormat;
        std::vector<VkImage> m_Images;
        std::vector<VkImageView> m_ImageViews;
        UVec2 m_ImageDimensions;

        std::vector<VkSemaphore> m_RenderCompleteSemaphores;
        std::vector<VkSemaphore> m_ImageAvailableSemaphores;
        std::vector<VkFence> m_Fences;
        uint32 m_CurrentSemaphorePairIndex;

        std::vector<RenderTargetHandle> m_RenderTargets;
    };

}
