/**
* @file MetalRenderTarget.cpp
* @author Andrew Fagan
* @date 5/26/26
*/

#include "MetalRenderTarget.h"

#include "MetalTexture.h"
#include "Renderer/RHI/Platform/Metal/Common/MTLEnumConversions.h"

namespace Astral {

    MetalRenderTarget::MetalRenderTarget(const MetalRenderTargetDesc& desc) :
        m_Device(desc.Device),
        m_Drawable(desc.Drawable)
    {
        MetalDrawableOwnedTextureDesc textureDesc = {
            .Device = m_Device,
            .DrawableOwnedTexture = m_Drawable->texture()
        };

        m_DrawableTexture = CreateGraphicsRef<MetalTexture>(textureDesc);
    }


    MetalRenderTarget::~MetalRenderTarget()
    {
    }


    ImageFormat MetalRenderTarget::GetImageFormat()
    {
        return m_DrawableTexture->GetFormat();
    }


    UVec2 MetalRenderTarget::GetDimensions()
    {
        return m_DrawableTexture->GetDimensions();
    }


    uint32 MetalRenderTarget::GetImageIndex()
    {
        // Not applicable in Metal
        return 0;
    }


    void* MetalRenderTarget::GetImageAvailableSemaphore()
    {
        // Not applicable in Metal
        return nullptr;
    }


    void* MetalRenderTarget::GetRenderCompleteSemaphore()
    {
        // Not applicable in Metal
        return nullptr;
    }


    void* MetalRenderTarget::GetFence()
    {
        // Not applicable in Metal
        return nullptr;
    }


    void* MetalRenderTarget::GetNativeImageView()
    {
        return m_Drawable;
    }


    void* MetalRenderTarget::GetNativeImage()
    {
        return m_Drawable;
    }


    TextureHandle MetalRenderTarget::GetAsTexture()
    {
        return m_DrawableTexture;
    }


    void MetalRenderTarget::SetSyncPrimatives(void* renderCompleteSemaphore, void* imageAvailableSemaphore, void* fence)
    {
        // Not applicable in Metal
    }

}
