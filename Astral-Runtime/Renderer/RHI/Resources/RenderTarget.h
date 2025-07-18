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

namespace Astral {

    class RenderTarget
    {
    public:
        virtual ~RenderTarget() = default;

        virtual ImageFormat GetImageFormat() = 0;
        virtual UVec2 GetDimensions() = 0;

        virtual uint32 GetImageIndex() = 0;
        virtual void* GetRenderCompleteSemaphore() = 0;
        virtual void* GetImageAvailableSemaphore() = 0;
        virtual void* GetFence() = 0;
        virtual void* GetImageView() = 0;
        virtual void* GetNativeHandle() = 0;

        virtual TextureHandle GetAsTexture() = 0;

        virtual void SetSyncPrimatives(void* presentCompleteSemaphore, void* imageAvailableSemaphore, void* fence) = 0;
    };

    using RenderTargetHandle = GraphicsRef<RenderTarget>;

}
