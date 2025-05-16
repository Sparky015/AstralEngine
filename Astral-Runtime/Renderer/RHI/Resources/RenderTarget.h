/**
* @file RenderTarget.h
* @author Andrew Fagan
* @date 5/13/25
*/

#pragma once

#include "Core/CoreMacroDefinitions.h"
#include "Renderer/RHI/Common/GraphicsSmartPointers.h"

namespace Astral {

    class RenderTarget
    {
    public:
        virtual ~RenderTarget() = default;
        virtual uint32 GetImageIndex() = 0;
        virtual void* GetRenderCompleteSemaphore() = 0;
        virtual void* GetImageAvailableSemaphore() = 0;
        virtual void* GetNativeHandle() = 0;
    };

    using RenderTargetHandle = GraphicsRef<RenderTarget>;

}
