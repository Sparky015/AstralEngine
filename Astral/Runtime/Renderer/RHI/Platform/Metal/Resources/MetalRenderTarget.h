/**
* @file MetalRenderTarget.h
* @author Andrew Fagan
* @date 5/26/26
*/

#pragma once

#include "Renderer/RHI/Resources/RenderTarget.h"
#include "Renderer/RHI/Resources/Texture.h"

namespace Astral {

    /**
    * @brief Information to create a VulkanRenderTarget from
    */
    struct MetalRenderTargetDesc
    {

    };

    /**
     * @brief Vulkan Backend RHI Render Target Object
     */
    class MetalRenderTarget : public RenderTarget
    {
    public:
        explicit MetalRenderTarget(const MetalRenderTargetDesc& desc);
        ~MetalRenderTarget() override;

        /**
         * @brief  Gets the image format of the render target
         * @return The image format of the render target
         */
        ImageFormat GetImageFormat() override;

        /**
         * @brief  Gets the render target dimensions
         * @return The render target dimensions
         */
        UVec2 GetDimensions() override;

        /**
         * @brief  Gets the index of the render target in the swapchain
         * @return The index of the render target in the swapchain
         */
        uint32 GetImageIndex() override;

        /**
         * @brief  Gets the render-complete semaphore for the render target
         * @return The render-complete semaphore for the render target
         * @note   The void pointer maps to the native semaphore handle of the selected renderer api backend
         */
        void* GetImageAvailableSemaphore() override;

        /**
         * @brief  Gets the image-available semaphore for the render target
         * @return The render-complete semaphore for the render target
         * @note   The void pointer maps to the native semaphore handle of the selected renderer api backend
         */
        void* GetRenderCompleteSemaphore() override;

        /**
         * @brief  Gets the fence for the render target
         * @return The fence for the render target
         * @note   The void pointer maps to the native fence handle of the selected renderer api backend
         */
        void* GetFence() override;

        /**
         * @brief  Gets the native image view handle of the render target
         * @return The native image view of the render target
         * @note   The void pointer maps to the native image view handle of the selected renderer api backend
         */
        void* GetNativeImageView() override;

        /**
         * @brief  Gets the native image handle of the texture
         * @return The native image handle of the texture
         * @note   The void pointer maps to the native image handle of the selected renderer api backend
         */
        void* GetNativeImage() override;

        /**
         * @brief Gets the render target as a texture
         * @return A texture that contains the render target image
         */
        TextureHandle GetAsTexture() override;

        /**
         * @brief   Assigns semaphores and fences to the render target
         * @param   renderCompleteSemaphore The render-complete semaphore to assign to the render target
         * @param   imageAvailableSemaphore The image-available semaphore to assign to the render target
         * @param   fence The fence to assign to the render target
         * @warning This should only be called by the swapchain!
         */
        void SetSyncPrimatives(void* renderCompleteSemaphore, void* imageAvailableSemaphore, void* fence) override;

    private:

    };

}