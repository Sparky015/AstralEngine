/**
* @file Framebuffer.h
* @author Andrew Fagan
* @date 5/13/25
*/

#pragma once

#include "Core/Math/Math.h"
#include "Renderer/RHI/Common/GraphicsSmartPointers.h"
#include "Renderer/RHI/Resources/Texture.h"
#include "Renderer/RHI/Resources/RenderTarget.h"

namespace Astral {

    class Framebuffer
    {
    public:
        virtual ~Framebuffer() = default;

        virtual UVec2 GetExtent() = 0;

        virtual void BeginBuildingFramebuffer(uint32 framebufferWidth, uint32 framebufferHeight) = 0;
        virtual void AttachRenderTarget(RenderTargetHandle renderTargetHandle) = 0;
        virtual void AttachTexture(TextureHandle textureHandle) = 0;
        virtual void EndBuildingFramebuffer() = 0;

        virtual TextureHandle GetAttachment(uint32 attachmentIndex) = 0;

        virtual void InvalidateFramebuffer() = 0;

        virtual void* GetNativeHandle() = 0;
    };

    using FramebufferHandle = GraphicsRef<Framebuffer>;

}
