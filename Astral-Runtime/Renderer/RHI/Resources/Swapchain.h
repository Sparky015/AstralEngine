/**
* @file Swapchain.h
* @author Andrew Fagan
* @date 5/14/25
*/

#pragma once

#include "Core/FixedIntegerTypes.h"
#include "RenderTarget.h"
#include "Renderer/RHI/Common/GraphicsSmartPointers.h"

#include <vector>

namespace Astral {

    class Swapchain
    {
    public:
        virtual ~Swapchain() = default;

        /**
         * @brief  Acquires the next render target from the swapchain and returns it
         * @return The acquired render target
         */
        virtual GraphicsRef<RenderTarget> AcquireNextImage() = 0;

        /**
         * @brief  Gets the number of render targets in the swapchain
         * @return The number of render targets in the swapchain
         */
        virtual uint32 GetNumberOfImages() = 0;

        /**
         * @brief  Gets the render targets in the swapchain
         * @return The render targets in the swapchain
         */
        virtual std::vector<RenderTargetHandle>& GetRenderTargets() = 0;

        /**
         * @brief Recreates the swapchain with the specified dimensions
         * @param width  The width of the new swapchain images
         * @param height The height of the new swapchain images
         */
        virtual void RecreateSwapchain(uint32 width, uint32 height) = 0;

        /**
         * @brief  Gets the native swapchain handle
         * @return The native swapchain handle
         * @note   The void pointer maps to the native swapchain handle of the selected renderer api backend
         */
        virtual void* GetNativeHandle() = 0;
    };

}
