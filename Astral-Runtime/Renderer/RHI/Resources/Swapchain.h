/**
* @file Swapchain.h
* @author Andrew Fagan
* @date 5/14/25
*/

#pragma once

#include "Core/CoreMacroDefinitions.h"
#include "RenderTarget.h"
#include "Renderer/RHI/Common/GraphicsSmartPointers.h"

namespace Astral {

    class Swapchain
    {
    public:
        virtual ~Swapchain() = default;

        virtual GraphicsRef<RenderTarget> AcquireNextImage() = 0;

        virtual void* GetNativeHandle() = 0;
    };

}
