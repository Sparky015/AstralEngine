/**
* @file MetalSwapchain.h
* @author Andrew Fagan
* @date 3/23/26
*/

#pragma once

#include "Renderer/RHI/Resources/Swapchain.h"

namespace Astral {

    struct MetalSwapchainDesc
    {
        // TODO
    };

    class MetalSwapchain : public Swapchain
    {
    public:
        MetalSwapchain(const MetalSwapchainDesc& metalSwapchainDesc); // TODO
        ~MetalSwapchain() override = default;

        GraphicsRef<RenderTarget> AcquireNextImage() override; // TODO

        uint32 GetNumberOfImages() override; // TODO

        std::vector<RenderTargetHandle>& GetRenderTargets() override; // TODO

        void RecreateSwapchain(uint32 width, uint32 height) override; // TODO

        void RecreateSwapchain(bool isVSyncEnabled) override; // TODO

        void* GetNativeHandle() override; // TODO

    private:

    };

}