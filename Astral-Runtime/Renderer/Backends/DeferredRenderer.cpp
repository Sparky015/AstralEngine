/**
* @file DeferredRenderer.cpp
* @author Andrew Fagan
* @date 7/1/2025
*/

#include "DeferredRenderer.h"

#include "../RHI/RendererAPI.h"

#include "Asset/AssetManager.h"
#include "Core/Engine.h"
#include "Renderer/RHI/Resources/CommandBuffer.h"
#include "Renderer/RHI/Resources/RenderTarget.h"
#include "Renderer/RHI/Resources/Shader.h"
#include "Renderer/RendererManager.h"

#include <glm/gtc/type_ptr.hpp>

#include "Debug/ImGui/ImGuiManager.h"

namespace Astral {

    void DeferredRenderer::Init()
    {
        PROFILE_SCOPE("SceneRenderer::Init")

        m_WindowResizedListener = EventListener<FramebufferResizedEvent>{[this](FramebufferResizedEvent event) { ResizeImages(event.Width, event.Height); }};
        m_WindowResizedListener.StartListening();

        m_ViewportSize = RendererAPI::GetContext().GetFramebufferSize();
        m_CurrentFrameIndex = 0;



        // Building the imgui render pass
        BuildImGuiEditorRenderPass();

        // Initializing the resources that are allocated per swapchain image
        InitializeFrameResources();


        BuildRenderGraph();


        m_PipelineStateCache.SetSceneDescriptorSet(m_FrameContexts[0].SceneDataDescriptorSet);
        m_CurrentViewportTexture.push(m_FrameContexts[1].OffscreenDescriptorSet);

        Engine::Get().GetRendererManager().GetContext().InitImGuiForAPIBackend(m_ImGuiRenderPass);
        AssetRegistry& registry = Engine::Get().GetAssetManager().GetRegistry();
        m_GeometryPassShader = registry.CreateAsset<Shader>("Shaders/Deferred_Set_GBuffer.frag");
        m_LightingShader = registry.CreateAsset<Shader>("Shaders/Deferred_Lighting_Pass.frag");
    }


    void DeferredRenderer::Shutdown()
    {
        PROFILE_SCOPE("SceneRenderer::Shutdown")
        m_WindowResizedListener.StopListening();

        Device& device = RendererAPI::GetDevice();
        device.WaitIdle();

        Engine::Get().GetRendererManager().GetContext().ShutdownImGuiForAPIBackend();
        m_FrameContexts.clear();
    }


    void DeferredRenderer::BeginScene(const SceneDescription& sceneDescription)
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
            .CameraInverseViewMat = glm::inverse(sceneDescription.Camera.GetViewMatrix()),
            .CameraInverseProjectionMat = glm::inverse(sceneDescription.Camera.GetProjectionMatrix()),
            .ScreenSize = m_ViewportSize,
            .CameraPosition = sceneDescription.Camera.GetPosition(),
            .NumLights = (uint32)sceneDescription.Lights.size(),
        };

        frameContext.SceneDataBuffer->CopyDataToBuffer(&sceneData, sizeof(SceneData));

        if (sizeof(Light) * sceneData.NumLights > frameContext.SceneLightsBuffer->GetAllocatedSize())
        {
            uint32 currentBufferAllocation = frameContext.SceneLightsBuffer->GetAllocatedSize();
            // TODO: If reallocation happens, you need to re-add to descriptor set to replace the old buffer as reallocation creates a new buffer
            frameContext.SceneLightsBuffer->ReallocateMemory(currentBufferAllocation * 2);
        }
        frameContext.SceneLightsBuffer->CopyDataToBuffer(sceneDescription.Lights.data(), sizeof(Light) * sceneData.NumLights);

        frameContext.Meshes.clear();
        frameContext.Materials.clear();
        frameContext.Transforms.clear();
    }


    void DeferredRenderer::EndScene()
    {
        {
            PROFILE_SCOPE("SceneRenderer::EndScene")
            m_IsSceneStarted = false;
        }

        RenderScene();
    }


    void DeferredRenderer::Submit(Mesh& mesh, Material& material, Mat4& transform)
    {
        ASSERT(m_IsSceneStarted, "Scene has not been started! Use SceneRenderer::BeginScene")
        FrameContext& frameContext = m_FrameContexts[m_CurrentFrameIndex];

        frameContext.Meshes.push_back(mesh);
        frameContext.Materials.push_back(material);
        frameContext.Transforms.push_back(transform);
    }


    DescriptorSetHandle DeferredRenderer::GetViewportTexture()
    {
        DescriptorSetHandle& descriptorSet = m_CurrentViewportTexture.front();
        m_CurrentViewportTexture.pop();
        return descriptorSet;
    }


    void DeferredRenderer::BuildRenderGraph()
    {

        AttachmentDescription albedoBufferDescription = {
            .Format = ImageFormat::R8G8B8A8_UNORM,
            .ImageUsageFlags = ImageUsageFlags::COLOR_ATTACHMENT_BIT,
            .LoadOp = AttachmentLoadOp::CLEAR,
            .StoreOp = AttachmentStoreOp::STORE,
            .InitialLayout = ImageLayout::COLOR_ATTACHMENT_OPTIMAL,
            .FinalLayout = ImageLayout::COLOR_ATTACHMENT_OPTIMAL,
            .ClearColor = Vec4(0.0, 0.0, 0.0, 0.0)
        };

        AttachmentDescription metallicBufferDescription = {
            .Format = ImageFormat::R8_UNORM,
            .ImageUsageFlags = ImageUsageFlags::COLOR_ATTACHMENT_BIT,
            .LoadOp = AttachmentLoadOp::CLEAR,
            .StoreOp = AttachmentStoreOp::STORE,
            .InitialLayout = ImageLayout::COLOR_ATTACHMENT_OPTIMAL,
            .FinalLayout = ImageLayout::COLOR_ATTACHMENT_OPTIMAL,
            .ClearColor = Vec4(0.0, 0.0, 0.0, 0.0)
        };

        AttachmentDescription roughnessBufferDescription = {
            .Format = ImageFormat::R8_UNORM,
            .ImageUsageFlags = ImageUsageFlags::COLOR_ATTACHMENT_BIT,
            .LoadOp = AttachmentLoadOp::CLEAR,
            .StoreOp = AttachmentStoreOp::STORE,
            .InitialLayout = ImageLayout::COLOR_ATTACHMENT_OPTIMAL,
            .FinalLayout = ImageLayout::COLOR_ATTACHMENT_OPTIMAL,
            .ClearColor = Vec4(0.0, 0.0, 0.0, 0.0)
        };

        AttachmentDescription emissionBufferDescription = {
            .Format = ImageFormat::R8G8B8A8_UNORM,
            .ImageUsageFlags = ImageUsageFlags::COLOR_ATTACHMENT_BIT,
            .LoadOp = AttachmentLoadOp::CLEAR,
            .StoreOp = AttachmentStoreOp::STORE,
            .InitialLayout = ImageLayout::COLOR_ATTACHMENT_OPTIMAL,
            .FinalLayout = ImageLayout::COLOR_ATTACHMENT_OPTIMAL,
            .ClearColor = Vec4(0.0, 0.0, 0.0, 0.0)
        };

        AttachmentDescription normalBufferDescription = {
            .Format = ImageFormat::R8G8B8A8_UNORM,
            .ImageUsageFlags = ImageUsageFlags::COLOR_ATTACHMENT_BIT,
            .LoadOp = AttachmentLoadOp::CLEAR,
            .StoreOp = AttachmentStoreOp::STORE,
            .InitialLayout = ImageLayout::COLOR_ATTACHMENT_OPTIMAL,
            .FinalLayout = ImageLayout::COLOR_ATTACHMENT_OPTIMAL,
            .ClearColor = Vec4(0.0, 0.0, 0.0, 0.0)
        };

        AttachmentDescription depthBufferDescription = {
            .Format = ImageFormat::D32_SFLOAT_S8_UINT,
            .ImageUsageFlags = ImageUsageFlags::DEPTH_STENCIL_ATTACHMENT_BIT,
            .LoadOp = AttachmentLoadOp::CLEAR,
            .StoreOp = AttachmentStoreOp::STORE,
            .InitialLayout = ImageLayout::DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
            .FinalLayout = ImageLayout::DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
            .ClearColor = Vec4(1.0, 0.0, 0.0, 0.0)
        };

        RenderGraphPass geometryPass = RenderGraphPass(OutputAttachmentDimensions, "GBuffer Pass", [&](){ GeometryPass(); });
        geometryPass.AddColorAttachment(albedoBufferDescription, "GBuffer_Albedo", ImageLayout::COLOR_ATTACHMENT_OPTIMAL);
        geometryPass.AddColorAttachment(metallicBufferDescription, "GBuffer_Metallic", ImageLayout::COLOR_ATTACHMENT_OPTIMAL);
        geometryPass.AddColorAttachment(roughnessBufferDescription, "GBuffer_Roughness", ImageLayout::COLOR_ATTACHMENT_OPTIMAL);
        geometryPass.AddColorAttachment(emissionBufferDescription, "GBuffer_Emission", ImageLayout::COLOR_ATTACHMENT_OPTIMAL);
        geometryPass.AddColorAttachment(normalBufferDescription, "GBuffer_Normals", ImageLayout::COLOR_ATTACHMENT_OPTIMAL);
        geometryPass.AddDepthStencilAttachment(depthBufferDescription, "GBuffer_Depth_Buffer", ImageLayout::DEPTH_STENCIL_ATTACHMENT_OPTIMAL);



        AttachmentDescription lightingTextureDescription = {
            .Format = ImageFormat::B8G8R8A8_UNORM,
            .ImageUsageFlags = ImageUsageFlags::COLOR_ATTACHMENT_BIT,
            .LoadOp = AttachmentLoadOp::CLEAR,
            .StoreOp = AttachmentStoreOp::STORE,
            .InitialLayout = ImageLayout::SHADER_READ_ONLY_OPTIMAL,
            .FinalLayout = ImageLayout::SHADER_READ_ONLY_OPTIMAL,
            .ClearColor = Vec4(0.0, 0.0, 1.0, 1.0)
        };


        RenderGraphPass lightingPass = RenderGraphPass(OutputAttachmentDimensions, "Lighting Pass", [&](){ LightingPass(); });
        lightingPass.AddInputAttachment(&geometryPass, "GBuffer_Albedo", ImageLayout::SHADER_READ_ONLY_OPTIMAL);
        lightingPass.AddInputAttachment(&geometryPass, "GBuffer_Metallic", ImageLayout::SHADER_READ_ONLY_OPTIMAL);
        lightingPass.AddInputAttachment(&geometryPass, "GBuffer_Roughness", ImageLayout::SHADER_READ_ONLY_OPTIMAL);
        lightingPass.AddInputAttachment(&geometryPass, "GBuffer_Emission", ImageLayout::SHADER_READ_ONLY_OPTIMAL);
        lightingPass.AddInputAttachment(&geometryPass, "GBuffer_Normals", ImageLayout::SHADER_READ_ONLY_OPTIMAL);
        lightingPass.AddInputAttachment(&geometryPass, "GBuffer_Depth_Buffer", ImageLayout::SHADER_READ_ONLY_OPTIMAL);

        lightingPass.AddColorAttachment(lightingTextureDescription, "Deferred_Lighting_Buffer", ImageLayout::COLOR_ATTACHMENT_OPTIMAL);

        std::vector<TextureHandle> outputTextures;
        constexpr int numFramesInFlight = 3;
        outputTextures.reserve(numFramesInFlight);
        for (int i = 0; i < numFramesInFlight; i++)
        {
            TextureHandle offscreenOutput = m_FrameContexts[i].OffscreenRenderTarget;
            outputTextures.push_back(offscreenOutput);
        }

        m_RenderGraph.BeginBuildingRenderGraph("Viewport");
        m_RenderGraph.AddPass(geometryPass);
        m_RenderGraph.AddPass(lightingPass);
        // m_RenderGraph.SetOutputAttachment(geometryPass, "GBuffer_Albedo", outputTextures);
        m_RenderGraph.SetOutputAttachment(lightingPass, "Deferred_Lighting_Buffer", outputTextures);
        m_RenderGraph.EndBuildingRenderGraph();
    }


    void DeferredRenderer::BuildImGuiEditorRenderPass()
    {
        Device& device = RendererAPI::GetDevice();
        Swapchain& swapchain = device.GetSwapchain();
        std::vector<RenderTargetHandle>& renderTargets = swapchain.GetRenderTargets();


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



    void DeferredRenderer::InitializeFrameResources()
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
            RendererAPI::NameObject(context.OffscreenRenderTarget, "Offscreen Render Target");




            context.OffscreenDescriptorSet = device.CreateDescriptorSet();
            context.OffscreenDescriptorSet->BeginBuildingSet();
            context.OffscreenDescriptorSet->AddDescriptorImageSampler(context.OffscreenRenderTarget, ShaderStage::FRAGMENT);
            context.OffscreenDescriptorSet->EndBuildingSet();


            context.SceneCommandBuffer = device.AllocateCommandBuffer();
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


    void DeferredRenderer::RenderScene()
    {
        PROFILE_SCOPE("SceneRenderer::RenderScene")

        // TODO: Sort the meshes by material
        Device& device = RendererAPI::GetDevice();

        FrameContext& frameContext = m_FrameContexts[m_CurrentFrameIndex];
        RenderTargetHandle renderTarget = frameContext.SceneRenderTarget;
        CommandBufferHandle commandBuffer = frameContext.SceneCommandBuffer;

        commandBuffer->BeginRecording();

        // Viewport Rendering
        m_RenderGraph.Execute(commandBuffer, m_CurrentFrameIndex);

        {
            PipelineBarrier pipelineBarrier = {};
            pipelineBarrier.SourceStageMask = PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
            pipelineBarrier.DestinationStageMask = PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
            pipelineBarrier.DependencyFlags = DependencyFlags::BY_REGION_BIT;


            ImageMemoryBarrier imageMemoryBarrier = {};
            imageMemoryBarrier.SourceAccessMask = ACCESS_FLAGS_COLOR_ATTACHMENT_WRITE_BIT;
            imageMemoryBarrier.DestinationAccessMask = ACCESS_FLAGS_SHADER_READ_BIT;
            imageMemoryBarrier.OldLayout = ImageLayout::SHADER_READ_ONLY_OPTIMAL;
            imageMemoryBarrier.NewLayout = ImageLayout::SHADER_READ_ONLY_OPTIMAL;
            imageMemoryBarrier.SourceQueueFamilyIndex = QueueFamilyIgnored;
            imageMemoryBarrier.DestinationQueueFamilyIndex = QueueFamilyIgnored;
            imageMemoryBarrier.Image = m_FrameContexts[m_CurrentFrameIndex].OffscreenRenderTarget;
            imageMemoryBarrier.ImageSubresourceRange = {
                .AspectMask = m_FrameContexts[m_CurrentFrameIndex].OffscreenRenderTarget->GetImageAspect(),
                .BaseMipLevel = 0,
                .LevelCount = 1,
                .BaseArrayLayer = 0,
                .LayerCount = 1
            };
            pipelineBarrier.ImageMemoryBarriers.push_back(imageMemoryBarrier);

            RendererAPI::SetPipelineBarrier(commandBuffer, pipelineBarrier);
        }


        // ImGui Rendering
        RendererAPI::BeginLabel(commandBuffer, "ImGui Render Draws", Vec4(0.0f, 0.0f, 1.0f, 1.0f));
        m_ImGuiRenderPass->BeginRenderPass(commandBuffer, frameContext.WindowFramebuffer);
        RendererAPI::CallImGuiDraws(commandBuffer);
        m_ImGuiRenderPass->EndRenderPass(commandBuffer);
        RendererAPI::EndLabel(commandBuffer);


        {
            PipelineBarrier pipelineBarrier = {};
            pipelineBarrier.SourceStageMask = PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
            pipelineBarrier.DestinationStageMask = PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
            pipelineBarrier.DependencyFlags = DependencyFlags::BY_REGION_BIT;

            ImageMemoryBarrier imageMemoryBarrier = {};
            imageMemoryBarrier.SourceAccessMask = ACCESS_FLAGS_SHADER_READ_BIT;
            imageMemoryBarrier.DestinationAccessMask = ACCESS_FLAGS_COLOR_ATTACHMENT_WRITE_BIT;
            imageMemoryBarrier.OldLayout = ImageLayout::SHADER_READ_ONLY_OPTIMAL; // TODO: Make render pass objects update the textures' layout when the render pass automatically transitions layouts
            imageMemoryBarrier.NewLayout = ImageLayout::SHADER_READ_ONLY_OPTIMAL;
            imageMemoryBarrier.SourceQueueFamilyIndex = QueueFamilyIgnored;
            imageMemoryBarrier.DestinationQueueFamilyIndex = QueueFamilyIgnored;
            imageMemoryBarrier.Image = m_FrameContexts[m_CurrentFrameIndex].OffscreenRenderTarget;
            imageMemoryBarrier.ImageSubresourceRange = {
                .AspectMask = m_FrameContexts[m_CurrentFrameIndex].OffscreenRenderTarget->GetImageAspect(),
                .BaseMipLevel = 0,
                .LevelCount = 1,
                .BaseArrayLayer = 0,
                .LayerCount = 1
            };
            pipelineBarrier.ImageMemoryBarriers.push_back(imageMemoryBarrier);

            RendererAPI::SetPipelineBarrier(commandBuffer, pipelineBarrier);
        }


        commandBuffer->EndRecording();

        CommandQueueHandle commandQueue = device.GetCommandQueue();
        commandQueue->Submit(commandBuffer, renderTarget);
        commandQueue->Present(renderTarget);

        uint32 nextFrameIndex = (m_CurrentFrameIndex + 1) % 3;
        if (m_CurrentViewportTexture.size() == 0)
        {
            m_CurrentViewportTexture.push(m_FrameContexts[nextFrameIndex].OffscreenDescriptorSet);
        }
    }


    void DeferredRenderer::ResizeImages(uint32 width, uint32 height)
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


    void DeferredRenderer::GeometryPass()
    {
        const RenderGraphPassExecutionContext& executionContext = m_RenderGraph.GetExecutionContext();
        FrameContext& frameContext = m_FrameContexts[m_CurrentFrameIndex];
        CommandBufferHandle commandBuffer = executionContext.CommandBuffer;


        for (uint32 i = 0; i < frameContext.Meshes.size(); i++)
        {
            Mesh& mesh = frameContext.Meshes[i];
            Material& material = frameContext.Materials[i];

            if (material.ShaderModel != ShaderModel::PBR) { continue; }

            DescriptorSetHandle& materialDescriptorSet = material.DescriptorSet;

            material.FragmentShader = m_GeometryPassShader;

            // TODO: Refactor pipeline cache to store descriptor set layouts instead of actual descriptor set handles (its causing a memory leak on the gpu)
            PipelineStateObjectHandle pipeline = m_PipelineStateCache.GetPipeline(executionContext.RenderPass, material, mesh, 0);
            pipeline->Bind(commandBuffer);
            pipeline->SetViewportAndScissor(commandBuffer, m_ViewportSize);

            PushConstant pushConstant = {
                .ModelMatrix = frameContext.Transforms[i],
                .HasNormalMap = material.HasNormalMap
            };

            RendererAPI::PushConstants(commandBuffer, pipeline, &pushConstant, sizeof(PushConstant));

            pipeline->BindDescriptorSet(commandBuffer, frameContext.SceneDataDescriptorSet, 0);
            pipeline->BindDescriptorSet(commandBuffer, materialDescriptorSet, 1);

            mesh.VertexBuffer->Bind(commandBuffer);
            mesh.IndexBuffer->Bind(commandBuffer);
            RendererAPI::DrawElementsIndexed(commandBuffer, mesh.IndexBuffer);
        }

    }


    void DeferredRenderer::LightingPass()
    {
        const RenderGraphPassExecutionContext& executionContext = m_RenderGraph.GetExecutionContext();
        FrameContext& frameContext = m_FrameContexts[m_CurrentFrameIndex];
        CommandBufferHandle commandBuffer = executionContext.CommandBuffer;
        AssetRegistry& registry = Engine::Get().GetAssetManager().GetRegistry();


        Mesh mesh = *registry.GetAsset<Mesh>("Meshes/Quad.obj"); 
        mesh.VertexShader = registry.CreateAsset<Shader>("Shaders/Lighting_Pass_No_Transform.vert");
        frameContext.Meshes.push_back(mesh); // Hold onto reference so it is not destroyed early
        Material material{};
        material.FragmentShader = m_LightingShader;
        material.DescriptorSet = executionContext.ReadAttachments;

        Ref<Shader> vertexShader = mesh.VertexShader;

        PipelineStateObjectHandle pipeline = m_PipelineStateCache.GetPipeline(executionContext.RenderPass, material, mesh, 0);
        pipeline->Bind(commandBuffer);
        pipeline->SetViewportAndScissor(commandBuffer, m_ViewportSize);

        pipeline->BindDescriptorSet(commandBuffer, frameContext.SceneDataDescriptorSet, 0);
        pipeline->BindDescriptorSet(commandBuffer, executionContext.ReadAttachments, 1);

        mesh.VertexBuffer->Bind(commandBuffer);
        mesh.IndexBuffer->Bind(commandBuffer);
        RendererAPI::DrawElementsIndexed(commandBuffer, mesh.IndexBuffer);

    }


    void DeferredRenderer::ResizeViewport(uint32 width, uint32 height)
    {
        m_ViewportSize = UVec2(width, height);
        m_ViewportResizedPublisher.PublishEvent(ViewportResizedEvent(width, height));

        Device& device = RendererAPI::GetDevice();
        Swapchain& swapchain = device.GetSwapchain();
        std::vector<RenderTargetHandle> renderTargets = swapchain.GetRenderTargets();
        device.WaitIdle();



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

            frameContext.OffscreenRenderTarget = device.CreateTexture(textureCreateInfo);

            frameContext.OffscreenDescriptorSet = device.CreateDescriptorSet(); // TODO: Avoid creating new descriptor set handles and reuse the handle but recreate the vk descriptor set
            frameContext.OffscreenDescriptorSet->BeginBuildingSet();
            frameContext.OffscreenDescriptorSet->AddDescriptorImageSampler(frameContext.OffscreenRenderTarget, ShaderStage::FRAGMENT);
            frameContext.OffscreenDescriptorSet->EndBuildingSet();
        }

        uint32 nextFrameIndex = (m_CurrentFrameIndex + 1) % 3;
        m_CurrentViewportTexture.pop();
        m_CurrentViewportTexture.push(m_FrameContexts[nextFrameIndex].OffscreenDescriptorSet);

        // Resizing Render Graph

        std::vector<TextureHandle> outputTextures;
        constexpr int numFramesInFlight = 3;
        outputTextures.reserve(numFramesInFlight);
        for (int i = 0; i < numFramesInFlight; i++)
        {
            TextureHandle offscreenOutput = m_FrameContexts[i].OffscreenRenderTarget;
            outputTextures.push_back(offscreenOutput);
        }

        m_RenderGraph.ResizeResources(outputTextures);
    }

} // Renderer