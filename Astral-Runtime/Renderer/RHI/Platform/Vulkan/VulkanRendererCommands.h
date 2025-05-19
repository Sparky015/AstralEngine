/**
* @file VulkanRendererCommands.h
* @author Andrew Fagan
* @date 3/4/25
*/

#pragma once

#include "Renderer/RHI/RendererCommands.h"
#include "Renderer/RHI/Platform/OpenGL/OpenGLRendererContext.h"
#include "Renderer/RHI/Resources/CommandBuffer.h"
#include "Renderer/RHI/Resources/RenderTarget.h"
#include "Renderer/RHI/Resources/Shader.h"

namespace Astral {

    class VulkanRendererCommands : public RendererCommands
    {
    public:
        void Clear(CommandBufferHandle commandBufferHandle, RenderTargetHandle renderTargetHandle) override;
        void SetClearColor(float r, float g, float b, float a) override;
        void DrawElements(CommandBufferHandle commandBufferHandle) override;
        void SetBlending(bool enable) override;

        void TestInit() override;
        void TestUpdate() override;
        void TestShutdown() override;

    private:

        CommandBufferHandle m_CommandBuffer;
        RenderPassHandle m_RenderPass;
        FramebufferHandle m_Framebuffer;
        ShaderHandle m_VertexShader;
        ShaderHandle m_FragmentShader;
        PipelineStateObjectHandle m_PipelineStateObject;
        DescriptorSetHandle m_DescriptorSet;
        VertexBufferHandle m_VertexBuffer;
    };

}
