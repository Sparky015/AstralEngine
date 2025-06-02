/**
* @file Framebuffer.h
* @author Andrew Fagan
* @date 5/13/25
*/

#pragma once

#include "Core/Math/Math.h"
#include "Renderer/RHI/Common/GraphicsSmartPointers.h"

namespace Astral {

    class Framebuffer
    {
    public:
        virtual ~Framebuffer() = default;

        virtual UVec2 GetExtent() = 0;

        virtual void* GetNativeHandle() = 0;
    };

    using FramebufferHandle = GraphicsRef<Framebuffer>;

}
