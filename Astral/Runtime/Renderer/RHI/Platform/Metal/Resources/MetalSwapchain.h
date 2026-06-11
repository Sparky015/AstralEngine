/**
* @file MetalSwapchain.h
* @author Andrew Fagan
* @date 3/23/26
*/

#pragma once

#include "Renderer/RHI/Resources/Swapchain.h"

#include "QuartzCore/CAMetalLayer.hpp"

namespace Astral {

    struct MetalSwapchainDesc
    {
        CA::MetalLayer* CAMetalLayer;
    };

    /**
     * @brief Manages the Metal swapchain functionality (on top of the CAMetalLayer)
     */
    class MetalSwapchain : public Swapchain
    {
    public:
        MetalSwapchain(const MetalSwapchainDesc& metalSwapchainDesc);
        ~MetalSwapchain() override = default;

        /**
         * @brief Acquires the next image from the CAMetalLayer
         * @return The render target (CA::MetalDrawable)
         */
        GraphicsRef<RenderTarget> AcquireNextImage() override; // TODO

        /**
         * @brief Gets the number of images supported by the swapchain
         * @return The number of images currently supported by the swapchain
         */
        uint32 GetNumberOfImages() override;

        /**
        * @brief Get the swapchain images' format
        * @return The swapchain images' format
        */
        ImageFormat GetImageFormat() override;

        /**
         * @brief Get the swapchain images' dimensions
         * @return The swapchain images' dimensions
         */
        UVec2 GetImageDimensions() override;

        /**
         * @brief Sets the dimensions of the render targets
         * @param width The new width of the render targets
         * @param height The new height of the render targets
         */
        void RecreateSwapchain(uint32 width, uint32 height) override;

        /**
         * @brief Sets the vsync state
         * @param isVSyncEnabled The new state to set the vsync to
         */
        void RecreateSwapchain(bool isVSyncEnabled) override;

        /**
         * @brief Gets the native handle of the swapchain
         * @return Returns the CA::MetalLayer*
         */
        void* GetNativeHandle() override;

    private:

        CA::MetalLayer* m_CAMetalLayer;
    };

}