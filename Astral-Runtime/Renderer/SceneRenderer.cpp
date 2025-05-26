//
// Created by Andrew Fagan on 11/29/24.
//

#include "SceneRenderer.h"

#include "Astral.h"
#include "RHI/RendererAPI.h"

#include "Renderer/RHI/Resources/CommandBuffer.h"
#include "Renderer/RHI/Resources/RenderTarget.h"
#include "Renderer/RHI/Resources/Shader.h"
#include "Renderer/RendererManager.h"

#include "stb_image.h"


namespace Astral {

    GraphicsOwnedPtr<SceneRenderer::SceneRendererContext> SceneRenderer::m_RendererContext = nullptr;

    void SceneRenderer::BeginScene(const OrthographicCamera& orthographicCamera)
    {
        RendererAPI::SetBlending(true);
    }


    void SceneRenderer::EndScene()
    {

    }


    void SceneRenderer::Submit(Mesh& mesh, Material& material, Mat4& transform)
    {
        Device& device = RendererAPI::GetDevice();
        CommandBufferHandle commandBuffer = device.AllocateCommandBuffer();
        RendererAPI::DrawElementsIndexed(commandBuffer, mesh.IndexBuffer);
        CommandQueueHandle commandQueue = device.GetCommandQueue();
    }


    uint32 SceneRenderer::GetDrawCallsPerFrame()
    {
        return RendererAPI::s_RendererCommands->GetNumberOfDrawCalls();
    }


    API SceneRenderer::GetRendererAPIBackend()
    {
        return RendererAPI::s_RendererCommands->GetAPI();
    }


    void SceneRenderer::TestInit()
    {
        CommandBufferHandle m_CommandBuffer;
        RenderPassHandle m_RenderPass;
        FramebufferHandle m_Framebuffer;
        ShaderHandle m_VertexShader;
        ShaderHandle m_FragmentShader;
        PipelineStateObjectHandle m_PipelineStateObject;
        DescriptorSetHandle m_DescriptorSet;
        VertexBufferHandle m_VertexBuffer;
        IndexBufferHandle m_IndexBuffer;
        TextureHandle m_Texture;

        Device& device = RendererAPI::GetDevice();
        RenderTargetHandle renderTarget = device.GetSwapchain().AcquireNextImage();
        m_CommandBuffer = device.AllocateCommandBuffer();
        m_RenderPass = device.CreateRenderPass(renderTarget);
        m_Framebuffer = device.CreateFramebuffer(m_RenderPass, renderTarget);

        ShaderSource vertexSource = ShaderSource( "SecondTriangle.vert");
        ShaderSource fragmentSource = ShaderSource( "SecondTriangle.frag");


        std::vector<VertexBuffer::Vertex> Vertices = {
            {{0.0f, -0.5f}},
            {{0.5f, 0.5f}},
            {{-0.5f, 0.5f}}
        };

        std::vector<uint32_t> Indices = {
            0, 1, 2
        };


        m_VertexBuffer = device.CreateVertexBuffer(Vertices.data(), sizeof(Vertices[0]) * Vertices.size() );
        m_IndexBuffer = device.CreateIndexBuffer(Indices.data(), sizeof(Indices[0]) * Indices.size());
        m_VertexShader = device.CreateShader(vertexSource);
        m_FragmentShader = device.CreateShader(fragmentSource);
        m_DescriptorSet = device.CreateDescriptorSet();


        Vec4 color = {1.0f, 0.0f, 1.0f, 1.0f};
        BufferHandle storageBuffer = device.CreateStorageBuffer(&color, sizeof(color));

        float mult = .5f;
        BufferHandle uniformBuffer = device.CreateUniformBuffer(&mult, sizeof(mult));

        int m_Width;
        int m_Height;
        int m_BPP; // bits per pixel
        stbi_set_flip_vertically_on_load(1);
        std::string filePath = std::string(SHADER_DIR) + "water.jpeg";
        unsigned char* data = stbi_load(filePath.c_str(), &m_Width, &m_Height, &m_BPP, 4);
        m_Texture = device.CreateTexture(data, m_Width, m_Height);

        m_DescriptorSet->BeginBuildingSet();
        m_DescriptorSet->AddDescriptorStorageBuffer(storageBuffer, ShaderStage::FRAGMENT);
        m_DescriptorSet->AddDescriptorUniformBuffer(uniformBuffer, ShaderStage::FRAGMENT);
        m_DescriptorSet->AddDescriptorImageSampler(m_Texture, ShaderStage::FRAGMENT);
        m_DescriptorSet->EndBuildingSet();


        m_PipelineStateObject = device.CreatePipelineStateObject(m_RenderPass, m_VertexShader, m_FragmentShader, m_DescriptorSet);

        m_CommandBuffer->BeginRecording();
        m_RenderPass->BeginRenderPass(m_CommandBuffer, m_Framebuffer);
        m_PipelineStateObject->Bind(m_CommandBuffer);
        m_PipelineStateObject->BindDescriptorSet(m_CommandBuffer, m_DescriptorSet);
        m_VertexBuffer->Bind(m_CommandBuffer);
        m_IndexBuffer->Bind(m_CommandBuffer);
        RendererAPI::DrawElementsIndexed(m_CommandBuffer, m_IndexBuffer);
        m_RenderPass->EndRenderPass(m_CommandBuffer);
        m_CommandBuffer->EndRecording();

        CommandQueueHandle commandQueue = device.GetCommandQueue();
        commandQueue->Submit(m_CommandBuffer, renderTarget);
        commandQueue->Present(renderTarget);

        device.WaitIdle();
    }


    void SceneRenderer::TestShutdown()
    {

    }


    void SceneRenderer::TestUpdate()
    {

    }


    void SceneRenderer::RenderScene()
    {

    }


    void SceneRenderer::RenderPassStart()
    {

    }


    void SceneRenderer::RenderPassEnd()
    {

    }

} // Renderer