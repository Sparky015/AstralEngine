/**
* @file RenderTarget.h
* @author Andrew Fagan
* @date 5/13/25
*/

#pragma once

#include "Core/FixedIntegerTypes.h"
#include "Core/Math/Math.h"
#include "Renderer/RHI/Common/GraphicsSmartPointers.h"
#include "Renderer/RHI/Common/ImageFormats.h"
#include "Texture.h"

namespace Astral {

    /**
    * @brief Information to create a render target from
    */
    class RenderTarget
    {
    public:
        virtual ~RenderTarget() = default;

        /**
         * @brief  Gets the image format of the render target
         * @return The image format of the render target
         */
        virtual ImageFormat GetImageFormat() = 0;

        /**
         * @brief  Gets the render target dimensions
         * @return The render target dimensions
         */
        virtual UVec2 GetDimensions() = 0;

        /**
         * @brief  Gets the index of the render target in the swapchain
         * @return The index of the render target in the swapchain
         */
        virtual uint32 GetImageIndex() = 0;

        /**
         * @brief  Gets the render-complete semaphore for the render target
         * @return The render-complete semaphore for the render target
         * @note   The void pointer maps to the native semaphore handle of the selected renderer api backend
         */
        virtual void* GetRenderCompleteSemaphore() = 0;

        /**
         * @brief  Gets the image-available semaphore for the render target
         * @return The render-complete semaphore for the render target
         * @note   The void pointer maps to the native semaphore handle of the selected renderer api backend
         */
        virtual void* GetImageAvailableSemaphore() = 0;

        /**
         * @brief  Gets the fence for the render target
         * @return The fence for the render target
         * @note   The void pointer maps to the native fence handle of the selected renderer api backend
         */
        virtual void* GetFence() = 0;

        /**
         * @brief  Gets the native image view handle of the render target
         * @return The native image view of the render target
         * @note   The void pointer maps to the native image view handle of the selected renderer api backend
         */
        virtual void* GetNativeImageView() = 0;

        /**
         * @brief  Gets the native image handle of the texture
         * @return The native image handle of the texture
         * @note   The void pointer maps to the native image handle of the selected renderer api backend
         */
        virtual void* GetNativeImage() = 0;

        /**
         * @brief Gets the render target as a texture
         * @return A texture that contains the render target image
         */
        virtual TextureHandle GetAsTexture() = 0;

        /**
         * @brief   Assigns semaphores and fences to the render target
         * @param   renderCompleteSemaphore The render-complete semaphore to assign to the render target
         * @param   imageAvailableSemaphore The image-available semaphore to assign to the render target
         * @param   fence The fence to assign to the render target
         * @warning This should only be called by the swapchain!
         */
        virtual void SetSyncPrimatives(void* renderCompleteSemaphore, void* imageAvailableSemaphore, void* fence) = 0;
    };

    using RenderTargetHandle = GraphicsRef<RenderTarget>;

}
