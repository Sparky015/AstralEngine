/**
* @file VulkanSwapchain.cpp
* @author Andrew Fagan
* @date 5/13/25
*/

#include "VulkanSwapchain.h"

#include "VulkanRenderTarget.h"
#include "Debug/Utilities/Asserts.h"

namespace Astral {

    VulkanSwapchain::VulkanSwapchain(const VulkanSwapchainDesc& vulkanSwapchainDesc) :
        m_Device(vulkanSwapchainDesc.Device),
        m_SelectedPhysicalDevice(vulkanSwapchainDesc.PhysicalDevice),
        m_WindowSurface(vulkanSwapchainDesc.WindowSurface),
        m_QueueFamilyIndex(vulkanSwapchainDesc.QueueFamilyIndex),
        m_NumberOfSwapchainImages(vulkanSwapchainDesc.NumberOfSwapchainImages),
        m_Swapchain(VK_NULL_HANDLE),
        m_SwapchainImageFormat(VK_FORMAT_UNDEFINED),
        m_Images(),
        m_ImageViews(),
        m_RenderCompleteSemaphores(),
        m_ImageAvailableSemaphores(),
        m_CurrentSemaphorePairIndex(0)
    {
        CreateSwapchain();
        CreateSemaphores();
        CreateRenderTargets();
    }


    VulkanSwapchain::~VulkanSwapchain()
    {
        DestroySemaphores();
        DestroySwapchain();
    }


    GraphicsRef<RenderTarget> VulkanSwapchain::AcquireNextImage()
    {
        uint32 imageIndex;
        VkResult result = vkAcquireNextImageKHR(m_Device, m_Swapchain, UINT64_MAX, m_ImageAvailableSemaphores[m_CurrentSemaphorePairIndex], VK_NULL_HANDLE, &imageIndex);
        ASSERT(result == VK_SUCCESS, "Failed to acquire swapchain image!");

        return m_RenderTargets[imageIndex];
    }


    uint32 VulkanSwapchain::ChooseNumSwapchainImages(const VkSurfaceCapabilitiesKHR& capabilities)
    {
        uint32 requestedNumberOfImage = (m_NumberOfSwapchainImages < capabilities.minImageCount) ? capabilities.minImageCount : m_NumberOfSwapchainImages;
        uint32 finalNumberOfImages = 0;

        if (capabilities.maxImageCount > 0 && requestedNumberOfImage > capabilities.maxImageCount)
            { finalNumberOfImages = capabilities.maxImageCount; }
        else
            { finalNumberOfImages = requestedNumberOfImage; }

        return finalNumberOfImages;
    }


    VkPresentModeKHR VulkanSwapchain::ChoosePresentMode(const std::vector<VkPresentModeKHR>& presentModes)
    {
        for (const VkPresentModeKHR& mode : presentModes)
        {
            if (mode == VK_PRESENT_MODE_MAILBOX_KHR)
            {
                return mode;
            }
        }

        return VK_PRESENT_MODE_FIFO_KHR; // Always supported
    }


    VkSurfaceFormatKHR VulkanSwapchain::ChooseSurfaceFormatAndColorSpace(const std::vector<VkSurfaceFormatKHR>& availableFormats)
    {
        for (const VkSurfaceFormatKHR& availableFormat : availableFormats)
        {
            if (availableFormat.format == VK_FORMAT_B8G8R8_SRGB &&
                availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
                {
                    m_SwapchainImageFormat = availableFormat.format;
                    return availableFormat;
                }
        }

        m_SwapchainImageFormat = availableFormats[0].format;
        return availableFormats[0];
    }


    VkImageView VulkanSwapchain::CreateImageView(VkDevice device, VkImage image, VkFormat format, VkImageAspectFlags aspectFlags,
                                VkImageViewType viewType, uint32 layerCount, uint32 mipLevels)
    {
        VkImageViewCreateInfo viewInfo = {
            .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .image = image,
            .viewType = viewType,
            .format = format,
            .components = {
                .r = VK_COMPONENT_SWIZZLE_IDENTITY,
                .g = VK_COMPONENT_SWIZZLE_IDENTITY,
                .b = VK_COMPONENT_SWIZZLE_IDENTITY,
                .a = VK_COMPONENT_SWIZZLE_IDENTITY
            },
            .subresourceRange = {
                .aspectMask = aspectFlags,
                .baseMipLevel = 0,
                .levelCount = mipLevels,
                .baseArrayLayer = 0,
                .layerCount = 1,
            }
        };

        VkImageView imageView;
        VkResult result = vkCreateImageView(device, &viewInfo, nullptr, &imageView);
        ASSERT(result == VK_SUCCESS, "Vulkan failed to create image view!");
        return imageView;
    }


    void VulkanSwapchain::CreateSwapchain()
    {
        const VkSurfaceCapabilitiesKHR surfaceCapabilities = m_SelectedPhysicalDevice.surfaceCapabilities;

        m_NumberOfSwapchainImages = ChooseNumSwapchainImages(surfaceCapabilities);


        const std::vector<VkPresentModeKHR>& presentModes = m_SelectedPhysicalDevice.presentModes;

        VkPresentModeKHR presentMode = ChoosePresentMode(presentModes);
        VkSurfaceFormatKHR surfaceFormat = ChooseSurfaceFormatAndColorSpace(m_SelectedPhysicalDevice.surfaceFormats);

        VkSwapchainCreateInfoKHR swapChainCreateInfo = {
            .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
            .pNext = nullptr,
            .flags = 0,
            .surface = m_WindowSurface,
            .minImageCount = m_NumberOfSwapchainImages,
            .imageFormat = surfaceFormat.format,
            .imageColorSpace = surfaceFormat.colorSpace,
            .imageExtent = surfaceCapabilities.currentExtent,
            .imageArrayLayers = 1,
            .imageUsage = (VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT),
            .imageSharingMode = VK_SHARING_MODE_EXCLUSIVE,
            .queueFamilyIndexCount = 1,
            .pQueueFamilyIndices = &m_QueueFamilyIndex,
            .preTransform = surfaceCapabilities.currentTransform,
            .compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
            .presentMode = presentMode,
            .clipped = VK_TRUE
        };

        VkResult result = vkCreateSwapchainKHR(m_Device, &swapChainCreateInfo, nullptr, &m_Swapchain);
        ASSERT(result == VK_SUCCESS, "Vulkan swapchain failed to create!");

        uint32 numberOfSwapchainImages = 0;
        result = vkGetSwapchainImagesKHR(m_Device, m_Swapchain, &numberOfSwapchainImages, nullptr);
        ASSERT(result == VK_SUCCESS, "Vulkan swapchain failed to get number of images!");

        m_Images.resize(numberOfSwapchainImages);
        m_ImageViews.resize(numberOfSwapchainImages);

        result = vkGetSwapchainImagesKHR(m_Device, m_Swapchain, &numberOfSwapchainImages, m_Images.data());
        ASSERT(result == VK_SUCCESS, "Vulkan swapchain failed to get images!");

        int32 layerCount = 1;
        int mipLevels = 1;

        for (uint32 i = 0; i < numberOfSwapchainImages; i++)
        {
            m_ImageViews[i] = CreateImageView(m_Device, m_Images[i], surfaceFormat.format,
                            VK_IMAGE_ASPECT_COLOR_BIT, VK_IMAGE_VIEW_TYPE_2D, layerCount, mipLevels);
        }

    }


    void VulkanSwapchain::DestroySwapchain()
    {
        for (const VkImageView& imageView : m_ImageViews)
        {
            vkDestroyImageView(m_Device, imageView, nullptr);
        }

        vkDestroySwapchainKHR(m_Device, m_Swapchain, nullptr);
    }


    void VulkanSwapchain::CreateSemaphores()
    {
        VkSemaphoreCreateInfo semaphoreCreateInfo = {
            .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
            .pNext = nullptr,
            .flags = VK_SEMAPHORE_TYPE_BINARY,
        };

        m_ImageAvailableSemaphores.resize(m_NumberOfSwapchainImages);
        m_RenderCompleteSemaphores.resize(m_NumberOfSwapchainImages);

        for (int i = 0; i < m_NumberOfSwapchainImages; i++)
        {
            VkResult result = vkCreateSemaphore(m_Device, &semaphoreCreateInfo, nullptr, &m_RenderCompleteSemaphores[i]);
            ASSERT(result == VK_SUCCESS, "Semaphore failed to create!");
            result = vkCreateSemaphore(m_Device, &semaphoreCreateInfo, nullptr, &m_ImageAvailableSemaphores[i]);
            ASSERT(result == VK_SUCCESS, "Semaphore failed to create!");
        }
    }


    void VulkanSwapchain::DestroySemaphores()
    {
        for (int i = 0; i < m_ImageAvailableSemaphores.size(); i++)
        {
            vkDestroySemaphore(m_Device, m_ImageAvailableSemaphores[i], nullptr);
            vkDestroySemaphore(m_Device, m_RenderCompleteSemaphores[i], nullptr);
        }
    }


    void VulkanSwapchain::CreateRenderTargets()
    {
        for (uint32 i = 0; i < m_NumberOfSwapchainImages; i++)
        {
            VulkanRenderTargetDesc renderTargetDesc = {
                .Image = m_Images[i],
                .ImageView = m_ImageViews[i],
                .ImageIndex = i,
                .ImageAvailableSemaphore = m_ImageAvailableSemaphores[i],
                .RenderCompleteSemaphore = m_RenderCompleteSemaphores[i],
                .Format = m_SwapchainImageFormat,
            };

            m_RenderTargets.emplace_back(CreateGraphicsRef<VulkanRenderTarget>(renderTargetDesc));
        }
    }

}
