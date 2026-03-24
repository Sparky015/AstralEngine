/**
* @file MetalSwapchain.cpp
* @author Andrew Fagan
* @date 3/23/26
*/

#include "MetalSwapchain.h"

#include "Debug/Utilities/Asserts.h"

namespace Astral {

    MetalSwapchain::MetalSwapchain(const MetalSwapchainDesc& metalSwapchainDesc) :
        m_CAMetalLayer(metalSwapchainDesc.CAMetalLayer)
    {
        ASSERT(m_CAMetalLayer, "Metal Swapchain can not be created with null CA::MetalLayer")
    }


    GraphicsRef<RenderTarget> MetalSwapchain::AcquireNextImage()
    {
        CA::MetalDrawable* nextDrawable = m_CAMetalLayer->nextDrawable();

        // TODO: Create render target abstraction for Metal
    }


    uint32 MetalSwapchain::GetNumberOfImages()
    {
        return m_CAMetalLayer->maximumDrawableCount();
    }


    std::vector<RenderTargetHandle>& MetalSwapchain::GetRenderTargets()
    {
        // Not possible for metal
    }


    void MetalSwapchain::RecreateSwapchain(uint32 width, uint32 height)
    {
        m_CAMetalLayer->setDrawableSize(CGSizeMake(width, height));
    }


    void MetalSwapchain::RecreateSwapchain(bool isVSyncEnabled)
    {
        m_CAMetalLayer->setDisplaySyncEnabled(isVSyncEnabled);
    }


    void* MetalSwapchain::GetNativeHandle()
    {
        return m_CAMetalLayer;
    }

}

