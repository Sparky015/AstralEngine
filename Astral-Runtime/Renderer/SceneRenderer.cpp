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
#include <glm/gtc/type_ptr.hpp>

namespace Astral {

    GraphicsOwnedPtr<SceneRenderer::SceneRendererContext> SceneRenderer::m_RendererContext = nullptr;


    void SceneRenderer::Init()
    {
        m_RendererContext = CreateGraphicsOwnedPtr<SceneRendererContext>();

        Device& device = RendererAPI::GetDevice();
        Swapchain& swapchain = device.GetSwapchain();

        std::vector<RenderTargetHandle> renderTargets = swapchain.GetRenderTargets();

        // TODO: Change this to passing in a format enum value instead of a whole render target object
        m_RendererContext->RenderPass = device.CreateRenderPass(renderTargets[0]);

        for (int i = 0; i < swapchain.GetNumberOfImages(); i++)
        {
            m_RendererContext->FrameContexts.emplace_back(FrameContext());
            FrameContext& context = m_RendererContext->FrameContexts[i];

            context.Meshes = std::vector<Mesh>();
            context.Materials = std::vector<Material>();
            context.Transforms = std::vector<Mat4>();
            context.SceneCommandBuffer = device.AllocateCommandBuffer();
            context.SceneFramebuffer = device.CreateFramebuffer(m_RendererContext->RenderPass, renderTargets[i]);
            context.TempPipelineState = nullptr;
            context.TempDescriptorSets = std::vector<DescriptorSetHandle>();
            context.SceneRenderTarget = nullptr;
        }

        g_RendererManager.GetContext().InitImGuiForAPIBackend(m_RendererContext->RenderPass);

        // TESTING CODE

        // CommandBufferHandle m_CommandBuffer;
        // RenderPassHandle m_RenderPass;
        // FramebufferHandle m_Framebuffer;
        // ShaderHandle m_VertexShader;
        // ShaderHandle m_FragmentShader;
        // PipelineStateObjectHandle m_PipelineStateObject;
        // DescriptorSetHandle m_DescriptorSet;
        // VertexBufferHandle m_VertexBuffer;
        // IndexBufferHandle m_IndexBuffer;
        // TextureHandle m_Texture;
        //
        //
        // RenderTargetHandle renderTarget = swapchain.AcquireNextImage();
        //
        // m_CommandBuffer = device.AllocateCommandBuffer();
        // m_RenderPass = device.CreateRenderPass(renderTarget);
        // m_Framebuffer = device.CreateFramebuffer(m_RenderPass, renderTarget);
        //
        // ShaderSource vertexSource = ShaderSource( "SecondTriangle.vert");
        // ShaderSource fragmentSource = ShaderSource( "SecondTriangle.frag");
        //
        //
        // std::vector<VertexBuffer::Vertex> Vertices = {
        //     {{0.0f, -0.5f}},
        //     {{0.5f, 0.5f}},
        //     {{-0.5f, 0.5f}}
        // };
        //
        // std::vector<uint32_t> Indices = {
        //     0, 1, 2
        // };
        //
        //
        // m_VertexBuffer = device.CreateVertexBuffer(Vertices.data(), sizeof(Vertices[0]) * Vertices.size() );
        // m_IndexBuffer = device.CreateIndexBuffer(Indices.data(), sizeof(Indices[0]) * Indices.size());
        // m_VertexShader = device.CreateShader(vertexSource);
        // m_FragmentShader = device.CreateShader(fragmentSource);
        // m_DescriptorSet = device.CreateDescriptorSet();
        //
        //
        // Vec4 color = {1.0f, 0.0f, 1.0f, 1.0f};
        // BufferHandle storageBuffer = device.CreateStorageBuffer(&color, sizeof(color));
        //
        // float mult = .5f;
        // BufferHandle uniformBuffer = device.CreateUniformBuffer(&mult, sizeof(mult));
        //
        // int m_Width;
        // int m_Height;
        // int m_BPP; // bits per pixel
        // stbi_set_flip_vertically_on_load(1);
        // std::string filePath = std::string(SHADER_DIR) + "water.jpeg";
        // unsigned char* data = stbi_load(filePath.c_str(), &m_Width, &m_Height, &m_BPP, 4);
        // m_Texture = device.CreateTexture(data, m_Width, m_Height);
        //
        // m_DescriptorSet->BeginBuildingSet();
        // m_DescriptorSet->AddDescriptorStorageBuffer(storageBuffer, ShaderStage::FRAGMENT);
        // m_DescriptorSet->AddDescriptorUniformBuffer(uniformBuffer, ShaderStage::FRAGMENT);
        // m_DescriptorSet->AddDescriptorImageSampler(m_Texture, ShaderStage::FRAGMENT);
        // m_DescriptorSet->EndBuildingSet();
        //
        //
        // m_PipelineStateObject = device.CreatePipelineStateObject(m_RenderPass, m_VertexShader, m_FragmentShader, m_DescriptorSet);
        //
        // m_CommandBuffer->BeginRecording();
        // m_RenderPass->BeginRenderPass(m_CommandBuffer, m_Framebuffer);
        // m_PipelineStateObject->Bind(m_CommandBuffer);
        // m_PipelineStateObject->BindDescriptorSet(m_CommandBuffer, m_DescriptorSet);
        // m_VertexBuffer->Bind(m_CommandBuffer);
        // m_IndexBuffer->Bind(m_CommandBuffer);
        // RendererAPI::DrawElementsIndexed(m_CommandBuffer, m_IndexBuffer);
        // m_RenderPass->EndRenderPass(m_CommandBuffer);
        // m_CommandBuffer->EndRecording();
        //
        // CommandQueueHandle commandQueue = device.GetCommandQueue();
        // commandQueue->Submit(m_CommandBuffer, renderTarget);
        // commandQueue->Present(renderTarget);
        //
        // device.WaitIdle();
    }


    void SceneRenderer::Shutdown()
    {
        Device& device = RendererAPI::GetDevice();
        device.WaitIdle();

        g_RendererManager.GetContext().ShutdownImGuiForAPIBackend();
        m_RendererContext->FrameContexts.clear();
        m_RendererContext.reset();
    }


    void SceneRenderer::BeginScene(const OrthographicCamera& orthographicCamera)
    {
        Device& device = RendererAPI::GetDevice();
        Swapchain& swapchain = device.GetSwapchain();

        // Blocks until resources from MAX_IN_FLIGHT_FRAMES frames ago are out of use
        RenderTargetHandle renderTarget = swapchain.AcquireNextImage();

        m_RendererContext->IsSceneStarted = true;
        m_RendererContext->CurrentFrameIndex++;
        if (m_RendererContext->CurrentFrameIndex == 3) { m_RendererContext->CurrentFrameIndex = 0; }

        FrameContext& frameContext = m_RendererContext->FrameContexts[m_RendererContext->CurrentFrameIndex];
        frameContext.SceneRenderTarget = renderTarget;
        frameContext.Meshes.clear();
        frameContext.Materials.clear();
        frameContext.Transforms.clear();

        frameContext.TempDescriptorSets.clear();

        RendererAPI::SetBlending(true);
    }


    void SceneRenderer::EndScene()
    {
        m_RendererContext->IsSceneStarted = false;
        RenderScene();
    }


    void SceneRenderer::Submit(Mesh& mesh, Material& material, Mat4& transform)
    {
        ASSERT(m_RendererContext->IsSceneStarted, "Scene has not been started! Use SceneRenderer::BeginScene")
        FrameContext& frameContext = m_RendererContext->FrameContexts[m_RendererContext->CurrentFrameIndex];

        frameContext.Meshes.push_back(mesh);
        frameContext.Materials.push_back(material);
        frameContext.Transforms.push_back(transform);

        Device& device = RendererAPI::GetDevice();
        frameContext.TempDescriptorSets.push_back(device.CreateDescriptorSet());
    }


    uint32 SceneRenderer::GetDrawCallsPerFrame()
    {
        return RendererAPI::s_RendererCommands->GetNumberOfDrawCalls();
    }


    API SceneRenderer::GetRendererAPIBackend()
    {
        return RendererAPI::s_RendererCommands->GetAPI();
    }


    void SceneRenderer::TestUpdate()
    {

    }


    void SceneRenderer::RenderScene()
    {
        // TODO: Sort the meshes by material

        Device& device = RendererAPI::GetDevice();

        FrameContext& frameContext = m_RendererContext->FrameContexts[m_RendererContext->CurrentFrameIndex];
        RenderTargetHandle renderTarget = frameContext.SceneRenderTarget;
        CommandBufferHandle commandBuffer = frameContext.SceneCommandBuffer;
        FramebufferHandle framebufferHandle = frameContext.SceneFramebuffer;
        RenderPassHandle renderPass = m_RendererContext->RenderPass;

        // RendererAPI::Clear(commandBuffer, renderTarget);

        commandBuffer->BeginRecording();
        renderPass->BeginRenderPass(commandBuffer, framebufferHandle);

        for (uint32 i = 0; i < frameContext.Meshes.size(); i++)
        {
            Mesh& mesh = frameContext.Meshes[i];
            Material& material = frameContext.Materials[i];


            DescriptorSetHandle& descriptorSet = frameContext.TempDescriptorSets[i];
            //TODO: Save the descriptor sets to the materials
            descriptorSet->BeginBuildingSet();
            descriptorSet->AddDescriptorImageSampler(material.TextureUniform, ShaderStage::FRAGMENT);
            descriptorSet->EndBuildingSet();

            if (i == 0)
            {
                frameContext.TempPipelineState = device.CreatePipelineStateObject(renderPass,
                    material.VertexShader, material.PixelShader, descriptorSet, mesh.VertexBuffer->GetBufferLayout());
            }

            RendererAPI::PushConstants(commandBuffer, frameContext.TempPipelineState, glm::value_ptr(frameContext.Transforms[i]), sizeof(Mat4));


            frameContext.TempPipelineState->Bind(commandBuffer); // temp, idk
            frameContext.TempPipelineState->BindDescriptorSet(commandBuffer, descriptorSet); // temp, idk

            mesh.VertexBuffer->Bind(commandBuffer);
            mesh.IndexBuffer->Bind(commandBuffer);
            RendererAPI::DrawElementsIndexed(commandBuffer, mesh.IndexBuffer);
        }

        RendererAPI::CallImGuiDraws(commandBuffer);

        renderPass->EndRenderPass(commandBuffer);
        commandBuffer->EndRecording();

        CommandQueueHandle commandQueue = device.GetCommandQueue();
        commandQueue->Submit(commandBuffer, renderTarget);
        commandQueue->Present(renderTarget);
    }

} // Renderer