/**
* @file Renderpass.h
* @author Andrew Fagan
* @date 5/16/25
*/

#pragma once

#include "Framebuffer.h"
#include "Renderer/RHI/Common/GraphicsSmartPointers.h"

namespace Astral {

    class RenderPass
    {
    public:
        virtual ~RenderPass() = default;

        virtual void BeginRenderPass(CommandBufferHandle commandBufferHandle, FramebufferHandle frameBufferHandle) = 0;
        virtual void EndRenderPass(CommandBufferHandle commandBufferHandle) = 0;

        virtual void* GetNativeHandle() = 0;
    };

    using RenderPassHandle = GraphicsRef<RenderPass>;

}
