/**
* @file Device.h
* @author Andrew Fagan
* @date 5/13/25
*/

#pragma once
#include "CommandQueue.h"
#include "Framebuffer.h"
#include "Renderpass.h"
#include "Shader.h"
#include "Swapchain.h"
#include "Renderer/RHI/Resources/CommandBuffer.h"

namespace Astral {

    class Device
    {
    public:
        virtual ~Device() = default;

        virtual Swapchain& GetSwapchain() = 0;
        virtual CommandBufferHandle AllocateCommandBuffer() = 0;
        virtual CommandQueueHandle GetCommandQueue() = 0;
        virtual RenderPassHandle CreateRenderPass(RenderTargetHandle renderTargetHandle) = 0;
        virtual FramebufferHandle CreateFramebuffer(RenderPassHandle renderPassHandle, RenderTargetHandle renderTargetHandle) = 0;
        virtual ShaderHandle CreateShader(const ShaderSource& shaderSource) = 0;

        virtual void* GetNativeHandle() = 0;

    protected:

        virtual GraphicsOwnedPtr<Swapchain> CreateSwapchain(uint32 numberOfImages) = 0;
    };

}
