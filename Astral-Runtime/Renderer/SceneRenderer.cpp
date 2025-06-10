//
// Created by Andrew Fagan on 11/29/24.
//

#include "SceneRenderer.h"


#include "RHI/RendererAPI.h"

#include "Asset/AssetManager.h"
#include "Core/Engine.h"
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
        PROFILE_SCOPE("SceneRenderer::Init")

        m_RendererContext = CreateGraphicsOwnedPtr<SceneRendererContext>();
        m_RendererContext->WindowResizedListener = EventListener<FramebufferResizedEvent>{[](FramebufferResizedEvent event) { SceneRenderer::ResizeImages(event.Width, event.Height); }};
        m_RendererContext->WindowResizedListener.StartListening();

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
            context.SceneRenderTarget = nullptr;
            context.SceneCameraBuffer = device.CreateUniformBuffer(nullptr, sizeof(Mat4));
            context.SceneCameraDescriptorSet = device.CreateDescriptorSet();
            context.SceneCameraDescriptorSet->BeginBuildingSet();
            context.SceneCameraDescriptorSet->AddDescriptorUniformBuffer(context.SceneCameraBuffer, ShaderStage::VERTEX);
            context.SceneCameraDescriptorSet->EndBuildingSet();
            RendererAPI::NameObject(context.SceneCameraDescriptorSet, "Camera Matrix");
        }

        Engine::Get().GetRendererManager().GetContext().InitImGuiForAPIBackend(m_RendererContext->RenderPass);
    }


    void SceneRenderer::Shutdown()
    {
        PROFILE_SCOPE("SceneRenderer::Shutdown")
        m_RendererContext->WindowResizedListener.StopListening();

        Device& device = RendererAPI::GetDevice();
        device.WaitIdle();

        Engine::Get().GetRendererManager().GetContext().ShutdownImGuiForAPIBackend();
        m_RendererContext->FrameContexts.clear();
        m_RendererContext.reset();
    }


    void SceneRenderer::BeginScene(const SceneDescription& sceneDescription)
    {
        PROFILE_SCOPE("SceneRenderer::BeginScene")
        Device& device = RendererAPI::GetDevice();
        Swapchain& swapchain = device.GetSwapchain();

        // Blocks until resources from MAX_IN_FLIGHT_FRAMES - 1 frames ago are out of use
        RenderTargetHandle renderTarget = swapchain.AcquireNextImage();

        m_RendererContext->IsSceneStarted = true;
        m_RendererContext->CurrentFrameIndex++;
        if (m_RendererContext->CurrentFrameIndex == 3) { m_RendererContext->CurrentFrameIndex = 0; }

        FrameContext& frameContext = m_RendererContext->FrameContexts[m_RendererContext->CurrentFrameIndex];
        frameContext.SceneRenderTarget = renderTarget;
        frameContext.SceneCameraBuffer->CopyDataToBuffer((void*)glm::value_ptr(sceneDescription.Camera.GetProjectionViewMatrix()), sizeof(Mat4));
        frameContext.Meshes.clear();
        frameContext.Materials.clear();
        frameContext.Transforms.clear();

        RendererAPI::SetBlending(true);
    }


    void SceneRenderer::EndScene()
    {
        {
            PROFILE_SCOPE("SceneRenderer::EndScene")
            m_RendererContext->IsSceneStarted = false;
        }

        RenderScene();
    }


    void SceneRenderer::Submit(Mesh& mesh, Material& material, Mat4& transform)
    {
        ASSERT(m_RendererContext->IsSceneStarted, "Scene has not been started! Use SceneRenderer::BeginScene")
        FrameContext& frameContext = m_RendererContext->FrameContexts[m_RendererContext->CurrentFrameIndex];

        frameContext.Meshes.push_back(mesh);
        frameContext.Materials.push_back(material);
        frameContext.Transforms.push_back(transform);
    }


    uint32 SceneRenderer::GetDrawCallsPerFrame()
    {
        return RendererAPI::s_RendererCommands->GetNumberOfDrawCalls();
    }


    API SceneRenderer::GetRendererAPIBackend()
    {
        return RendererAPI::s_RendererCommands->GetAPI();
    }


    void SceneRenderer::RenderScene()
    {
        PROFILE_SCOPE("SceneRenderer::RenderScene")

        // TODO: Sort the meshes by material
        Device& device = RendererAPI::GetDevice();

        FrameContext& frameContext = m_RendererContext->FrameContexts[m_RendererContext->CurrentFrameIndex];
        RenderTargetHandle renderTarget = frameContext.SceneRenderTarget;
        CommandBufferHandle commandBuffer = frameContext.SceneCommandBuffer;
        FramebufferHandle framebufferHandle = frameContext.SceneFramebuffer;
        RenderPassHandle renderPass = m_RendererContext->RenderPass;

        commandBuffer->BeginRecording();
        RendererAPI::BeginLabel(commandBuffer, "Main Render Pass", Vec4(1.0 , 1.0, 0, 1.0));
        renderPass->BeginRenderPass(commandBuffer, framebufferHandle);

        for (uint32 i = 0; i < frameContext.Meshes.size(); i++)
        {
            Mesh& mesh = frameContext.Meshes[i];
            Material& material = frameContext.Materials[i];
            DescriptorSetHandle& descriptorSet = material.DescriptorSet;

            AssetRegistry& registry = Engine::Get().GetAssetManager().GetRegistry();
            Ref<Shader> vertexShader = registry.GetAsset<Shader>(material.VertexShaderID);
            Ref<Shader> fragmentShader = registry.GetAsset<Shader>(material.PixelShaderID);

            if (frameContext.TempPipelineState == nullptr)
            {
                std::vector<DescriptorSetHandle> descriptorSets{frameContext.SceneCameraDescriptorSet, descriptorSet};
                frameContext.TempPipelineState = device.CreatePipelineStateObject(renderPass,
                    vertexShader, fragmentShader, descriptorSets, mesh.VertexBuffer->GetBufferLayout());
            }

            RendererAPI::PushConstants(commandBuffer, frameContext.TempPipelineState, glm::value_ptr(frameContext.Transforms[i]), sizeof(Mat4));

            frameContext.TempPipelineState->Bind(commandBuffer);
            frameContext.TempPipelineState->BindDescriptorSet(commandBuffer, frameContext.SceneCameraDescriptorSet, 0);
            frameContext.TempPipelineState->BindDescriptorSet(commandBuffer, descriptorSet, 1);

            mesh.VertexBuffer->Bind(commandBuffer);
            mesh.IndexBuffer->Bind(commandBuffer);
            RendererAPI::DrawElementsIndexed(commandBuffer, mesh.IndexBuffer);
        }
        RendererAPI::EndLabel(commandBuffer);

        RendererAPI::BeginLabel(commandBuffer, "ImGui Render Draws", Vec4(0.0f, 0.0f, 1.0f, 1.0f));
        RendererAPI::CallImGuiDraws(commandBuffer);
        RendererAPI::EndLabel(commandBuffer);

        renderPass->EndRenderPass(commandBuffer);
        commandBuffer->EndRecording();

        CommandQueueHandle commandQueue = device.GetCommandQueue();
        commandQueue->Submit(commandBuffer, renderTarget);
        commandQueue->Present(renderTarget);
    }


    void SceneRenderer::ResizeImages(uint32 width, uint32 height)
    {
        Device& device = RendererAPI::GetDevice();
        Swapchain& swapchain = device.GetSwapchain();
        device.WaitIdle();
        swapchain.RecreateSwapchain(width, height);
        std::vector<RenderTargetHandle> renderTargets = swapchain.GetRenderTargets();
        for (int i = 0; i < swapchain.GetNumberOfImages(); i++)
        {
            m_RendererContext->FrameContexts[i].SceneFramebuffer = device.CreateFramebuffer(m_RendererContext->RenderPass, renderTargets[i]);
        }
        device.WaitIdle();
    }

} // Renderer