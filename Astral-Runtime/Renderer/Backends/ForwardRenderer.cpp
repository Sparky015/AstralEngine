/**
* @file ForwardRenderer.cpp
* @author Andrew Fagan
* @date 6/29/25
*/

#include "ForwardRenderer.h"

#include "../RHI/RendererAPI.h"

#include "Asset/AssetManager.h"
#include "Core/Engine.h"
#include "Renderer/RHI/Resources/CommandBuffer.h"
#include "Renderer/RHI/Resources/RenderTarget.h"
#include "Renderer/RHI/Resources/Shader.h"
#include "Renderer/RendererManager.h"

#include "stb_image.h"
#include <glm/gtc/type_ptr.hpp>

namespace Astral {

    void ForwardRenderer::Init()
    {
        PROFILE_SCOPE("SceneRenderer::Init")

        m_WindowResizedListener = EventListener<FramebufferResizedEvent>{[this](FramebufferResizedEvent event) { ResizeImages(event.Width, event.Height); }};
        m_WindowResizedListener.StartListening();

        m_ViewportSize = RendererAPI::GetContext().GetFramebufferSize();
        m_CurrentFrameIndex = 0;


        // Building the main render pass and the imgui render pass
        BuildRenderPasses();

        // Initializing the resources that are allocated per swapchain image
        InitializeFrameResources();


        m_PipelineStateCache.SetSceneDescriptorSet(m_FrameContexts[0].SceneDataDescriptorSet);
        m_CurrentViewportTexture.push(m_FrameContexts[1].OffscreenDescriptorSet);

        Engine::Get().GetRendererManager().GetContext().InitImGuiForAPIBackend(m_ImGuiRenderPass);
    }


    void ForwardRenderer::Shutdown()
    {
        PROFILE_SCOPE("SceneRenderer::Shutdown")
        m_WindowResizedListener.StopListening();

        Device& device = RendererAPI::GetDevice();
        device.WaitIdle();

        Engine::Get().GetRendererManager().GetContext().ShutdownImGuiForAPIBackend();
        m_FrameContexts.clear();
    }


    void ForwardRenderer::BeginScene(const SceneDescription& sceneDescription)
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

        m_IsSceneStarted = true;
        m_CurrentFrameIndex = renderTarget->GetImageIndex();

        FrameContext& frameContext = m_FrameContexts[m_CurrentFrameIndex];
        frameContext.SceneRenderTarget = renderTarget;

        SceneData sceneData = {
            .CameraViewProjection = sceneDescription.Camera.GetProjectionViewMatrix(),
            .CameraView = sceneDescription.Camera.GetViewMatrix(),
            .CameraProjection = sceneDescription.Camera.GetProjectionMatrix(),
            .CameraInverseViewMat = glm::inverse(sceneDescription.Camera.GetViewMatrix()),
            .CameraInverseProjectionMat = glm::inverse(sceneDescription.Camera.GetProjectionMatrix()),
            .ScreenSize = m_ViewportSize,
            .CameraPosition = sceneDescription.Camera.GetPosition(),
            .NumLights = (uint32)sceneDescription.Lights.size(),
            .AmbientLightConstant = sceneDescription.AmbientLightConstant
        };

        frameContext.SceneDataBuffer->CopyDataToBuffer(&sceneData, sizeof(SceneData));

        if (sizeof(Light) * sceneData.NumLights > frameContext.SceneLightsBuffer->GetAllocatedSize())
        {
            uint32 currentBufferAllocation = frameContext.SceneLightsBuffer->GetAllocatedSize();
            frameContext.SceneLightsBuffer->ReallocateMemory(currentBufferAllocation * 2);
        }
        frameContext.SceneLightsBuffer->CopyDataToBuffer(sceneDescription.Lights.data(), sizeof(Light) * sceneData.NumLights); // TODO: Check if it fits

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


    void ForwardRenderer::EndScene()
    {
        {
            PROFILE_SCOPE("SceneRenderer::EndScene")
            m_IsSceneStarted = false;
        }

        RenderScene();
    }


    void ForwardRenderer::Submit(Mesh& mesh, Material& material, Mat4& transform)
    {
        ASSERT(m_IsSceneStarted, "Scene has not been started! Use SceneRenderer::BeginScene")
        FrameContext& frameContext = m_FrameContexts[m_CurrentFrameIndex];

        frameContext.Meshes.push_back(mesh);
        frameContext.Materials.push_back(material);
        frameContext.Transforms.push_back(transform);
    }


    void ForwardRenderer::SetRendererSettings(const RendererSettings& rendererSettings)
    {
    }


    const RendererSettings& ForwardRenderer::GetRendererSettings()
    {
        return m_RendererSettings;
    }


    DescriptorSetHandle ForwardRenderer::GetViewportTexture()
    {
        DescriptorSetHandle& descriptorSet = m_CurrentViewportTexture.front();
        m_CurrentViewportTexture.pop();
        return descriptorSet;
    }


    void ForwardRenderer::BuildRenderPasses()
    {
        Device& device = RendererAPI::GetDevice();
        Swapchain& swapchain = device.GetSwapchain();
        std::vector<RenderTargetHandle>& renderTargets = swapchain.GetRenderTargets();

        m_MainRenderPass = device.CreateRenderPass();
        RenderPassHandle& mainRenderPass = m_MainRenderPass;

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
            .StoreOp = AttachmentStoreOp::DONT_CARE,
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


        m_ImGuiRenderPass = device.CreateRenderPass();
        RenderPassHandle& imguiRenderPass = m_ImGuiRenderPass;

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
    }


    void ForwardRenderer::InitializeFrameResources()
    {
        RenderingContext& renderingContext = RendererAPI::GetContext();
        Device& device = RendererAPI::GetDevice();
        Swapchain& swapchain = device.GetSwapchain();
        const std::vector<RenderTargetHandle>& renderTargets = swapchain.GetRenderTargets();

        for (int i = 0; i < swapchain.GetNumberOfImages(); i++)
        {
            m_FrameContexts.emplace_back(FrameContext());
            FrameContext& context = m_FrameContexts[i];
            context.Meshes = std::vector<Mesh>();
            context.Materials = std::vector<Material>();
            context.Transforms = std::vector<Mat4>();


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


            context.SceneCommandBuffer = device.AllocateCommandBuffer();

            context.SceneFramebuffer = device.CreateFramebuffer(m_MainRenderPass);
            UVec2 viewportDimensions = m_ViewportSize;
            context.SceneFramebuffer->BeginBuildingFramebuffer(viewportDimensions.x, viewportDimensions.y);
            context.SceneFramebuffer->AttachTexture(context.OffscreenRenderTarget);
            context.SceneFramebuffer->AttachTexture(context.OffscreenDepthBuffer);
            context.SceneFramebuffer->EndBuildingFramebuffer();

            context.SceneRenderTarget = nullptr;


            context.SceneDataBuffer = device.CreateUniformBuffer(nullptr, sizeof(SceneData));
            RendererAPI::NameObject(context.SceneDataBuffer, "Scene Data Buffer");

            context.SceneLightsBuffer = device.CreateStorageBuffer(nullptr, 1024);
            RendererAPI::NameObject(context.SceneLightsBuffer, "Scene Lights Buffer");

            context.SceneDataDescriptorSet = device.CreateDescriptorSet();
            context.SceneDataDescriptorSet->BeginBuildingSet();
            context.SceneDataDescriptorSet->AddDescriptorUniformBuffer(context.SceneDataBuffer, ShaderStage::ALL);
            context.SceneDataDescriptorSet->AddDescriptorStorageBuffer(context.SceneLightsBuffer, ShaderStage::ALL);
            context.SceneDataDescriptorSet->EndBuildingSet();
            RendererAPI::NameObject(context.SceneDataDescriptorSet, "Scene Data");


            context.WindowFramebuffer = device.CreateFramebuffer(m_ImGuiRenderPass);
            UVec2 frameBufferDimensions = renderingContext.GetFramebufferSize();
            context.WindowFramebuffer->BeginBuildingFramebuffer(frameBufferDimensions.x, frameBufferDimensions.y);
            context.WindowFramebuffer->AttachRenderTarget(renderTargets[i]);
            context.WindowFramebuffer->EndBuildingFramebuffer();
        }
    }


    void ForwardRenderer::RenderScene()
    {
        PROFILE_SCOPE("SceneRenderer::RenderScene")

        // TODO: Sort the meshes by material
        Device& device = RendererAPI::GetDevice();
        AssetRegistry& registry = Engine::Get().GetAssetManager().GetRegistry();

        FrameContext& frameContext = m_FrameContexts[m_CurrentFrameIndex];
        RenderTargetHandle renderTarget = frameContext.SceneRenderTarget;
        CommandBufferHandle commandBuffer = frameContext.SceneCommandBuffer;
        FramebufferHandle mainFramebufferHandle = frameContext.SceneFramebuffer;
        RenderPassHandle mainRenderPass = m_MainRenderPass;

        commandBuffer->BeginRecording();
        RendererAPI::BeginLabel(commandBuffer, "Main Render Pass", Vec4(1.0 , 1.0, 0, 1.0));
        mainRenderPass->BeginRenderPass(commandBuffer, mainFramebufferHandle);

        for (uint32 i = 0; i < frameContext.Meshes.size(); i++)
        {
            Mesh& mesh = frameContext.Meshes[i];
            Material& material = frameContext.Materials[i];

            DescriptorSetHandle& materialDescriptorSet = material.DescriptorSet;

            if (material.TextureConvention == TextureConvention::UNPACKED)
            {
                material.FragmentShader = registry.CreateAsset<Shader>("Shaders/brdf.frag");
            }
            else if (material.TextureConvention == TextureConvention::ORM_PACKED)
            {
                material.FragmentShader = registry.CreateAsset<Shader>("Shaders/Forward_ORM_LightingPass.frag");
            }

            PipelineStateObjectHandle pipeline = m_PipelineStateCache.GetPipeline(mainRenderPass, material, mesh, 0);
            pipeline->Bind(commandBuffer);
            pipeline->SetViewportAndScissor(commandBuffer, m_ViewportSize);

            PushConstant pushConstant = {
                .ModelMatrix = frameContext.Transforms[i],
                .HasNormalMap = material.HasNormalMap,
                .HasDirectXNormals = material.HasDirectXNormals,
            };

            RendererAPI::PushConstants(commandBuffer, pipeline, &pushConstant, sizeof(PushConstant));

            pipeline->BindDescriptorSet(commandBuffer, frameContext.SceneDataDescriptorSet, 0);
            pipeline->BindDescriptorSet(commandBuffer, materialDescriptorSet, 1);

            mesh.VertexBuffer->Bind(commandBuffer);
            mesh.IndexBuffer->Bind(commandBuffer);
            RendererAPI::DrawElementsIndexed(commandBuffer, mesh.IndexBuffer);
        }
        mainRenderPass->EndRenderPass(commandBuffer);
        RendererAPI::EndLabel(commandBuffer);

        RendererAPI::BeginLabel(commandBuffer, "ImGui Render Draws", Vec4(0.0f, 0.0f, 1.0f, 1.0f));
        m_ImGuiRenderPass->BeginRenderPass(commandBuffer, frameContext.WindowFramebuffer);
        RendererAPI::CallImGuiDraws(commandBuffer);
        m_ImGuiRenderPass->EndRenderPass(commandBuffer);
        RendererAPI::EndLabel(commandBuffer);

        commandBuffer->EndRecording();

        CommandQueueHandle commandQueue = device.GetCommandQueue();
        commandQueue->Submit(commandBuffer, renderTarget);
        commandQueue->Present(renderTarget);

        uint32 nextOldestFrameIndex = (m_CurrentFrameIndex + 2) % 3;
        if (m_CurrentViewportTexture.size() == 0)
        {
            m_CurrentViewportTexture.push(m_FrameContexts[nextOldestFrameIndex].OffscreenDescriptorSet);
        }
    }


    void ForwardRenderer::ResizeImages(uint32 width, uint32 height)
    {
        Device& device = RendererAPI::GetDevice();
        Swapchain& swapchain = device.GetSwapchain();
        device.WaitIdle();
        swapchain.RecreateSwapchain(width, height);
        std::vector<RenderTargetHandle> renderTargets = swapchain.GetRenderTargets();
        for (int i = 0; i < swapchain.GetNumberOfImages(); i++)
        {
            FrameContext& frameContext = m_FrameContexts[i];
            frameContext.WindowFramebuffer = device.CreateFramebuffer(m_ImGuiRenderPass);
            FramebufferHandle framebuffer = frameContext.WindowFramebuffer;

            framebuffer->BeginBuildingFramebuffer(width, height);
            framebuffer->AttachRenderTarget(renderTargets[i]);
            framebuffer->EndBuildingFramebuffer();
        }
    }


    void ForwardRenderer::ResizeViewport(uint32 width, uint32 height)
    {
        m_ViewportSize = UVec2(width, height);
        m_ViewportResizedPublisher.PublishEvent(ViewportResizedEvent(width, height));

        Device& device = RendererAPI::GetDevice();
        Swapchain& swapchain = device.GetSwapchain();
        std::vector<RenderTargetHandle> renderTargets = swapchain.GetRenderTargets();
        device.WaitIdle();

        uint32 nextOldestFrameIndex = m_CurrentFrameIndex;
        m_CurrentViewportTexture.push(m_FrameContexts[nextOldestFrameIndex].OffscreenDescriptorSet);


        for (int i = 0; i < swapchain.GetNumberOfImages(); i++)
        {
            FrameContext& frameContext = m_FrameContexts[i];

            TextureCreateInfo textureCreateInfo = {
                .Format = renderTargets[0]->GetImageFormat(),
                .Layout = ImageLayout::SHADER_READ_ONLY_OPTIMAL,
                .UsageFlags = ImageUsageFlags::COLOR_ATTACHMENT_BIT,
                .Dimensions = UVec2(width, height),
                .ImageData = nullptr
            };
            m_FrameContexts[m_CurrentFrameIndex].FramesTillFree = 2;
            m_FrameContexts[m_CurrentFrameIndex].ImGuiTexturesToBeFreed.push_back(frameContext.OffscreenDescriptorSet);
            m_FrameContexts[m_CurrentFrameIndex].TexturesToBeFreed.push_back(frameContext.OffscreenDepthBuffer);

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

            frameContext.SceneFramebuffer = device.CreateFramebuffer(m_MainRenderPass);
            FramebufferHandle framebuffer = frameContext.SceneFramebuffer;

            framebuffer->BeginBuildingFramebuffer(width, height);
            framebuffer->AttachTexture(frameContext.OffscreenRenderTarget);
            framebuffer->AttachTexture(frameContext.OffscreenDepthBuffer);
            framebuffer->EndBuildingFramebuffer();
        }

    }

} // Renderer