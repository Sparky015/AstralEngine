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

        m_RendererContext->ViewportSize = RendererAPI::GetContext().GetFramebufferSize();
        m_RendererContext->CurrentFrameIndex = 0;

        RenderingContext& renderingContext = RendererAPI::GetContext();
        Device& device = RendererAPI::GetDevice();
        Swapchain& swapchain = device.GetSwapchain();

        std::vector<RenderTargetHandle> renderTargets = swapchain.GetRenderTargets();

        m_RendererContext->MainRenderPass = device.CreateRenderPass();
        RenderPassHandle& mainRenderPass = m_RendererContext->MainRenderPass;

        AttachmentDescription offscreenTextureDescription = {
            .Format = renderTargets[0]->GetImageFormat(),
            .LoadOp = AttachmentLoadOp::CLEAR,
            .StoreOp = AttachmentStoreOp::STORE,
            .InitialLayout = ImageLayout::SHADER_READ_ONLY_OPTIMAL,
            .FinalLayout = ImageLayout::SHADER_READ_ONLY_OPTIMAL,
            .ClearColor = Vec4(0.0, 0.0, 1.0, 1.0)
        };

        AttachmentDescription offscreenDepthBufferDescription = {
            .Format = ImageFormat::D32_SFLOAT_S8_UINT,
            .LoadOp = AttachmentLoadOp::CLEAR,
            .StoreOp = AttachmentStoreOp::STORE,
            .InitialLayout = ImageLayout::DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
            .FinalLayout = ImageLayout::DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
            .ClearColor = Vec4(1.0, 0.0, 0.0, 0.0)
        };

        mainRenderPass->BeginBuildingRenderPass();
        AttachmentIndex offscreenTextureIndex = mainRenderPass->DefineAttachment(offscreenTextureDescription);
        AttachmentIndex offscreenDepthBufferIndex = mainRenderPass->DefineAttachment(offscreenDepthBufferDescription);
        mainRenderPass->BeginBuildingSubpass();
        mainRenderPass->AddColorAttachment(offscreenTextureIndex, ImageLayout::COLOR_ATTACHMENT_OPTIMAL);
        mainRenderPass->AddDepthStencilAttachment(offscreenDepthBufferIndex, ImageLayout::DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
        mainRenderPass->EndBuildingSubpass();
        mainRenderPass->EndBuildingRenderPass();


        m_RendererContext->ImGuiRenderPass = device.CreateRenderPass();
        RenderPassHandle& imguiRenderPass = m_RendererContext->ImGuiRenderPass;

        AttachmentDescription renderTargetDescription = {
            .Format = renderTargets[0]->GetImageFormat(),
            .LoadOp = AttachmentLoadOp::CLEAR,
            .StoreOp = AttachmentStoreOp::STORE,
            .InitialLayout = ImageLayout::UNDEFINED,
            .FinalLayout = ImageLayout::PRESENT_SRC_KHR,
            .ClearColor = Vec4(0.0, 0.0, 1.0, 1.0)
        };

        imguiRenderPass->BeginBuildingRenderPass();
        AttachmentIndex renderTargetIndex = imguiRenderPass->DefineAttachment(renderTargetDescription);
        imguiRenderPass->BeginBuildingSubpass();
        imguiRenderPass->AddColorAttachment(renderTargetIndex, ImageLayout::COLOR_ATTACHMENT_OPTIMAL);
        imguiRenderPass->EndBuildingSubpass();
        imguiRenderPass->EndBuildingRenderPass();

        for (int i = 0; i < swapchain.GetNumberOfImages(); i++)
        {
            m_RendererContext->FrameContexts.emplace_back(FrameContext());
            FrameContext& context = m_RendererContext->FrameContexts[i];

            context.Meshes = std::vector<Mesh>();
            context.Materials = std::vector<Material>();
            context.Transforms = std::vector<Mat4>();
            context.SceneCommandBuffer = device.AllocateCommandBuffer();


            TextureCreateInfo textureCreateInfo = {
                .Format = renderTargets[0]->GetImageFormat(),
                .Layout = ImageLayout::SHADER_READ_ONLY_OPTIMAL,
                .UsageFlags = ImageUsageFlags::COLOR_ATTACHMENT_BIT,
                .Dimensions = renderTargets[0]->GetDimensions(),
                .ImageData = nullptr
            };
            context.OffscreenRenderTarget = device.CreateTexture(textureCreateInfo);
            TextureCreateInfo depthBufferTextureCreateInfo = {
                .Format = ImageFormat::D32_SFLOAT_S8_UINT,
                .Layout = ImageLayout::DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
                .UsageFlags = ImageUsageFlags::DEPTH_STENCIL_ATTACHMENT_BIT,
                .Dimensions = renderTargets[0]->GetDimensions(),
                .ImageData = nullptr
            };
            context.OffscreenDepthBuffer = device.CreateTexture(depthBufferTextureCreateInfo);

            context.OffscreenDescriptorSet = device.CreateDescriptorSet();
            context.OffscreenDescriptorSet->BeginBuildingSet();
            context.OffscreenDescriptorSet->AddDescriptorImageSampler(context.OffscreenRenderTarget, ShaderStage::FRAGMENT);
            context.OffscreenDescriptorSet->EndBuildingSet();
            context.FramesTillFree = 2;


            context.WindowFramebuffer = device.CreateFramebuffer(m_RendererContext->ImGuiRenderPass);
            UVec2 frameBufferDimensions = renderingContext.GetFramebufferSize();
            context.WindowFramebuffer->BeginBuildingFramebuffer(frameBufferDimensions.x, frameBufferDimensions.y);
            context.WindowFramebuffer->AttachRenderTarget(renderTargets[i]);
            context.WindowFramebuffer->EndBuildingFramebuffer();


            context.SceneFramebuffer = device.CreateFramebuffer(m_RendererContext->MainRenderPass);
            UVec2 viewportDimensions = m_RendererContext->ViewportSize;
            context.SceneFramebuffer->BeginBuildingFramebuffer(viewportDimensions.x, viewportDimensions.y);
            context.SceneFramebuffer->AttachTexture(context.OffscreenRenderTarget);
            context.SceneFramebuffer->AttachTexture(context.OffscreenDepthBuffer);
            context.SceneFramebuffer->EndBuildingFramebuffer();


            context.TempPipelineState = nullptr;
            context.SceneRenderTarget = nullptr;


            context.SceneCameraBuffer = device.CreateUniformBuffer(nullptr, sizeof(Mat4));
            context.SceneCameraDescriptorSet = device.CreateDescriptorSet();
            context.SceneCameraDescriptorSet->BeginBuildingSet();
            context.SceneCameraDescriptorSet->AddDescriptorUniformBuffer(context.SceneCameraBuffer, ShaderStage::VERTEX);
            context.SceneCameraDescriptorSet->EndBuildingSet();
            RendererAPI::NameObject(context.SceneCameraDescriptorSet, "Camera Matrix");


        }

        m_RendererContext->CurrentViewportTexture.push(m_RendererContext->FrameContexts[1].OffscreenDescriptorSet);

        Engine::Get().GetRendererManager().GetContext().InitImGuiForAPIBackend(m_RendererContext->ImGuiRenderPass);
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
        RenderTargetHandle renderTarget;
        {
            PROFILE_SCOPE("SceneRenderer::BeginScene::AcquireNextImage")
            renderTarget = swapchain.AcquireNextImage();
        }

        m_RendererContext->IsSceneStarted = true;
        m_RendererContext->CurrentFrameIndex = renderTarget->GetImageIndex();

        FrameContext& frameContext = m_RendererContext->FrameContexts[m_RendererContext->CurrentFrameIndex];
        frameContext.SceneRenderTarget = renderTarget;
        frameContext.SceneCameraBuffer->CopyDataToBuffer((void*)glm::value_ptr(sceneDescription.Camera.GetProjectionViewMatrix()), sizeof(Mat4));
        frameContext.Meshes.clear();
        frameContext.Materials.clear();
        frameContext.Transforms.clear();

        frameContext.FramesTillFree--;
        if (frameContext.FramesTillFree == 0)
        {
            frameContext.ImGuiTexturesToBeFreed.clear();
            frameContext.TexturesToBeFreed.clear();
        }
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


    DescriptorSetHandle SceneRenderer::GetViewportTexture()
    {
        DescriptorSetHandle& descriptorSet = m_RendererContext->CurrentViewportTexture.front();
        m_RendererContext->CurrentViewportTexture.pop();
        return descriptorSet;
    }


    RendererDebugStats SceneRenderer::GetRendererDebugStats()
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
        FramebufferHandle mainFramebufferHandle = frameContext.SceneFramebuffer;
        RenderPassHandle mainRenderPass = m_RendererContext->MainRenderPass;

        commandBuffer->BeginRecording();
        RendererAPI::BeginLabel(commandBuffer, "Main Render Pass", Vec4(1.0 , 1.0, 0, 1.0));
        mainRenderPass->BeginRenderPass(commandBuffer, mainFramebufferHandle);

        if (frameContext.TempPipelineState)
        {
            frameContext.TempPipelineState->Bind(commandBuffer);
            frameContext.TempPipelineState->SetViewportAndScissor(commandBuffer, m_RendererContext->ViewportSize);
        }

        for (uint32 i = 0; i < frameContext.Meshes.size(); i++)
        {
            Mesh& mesh = frameContext.Meshes[i];
            Material& material = frameContext.Materials[i];

            DescriptorSetHandle& materialDescriptorSet = material.DescriptorSet;
            Ref<Shader> vertexShader = material.VertexShader;
            Ref<Shader> fragmentShader = material.FragmentShader;

            if (frameContext.TempPipelineState == nullptr)
            {
                std::vector<DescriptorSetHandle> descriptorSets{frameContext.SceneCameraDescriptorSet, materialDescriptorSet};
                frameContext.TempPipelineState = device.CreatePipelineStateObject(mainRenderPass,
                    vertexShader, fragmentShader, descriptorSets, mesh.VertexBuffer->GetBufferLayout());
                frameContext.TempPipelineState->Bind(commandBuffer);
                frameContext.TempPipelineState->SetViewportAndScissor(commandBuffer, m_RendererContext->ViewportSize);

            }

            RendererAPI::PushConstants(commandBuffer, frameContext.TempPipelineState, glm::value_ptr(frameContext.Transforms[i]), sizeof(Mat4));

            frameContext.TempPipelineState->BindDescriptorSet(commandBuffer, frameContext.SceneCameraDescriptorSet, 0);
            frameContext.TempPipelineState->BindDescriptorSet(commandBuffer, materialDescriptorSet, 1);

            mesh.VertexBuffer->Bind(commandBuffer);
            mesh.IndexBuffer->Bind(commandBuffer);
            RendererAPI::DrawElementsIndexed(commandBuffer, mesh.IndexBuffer);
        }
        mainRenderPass->EndRenderPass(commandBuffer);
        RendererAPI::EndLabel(commandBuffer);

        RendererAPI::BeginLabel(commandBuffer, "ImGui Render Draws", Vec4(0.0f, 0.0f, 1.0f, 1.0f));
        m_RendererContext->ImGuiRenderPass->BeginRenderPass(commandBuffer, frameContext.WindowFramebuffer);
        RendererAPI::CallImGuiDraws(commandBuffer);
        m_RendererContext->ImGuiRenderPass->EndRenderPass(commandBuffer);
        RendererAPI::EndLabel(commandBuffer);

        commandBuffer->EndRecording();

        CommandQueueHandle commandQueue = device.GetCommandQueue();
        commandQueue->Submit(commandBuffer, renderTarget);
        commandQueue->Present(renderTarget);

        uint32 nextOldestFrameIndex = (m_RendererContext->CurrentFrameIndex + 2) % 3;
        if (m_RendererContext->CurrentViewportTexture.size() == 0)
        {
            m_RendererContext->CurrentViewportTexture.push(m_RendererContext->FrameContexts[nextOldestFrameIndex].OffscreenDescriptorSet);
        }
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
            FrameContext& frameContext = m_RendererContext->FrameContexts[i];
            frameContext.WindowFramebuffer = device.CreateFramebuffer(m_RendererContext->ImGuiRenderPass);
            FramebufferHandle framebuffer = frameContext.WindowFramebuffer;

            framebuffer->BeginBuildingFramebuffer(width, height);
            framebuffer->AttachRenderTarget(renderTargets[i]);
            framebuffer->EndBuildingFramebuffer();
        }
    }


    void SceneRenderer::ResizeViewport(uint32 width, uint32 height)
    {
        m_RendererContext->ViewportSize = UVec2(width, height);
        m_RendererContext->ViewportResizedPublisher.PublishEvent(ViewportResizedEvent(width, height));

        Device& device = RendererAPI::GetDevice();
        Swapchain& swapchain = device.GetSwapchain();
        std::vector<RenderTargetHandle> renderTargets = swapchain.GetRenderTargets();
        device.WaitIdle();

        uint32 nextOldestFrameIndex = m_RendererContext->CurrentFrameIndex;
        m_RendererContext->CurrentViewportTexture.push(m_RendererContext->FrameContexts[nextOldestFrameIndex].OffscreenDescriptorSet);


        for (int i = 0; i < swapchain.GetNumberOfImages(); i++)
        {
            FrameContext& frameContext = m_RendererContext->FrameContexts[i];

            TextureCreateInfo textureCreateInfo = {
                .Format = renderTargets[0]->GetImageFormat(),
                .Layout = ImageLayout::SHADER_READ_ONLY_OPTIMAL,
                .UsageFlags = ImageUsageFlags::COLOR_ATTACHMENT_BIT,
                .Dimensions = UVec2(width, height),
                .ImageData = nullptr
            };
            m_RendererContext->FrameContexts[m_RendererContext->CurrentFrameIndex].FramesTillFree = 2;
            m_RendererContext->FrameContexts[m_RendererContext->CurrentFrameIndex].ImGuiTexturesToBeFreed.push_back(frameContext.OffscreenDescriptorSet);
            m_RendererContext->FrameContexts[m_RendererContext->CurrentFrameIndex].TexturesToBeFreed.push_back(frameContext.OffscreenDepthBuffer);

            frameContext.OffscreenRenderTarget = device.CreateTexture(textureCreateInfo);

            TextureCreateInfo depthBufferTextureCreateInfo = {
                .Format = ImageFormat::D32_SFLOAT_S8_UINT,
                .Layout = ImageLayout::DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
                .UsageFlags = ImageUsageFlags::DEPTH_STENCIL_ATTACHMENT_BIT,
                .Dimensions = UVec2(width, height),
                .ImageData = nullptr
            };
            frameContext.OffscreenDepthBuffer = device.CreateTexture(depthBufferTextureCreateInfo);

            frameContext.OffscreenDescriptorSet = device.CreateDescriptorSet();
            frameContext.OffscreenDescriptorSet->BeginBuildingSet();
            frameContext.OffscreenDescriptorSet->AddDescriptorImageSampler(frameContext.OffscreenRenderTarget, ShaderStage::FRAGMENT);
            frameContext.OffscreenDescriptorSet->EndBuildingSet();

            frameContext.SceneFramebuffer = device.CreateFramebuffer(m_RendererContext->MainRenderPass);
            FramebufferHandle framebuffer = frameContext.SceneFramebuffer;

            framebuffer->BeginBuildingFramebuffer(width, height);
            framebuffer->AttachTexture(frameContext.OffscreenRenderTarget);
            framebuffer->AttachTexture(frameContext.OffscreenDepthBuffer);
            framebuffer->EndBuildingFramebuffer();
        }

    }

} // Renderer