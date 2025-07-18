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

    /**
     * @brief RHI Framebuffer Object
     */
    class Framebuffer
    {
    public:
        virtual ~Framebuffer() = default;

        /**
         * @brief  Gets the extent of the framebuffer
         * @return The extent of the framebuffer
         */
        virtual UVec2 GetExtent() = 0;

        /**
         * @brief Begins the building of a framebuffer
         * @param framebufferWidth Specifies the width of the framebuffer
         * @param framebufferHeight Specifies the height of the framebuffer
         * @post  @ref EndBuildingFramebuffer should be called to create the framebuffer
         */
        virtual void BeginBuildingFramebuffer(uint32 framebufferWidth, uint32 framebufferHeight) = 0;

        /**
         * @brief Attaches a render target to the framebuffer
         * @param renderTargetHandle The render target to attach to the framebuffer
         */
        virtual void AttachRenderTarget(RenderTargetHandle renderTargetHandle) = 0;

        /**
         * @brief Attaches a render target to the framebuffer
         * @param textureHandle The texture to attach to the framebuffer
         */
        virtual void AttachTexture(TextureHandle textureHandle) = 0;

        /**
         * @brief Ends the building of a framebuffer and creates the framebuffer
         * @pre   @ref BeginBuildingFramebuffer should be called to create the framebuffer
         */
        virtual void EndBuildingFramebuffer() = 0;

        /**
         * @brief  Gets the attachment at the specified index
         * @param  attachmentIndex The index of the attachment
         * @return The texture located at the specified index
         */
        virtual TextureHandle GetAttachment(uint32 attachmentIndex) = 0;

        /**
         * @brief Destroys the framebuffer and removes any held textures
         */
        virtual void InvalidateFramebuffer() = 0;

        /**
         * @brief Gets the native framebuffer handle
         * @note  The void pointer maps to the native framebuffer handle of the selected renderer api backend
         */
        virtual void* GetNativeHandle() = 0;
    };

    using FramebufferHandle = GraphicsRef<Framebuffer>;

}
