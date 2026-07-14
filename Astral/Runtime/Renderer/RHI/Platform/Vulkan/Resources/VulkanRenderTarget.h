/**
* @file VulkanRenderTarget.h
* @author Andrew Fagan
* @date 5/15/2025
*/

#pragma once

#include "Core/CoreMacroDefinitions.h"
#include "Renderer/RHI/Resources/RenderTarget.h"
#include "VulkanTexture.h"

#include <vulkan/vulkan_core.h>

namespace Astral {

    /**
    * @brief Information to create a VulkanRenderTarget from
    */
    struct VulkanRenderTargetDesc
    {
        VkDevice Device;
        VkImage Image;
        VkImageView ImageView;
        ImageFormat Format;
        ImageLayout Layout;
        uint32 ImageIndex;
        VkSemaphore ImageAvailableSemaphore;
        VkSemaphore RenderCompleteSemaphore;
        uint32 ImageWidth;
        uint32 ImageHeight;
    };

    /**
     * @brief Vulkan Backend RHI Render Target Object
     */
    class VulkanRenderTarget : public RenderTarget
    {
    public:
        explicit VulkanRenderTarget(const VulkanRenderTargetDesc& desc);
        ~VulkanRenderTarget() override;

        /**
         * @brief  Gets the image format of the render target
         * @return The image format of the render target
         */
        ImageFormat GetImageFormat() override { return m_ImageFormat; }

        /**
         * @brief  Gets the render target dimensions
         * @return The render target dimensions
         */
        UVec2 GetDimensions() override { return UVec2(m_ImageWidth, m_ImageHeight); }

        /**
         * @brief  Gets the index of the render target in the swapchain
         * @return The index of the render target in the swapchain
         */
        uint32 GetImageIndex() override { return m_ImageIndex; }

        /**
         * @brief  Gets the render-complete semaphore for the render target
         * @return The render-complete semaphore for the render target
         * @note   The void pointer maps to the native semaphore handle of the selected renderer api backend
         */
        void* GetImageAvailableSemaphore() override { return m_ImageAvailableSemaphore; }

        /**
         * @brief  Gets the image-available semaphore for the render target
         * @return The render-complete semaphore for the render target
         * @note   The void pointer maps to the native semaphore handle of the selected renderer api backend
         */
        void* GetRenderCompleteSemaphore() override { return m_RenderCompleteSemaphore; }

        /**
         * @brief  Gets the fence for the render target
         * @return The fence for the render target
         * @note   The void pointer maps to the native fence handle of the selected renderer api backend
         */
        void* GetFence() override { return m_Fence; }

        /**
         * @brief  Gets the native image view handle of the render target
         * @return The native image view of the render target
         * @note   The void pointer maps to the native image view handle of the selected renderer api backend
         */
        void* GetNativeImageView() override { return m_ImageView; }

        /**
         * @brief  Gets the native image handle of the texture
         * @return The native image handle of the texture
         * @note   The void pointer maps to the native image handle of the selected renderer api backend
         */
        void* GetNativeImage() override { return m_Image; }

        /**
         * @brief Gets the render target as a texture
         * @return A texture that contains the render target image
         */
        TextureHandle GetAsTexture() override { return m_RenderTargetTexture; }

        /**
         * @brief   Assigns semaphores and fences to the render target
         * @param   renderCompleteSemaphore The render-complete semaphore to assign to the render target
         * @param   imageAvailableSemaphore The image-available semaphore to assign to the render target
         * @param   fence The fence to assign to the render target
         * @warning This should only be called by the swapchain!
         */
        void SetSyncPrimatives(void* renderCompleteSemaphore, void* imageAvailableSemaphore, void* fence) override
        {
            m_ImageAvailableSemaphore = (VkSemaphore)imageAvailableSemaphore;
            m_RenderCompleteSemaphore = (VkSemaphore)renderCompleteSemaphore;
            m_Fence = (VkFence)fence;
        }

    private:


        VkImage m_Image;
        VkImageView m_ImageView;
        ImageFormat m_ImageFormat;
        ImageLayout m_ImageLayout;

        TextureHandle m_RenderTargetTexture;

        uint32 m_ImageIndex;
        VkSemaphore m_RenderCompleteSemaphore;
        VkSemaphore m_ImageAvailableSemaphore;
        VkFence m_Fence;

        uint32 m_ImageWidth;
        uint32 m_ImageHeight;
    };

}
