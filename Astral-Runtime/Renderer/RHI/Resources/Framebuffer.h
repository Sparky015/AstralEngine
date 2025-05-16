/**
* @file Framebuffer.h
* @author Andrew Fagan
* @date 5/13/25
*/

#pragma once
#include "Renderer/RHI/Common/GraphicsSmartPointers.h"

namespace Astral {

    class Framebuffer
    {
    public:
        virtual ~Framebuffer() = default;


        virtual void* GetNativeHandle() = 0;
    };

    using FramebufferHandle = GraphicsRef<Framebuffer>;

}
