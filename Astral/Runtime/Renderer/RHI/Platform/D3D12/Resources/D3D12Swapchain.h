/**
* @file D3D12Swapchain.h
* @author Andrew Fagan
* @date 3/24/2026
*/

#pragma once

#include "Renderer/RHI/Resources/Swapchain.h"

namespace Astral {

    struct D3D12SwapchainDesc
    {
        // TODO
    };

    class D3D12Swapchain : public Swapchain
    {
    public:
        D3D12Swapchain(const D3D12SwapchainDesc& d3d12SwapchainDesc); // TODO
        ~D3D12Swapchain() override = default;

        GraphicsRef<RenderTarget> AcquireNextImage() override; // TODO

        uint32 GetNumberOfImages() override; // TODO

        std::vector<RenderTargetHandle>& GetRenderTargets() override; // TODO

        void RecreateSwapchain(uint32 width, uint32 height) override; // TODO

        void RecreateSwapchain(bool isVSyncEnabled) override; // TODO

        void* GetNativeHandle() override; // TODO

    private:

    };

}