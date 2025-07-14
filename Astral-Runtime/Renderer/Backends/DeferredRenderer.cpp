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

namespace Astral {

    void DeferredRenderer::Init()
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

        frameContext.FramesTillFree--;
        if (frameContext.FramesTillFree == 0)
        {
            frameContext.ImGuiTexturesToBeFreed.clear();
            frameContext.TexturesToBeFreed.clear();
        }
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

        RenderGraphPass geometryPass = RenderGraphPass(m_ViewportSize, "Geometry Pass", [&](){ GeometryPass(); });
        geometryPass.AddColorAttachment(albedoBufferDescription, "Geometry_Albedo", ImageLayout::COLOR_ATTACHMENT_OPTIMAL);
        geometryPass.AddColorAttachment(metallicBufferDescription, "Geometry_Metallic", ImageLayout::COLOR_ATTACHMENT_OPTIMAL);
        geometryPass.AddColorAttachment(roughnessBufferDescription, "Geometry_Roughness", ImageLayout::COLOR_ATTACHMENT_OPTIMAL);
        geometryPass.AddColorAttachment(emissionBufferDescription, "Geometry_Emission", ImageLayout::COLOR_ATTACHMENT_OPTIMAL);
        geometryPass.AddColorAttachment(normalBufferDescription, "Geometry_Normals", ImageLayout::COLOR_ATTACHMENT_OPTIMAL);
        geometryPass.AddDepthStencilAttachment(depthBufferDescription, "Depth_Buffer", ImageLayout::DEPTH_STENCIL_ATTACHMENT_OPTIMAL);



        AttachmentDescription lightingTextureDescription = {
            .Format = ImageFormat::B8G8R8A8_UNORM,
            .ImageUsageFlags = ImageUsageFlags::COLOR_ATTACHMENT_BIT,
            .LoadOp = AttachmentLoadOp::CLEAR,
            .StoreOp = AttachmentStoreOp::STORE,
            .InitialLayout = ImageLayout::SHADER_READ_ONLY_OPTIMAL,
            .FinalLayout = ImageLayout::SHADER_READ_ONLY_OPTIMAL,
            .ClearColor = Vec4(0.0, 0.0, 1.0, 1.0)
        };


        RenderGraphPass lightingPass = RenderGraphPass(m_ViewportSize, "Lighting Pass", [&](){ LightingPass(); });;
        lightingPass.AddInputAttachment(geometryPass, "Geometry_Albedo", ImageLayout::SHADER_READ_ONLY_OPTIMAL);
        lightingPass.AddInputAttachment(geometryPass, "Geometry_Metallic", ImageLayout::SHADER_READ_ONLY_OPTIMAL);
        lightingPass.AddInputAttachment(geometryPass, "Geometry_Roughness", ImageLayout::SHADER_READ_ONLY_OPTIMAL);
        lightingPass.AddInputAttachment(geometryPass, "Geometry_Emission", ImageLayout::SHADER_READ_ONLY_OPTIMAL);
        lightingPass.AddInputAttachment(geometryPass, "Geometry_Normals", ImageLayout::SHADER_READ_ONLY_OPTIMAL);
        lightingPass.AddInputAttachment(geometryPass, "Depth_Buffer", ImageLayout::SHADER_READ_ONLY_OPTIMAL);

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
        m_RenderGraph.SetOutputAttachment(lightingPass, "Deferred_Lighting_Buffer", outputTextures);
        m_RenderGraph.EndBuildingRenderGraph();
    }


    void DeferredRenderer::BuildRenderPasses()
    {
        Device& device = RendererAPI::GetDevice();
        Swapchain& swapchain = device.GetSwapchain();
        std::vector<RenderTargetHandle>& renderTargets = swapchain.GetRenderTargets();

        m_MainRenderPass = device.CreateRenderPass();

        AttachmentDescription albedoBufferDescription = {
            .Format = ImageFormat::R8G8B8A8_UNORM,
            .LoadOp = AttachmentLoadOp::CLEAR,
            .StoreOp = AttachmentStoreOp::STORE,
            .InitialLayout = ImageLayout::COLOR_ATTACHMENT_OPTIMAL,
            .FinalLayout = ImageLayout::COLOR_ATTACHMENT_OPTIMAL,
            .ClearColor = Vec4(0.0, 0.0, 0.0, 0.0)
        };

        AttachmentDescription metallicBufferDescription = {
            .Format = ImageFormat::R8_UNORM,
            .LoadOp = AttachmentLoadOp::CLEAR,
            .StoreOp = AttachmentStoreOp::STORE,
            .InitialLayout = ImageLayout::COLOR_ATTACHMENT_OPTIMAL,
            .FinalLayout = ImageLayout::COLOR_ATTACHMENT_OPTIMAL,
            .ClearColor = Vec4(0.0, 0.0, 0.0, 0.0)
        };

        AttachmentDescription roughnessBufferDescription = {
            .Format = ImageFormat::R8_UNORM,
            .LoadOp = AttachmentLoadOp::CLEAR,
            .StoreOp = AttachmentStoreOp::STORE,
            .InitialLayout = ImageLayout::COLOR_ATTACHMENT_OPTIMAL,
            .FinalLayout = ImageLayout::COLOR_ATTACHMENT_OPTIMAL,
            .ClearColor = Vec4(0.0, 0.0, 0.0, 0.0)
        };

        AttachmentDescription emissionBufferDescription = {
            .Format = ImageFormat::R8G8B8A8_UNORM,
            .LoadOp = AttachmentLoadOp::CLEAR,
            .StoreOp = AttachmentStoreOp::STORE,
            .InitialLayout = ImageLayout::COLOR_ATTACHMENT_OPTIMAL,
            .FinalLayout = ImageLayout::COLOR_ATTACHMENT_OPTIMAL,
            .ClearColor = Vec4(0.0, 0.0, 0.0, 0.0)
        };

        AttachmentDescription normalBufferDescription = {
            .Format = ImageFormat::R8G8B8A8_UNORM,
            .LoadOp = AttachmentLoadOp::CLEAR,
            .StoreOp = AttachmentStoreOp::STORE,
            .InitialLayout = ImageLayout::COLOR_ATTACHMENT_OPTIMAL,
            .FinalLayout = ImageLayout::COLOR_ATTACHMENT_OPTIMAL,
            .ClearColor = Vec4(0.0, 0.0, 0.0, 0.0)
        };

        AttachmentDescription depthBufferDescription = {
            .Format = ImageFormat::D32_SFLOAT_S8_UINT,
            .LoadOp = AttachmentLoadOp::CLEAR,
            .StoreOp = AttachmentStoreOp::STORE,
            .InitialLayout = ImageLayout::DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
            .FinalLayout = ImageLayout::DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
            .ClearColor = Vec4(1.0, 0.0, 0.0, 0.0)
        };

        AttachmentDescription offscreenTextureDescription = {
            .Format = renderTargets[0]->GetImageFormat(),
            .LoadOp = AttachmentLoadOp::CLEAR,
            .StoreOp = AttachmentStoreOp::STORE,
            .InitialLayout = ImageLayout::SHADER_READ_ONLY_OPTIMAL,
            .FinalLayout = ImageLayout::SHADER_READ_ONLY_OPTIMAL,
            .ClearColor = Vec4(0.0, 0.0, 1.0, 1.0)
        };

        m_MainRenderPass->BeginBuildingRenderPass();
        AttachmentIndex albedoTextureIndex = m_MainRenderPass->DefineAttachment(albedoBufferDescription);
        AttachmentIndex metallicTextureIndex = m_MainRenderPass->DefineAttachment(metallicBufferDescription);
        AttachmentIndex roughnessTextureIndex = m_MainRenderPass->DefineAttachment(roughnessBufferDescription);
        AttachmentIndex emissionTextureIndex = m_MainRenderPass->DefineAttachment(emissionBufferDescription);
        AttachmentIndex normalTextureIndex = m_MainRenderPass->DefineAttachment(normalBufferDescription);

        AttachmentIndex depthBufferIndex = m_MainRenderPass->DefineAttachment(depthBufferDescription);
        AttachmentIndex offscreenTextureIndex = m_MainRenderPass->DefineAttachment(offscreenTextureDescription);

        // Geometry Pass
        m_MainRenderPass->BeginBuildingSubpass();

        m_MainRenderPass->AddColorAttachment(albedoTextureIndex, ImageLayout::COLOR_ATTACHMENT_OPTIMAL);
        m_MainRenderPass->AddColorAttachment(metallicTextureIndex, ImageLayout::COLOR_ATTACHMENT_OPTIMAL);
        m_MainRenderPass->AddColorAttachment(roughnessTextureIndex, ImageLayout::COLOR_ATTACHMENT_OPTIMAL);
        m_MainRenderPass->AddColorAttachment(emissionTextureIndex, ImageLayout::COLOR_ATTACHMENT_OPTIMAL);
        m_MainRenderPass->AddColorAttachment(normalTextureIndex, ImageLayout::COLOR_ATTACHMENT_OPTIMAL);
        m_MainRenderPass->AddDepthStencilAttachment(depthBufferIndex, ImageLayout::DEPTH_STENCIL_ATTACHMENT_OPTIMAL);

        SubpassIndex geometrySubpassIndex = m_MainRenderPass->EndBuildingSubpass();

        // Lighting Pass
        m_MainRenderPass->BeginBuildingSubpass();

        m_MainRenderPass->AddInputAttachment(albedoTextureIndex, ImageLayout::SHADER_READ_ONLY_OPTIMAL);
        m_MainRenderPass->AddInputAttachment(metallicTextureIndex, ImageLayout::SHADER_READ_ONLY_OPTIMAL);
        m_MainRenderPass->AddInputAttachment(roughnessTextureIndex, ImageLayout::SHADER_READ_ONLY_OPTIMAL);
        m_MainRenderPass->AddInputAttachment(emissionTextureIndex, ImageLayout::SHADER_READ_ONLY_OPTIMAL);
        m_MainRenderPass->AddInputAttachment(normalTextureIndex, ImageLayout::SHADER_READ_ONLY_OPTIMAL);
        m_MainRenderPass->AddInputAttachment(depthBufferIndex, ImageLayout::SHADER_READ_ONLY_OPTIMAL);

        m_MainRenderPass->AddColorAttachment(offscreenTextureIndex, ImageLayout::COLOR_ATTACHMENT_OPTIMAL);
        SubpassIndex lightingSubpassIndex = m_MainRenderPass->EndBuildingSubpass();

        m_MainRenderPass->EndBuildingRenderPass();

        SubpassDependencyMasks subpassDependencyMasks = {
            .SourceStageMask       =  PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
            .DestinationStageMask  =  PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
            .SourceAccessMask      =  ACCESS_FLAGS_SHADER_WRITE_BIT,
            .DestinationAccessMask =  ACCESS_FLAGS_SHADER_READ_BIT,
        };
        m_MainRenderPass->DefineSubpassDependency(geometrySubpassIndex, lightingSubpassIndex, subpassDependencyMasks);


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


    void DeferredRenderer::CreateGBufferTextures(GBuffer& outGBuffer, UVec2 dimensions)
    {
        Device& device = RendererAPI::GetDevice();

        // Albedo Texture
        TextureCreateInfo albedoTextureCreateInfo = {
            .Format = ImageFormat::R8G8B8A8_UNORM,
            .Layout = ImageLayout::COLOR_ATTACHMENT_OPTIMAL,
            .UsageFlags = ImageUsageFlags::COLOR_ATTACHMENT_BIT,
            .Dimensions = dimensions,
            .ImageData = nullptr
        };
        outGBuffer.AlbedoBuffer = device.CreateTexture(albedoTextureCreateInfo);
        RendererAPI::NameObject(outGBuffer.AlbedoBuffer, "Albedo Buffer");

        // Metallic Texture
        TextureCreateInfo metallicTextureCreateInfo = {
            .Format = ImageFormat::R8_UNORM,
            .Layout = ImageLayout::COLOR_ATTACHMENT_OPTIMAL,
            .UsageFlags = ImageUsageFlags::COLOR_ATTACHMENT_BIT,
            .Dimensions = dimensions,
            .ImageData = nullptr
        };
        outGBuffer.MetallicBuffer = device.CreateTexture(metallicTextureCreateInfo);
        RendererAPI::NameObject(outGBuffer.MetallicBuffer, "Metallic Buffer");

        // Roughness Texture
        TextureCreateInfo roughnessTextureCreateInfo = {
            .Format = ImageFormat::R8_UNORM,
            .Layout = ImageLayout::COLOR_ATTACHMENT_OPTIMAL,
            .UsageFlags = ImageUsageFlags::COLOR_ATTACHMENT_BIT,
            .Dimensions = dimensions,
            .ImageData = nullptr
        };
        outGBuffer.RoughnessBuffer = device.CreateTexture(roughnessTextureCreateInfo);
        RendererAPI::NameObject(outGBuffer.RoughnessBuffer, "Roughness Buffer");

        // Emission Texture
        TextureCreateInfo emissionTextureCreateInfo = {
            .Format = ImageFormat::R8G8B8A8_UNORM,
            .Layout = ImageLayout::COLOR_ATTACHMENT_OPTIMAL,
            .UsageFlags = ImageUsageFlags::COLOR_ATTACHMENT_BIT,
            .Dimensions = dimensions,
            .ImageData = nullptr
        };
        outGBuffer.EmissionBuffer = device.CreateTexture(emissionTextureCreateInfo);
        RendererAPI::NameObject(outGBuffer.EmissionBuffer, "Emission Buffer");

        // Normal Texture
        TextureCreateInfo normalTextureCreateInfo = {
            .Format = ImageFormat::R8G8B8A8_UNORM,
            .Layout = ImageLayout::COLOR_ATTACHMENT_OPTIMAL,
            .UsageFlags = ImageUsageFlags::COLOR_ATTACHMENT_BIT,
            .Dimensions = dimensions,
            .ImageData = nullptr
        };
        outGBuffer.NormalBuffer = device.CreateTexture(normalTextureCreateInfo);
        RendererAPI::NameObject(outGBuffer.NormalBuffer, "Normal Buffer");

        TextureCreateInfo depthBufferTextureCreateInfo = {
            .Format = ImageFormat::D32_SFLOAT_S8_UINT,
            .Layout = ImageLayout::DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
            .UsageFlags = ImageUsageFlags::DEPTH_STENCIL_ATTACHMENT_BIT,
            .Dimensions = dimensions,
            .ImageData = nullptr
        };
        outGBuffer.DepthBuffer = device.CreateTexture(depthBufferTextureCreateInfo);
        RendererAPI::NameObject(outGBuffer.DepthBuffer, "Depth Buffer");

        outGBuffer.GBufferDescriptorSet = device.CreateDescriptorSet();
        outGBuffer.GBufferDescriptorSet->BeginBuildingSet();
        outGBuffer.GBufferDescriptorSet->AddDescriptorSubpassInputAttachment(outGBuffer.AlbedoBuffer, ShaderStage::FRAGMENT);
        outGBuffer.GBufferDescriptorSet->AddDescriptorSubpassInputAttachment(outGBuffer.MetallicBuffer, ShaderStage::FRAGMENT);
        outGBuffer.GBufferDescriptorSet->AddDescriptorSubpassInputAttachment(outGBuffer.RoughnessBuffer, ShaderStage::FRAGMENT);
        outGBuffer.GBufferDescriptorSet->AddDescriptorSubpassInputAttachment(outGBuffer.EmissionBuffer, ShaderStage::FRAGMENT);
        outGBuffer.GBufferDescriptorSet->AddDescriptorSubpassInputAttachment(outGBuffer.NormalBuffer, ShaderStage::FRAGMENT);
        outGBuffer.GBufferDescriptorSet->AddDescriptorSubpassInputAttachment(outGBuffer.DepthBuffer, ShaderStage::FRAGMENT);
        outGBuffer.GBufferDescriptorSet->EndBuildingSet();
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


            CreateGBufferTextures(context.GBuffer, renderTargets[0]->GetDimensions());

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
            context.FramesTillFree = 2;


            context.SceneCommandBuffer = device.AllocateCommandBuffer();

            context.SceneFramebuffer = device.CreateFramebuffer(m_MainRenderPass);
            UVec2 viewportDimensions = m_ViewportSize;
            context.SceneFramebuffer->BeginBuildingFramebuffer(viewportDimensions.x, viewportDimensions.y);
            context.SceneFramebuffer->AttachTexture(context.GBuffer.AlbedoBuffer);
            context.SceneFramebuffer->AttachTexture(context.GBuffer.MetallicBuffer);
            context.SceneFramebuffer->AttachTexture(context.GBuffer.RoughnessBuffer);
            context.SceneFramebuffer->AttachTexture(context.GBuffer.EmissionBuffer);
            context.SceneFramebuffer->AttachTexture(context.GBuffer.NormalBuffer);
            context.SceneFramebuffer->AttachTexture(context.GBuffer.DepthBuffer);
            context.SceneFramebuffer->AttachTexture(context.OffscreenRenderTarget);
            context.SceneFramebuffer->EndBuildingFramebuffer();
            RendererAPI::NameObject(context.OffscreenRenderTarget, "Offscreen Render Target");


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
        FramebufferHandle mainFramebufferHandle = frameContext.SceneFramebuffer;

        commandBuffer->BeginRecording();

        // Viewport Rendering
        m_RenderGraph.Execute(commandBuffer, m_CurrentFrameIndex);


        // ImGui Rendering
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


        Mesh mesh = *registry.GetAsset<Mesh>("Meshes/Quad.obj"); // TODO: Fix quad to have the correct UVs on load (right now its flipped)
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

        uint32 nextOldestFrameIndex = m_CurrentFrameIndex;
        m_CurrentViewportTexture.push(m_FrameContexts[nextOldestFrameIndex].OffscreenDescriptorSet);


        for (int i = 0; i < swapchain.GetNumberOfImages(); i++)
        {
            FrameContext& frameContext = m_FrameContexts[i];

            CreateGBufferTextures(frameContext.GBuffer, UVec2(width, height));

            TextureCreateInfo textureCreateInfo = {
                .Format = renderTargets[0]->GetImageFormat(),
                .Layout = ImageLayout::SHADER_READ_ONLY_OPTIMAL,
                .UsageFlags = ImageUsageFlags::COLOR_ATTACHMENT_BIT,
                .Dimensions = UVec2(width, height),
                .ImageData = nullptr
            };
            m_FrameContexts[m_CurrentFrameIndex].FramesTillFree = 2;
            m_FrameContexts[m_CurrentFrameIndex].ImGuiTexturesToBeFreed.push_back(frameContext.OffscreenDescriptorSet);

            frameContext.OffscreenRenderTarget = device.CreateTexture(textureCreateInfo);

            frameContext.OffscreenDescriptorSet = device.CreateDescriptorSet();
            frameContext.OffscreenDescriptorSet->BeginBuildingSet();
            frameContext.OffscreenDescriptorSet->AddDescriptorImageSampler(frameContext.OffscreenRenderTarget, ShaderStage::FRAGMENT);
            frameContext.OffscreenDescriptorSet->EndBuildingSet();

            frameContext.SceneFramebuffer = device.CreateFramebuffer(m_MainRenderPass);
            FramebufferHandle framebuffer = frameContext.SceneFramebuffer;

            framebuffer->BeginBuildingFramebuffer(width, height);
            framebuffer->AttachTexture(frameContext.GBuffer.AlbedoBuffer);
            framebuffer->AttachTexture(frameContext.GBuffer.MetallicBuffer);
            framebuffer->AttachTexture(frameContext.GBuffer.RoughnessBuffer);
            framebuffer->AttachTexture(frameContext.GBuffer.EmissionBuffer);
            framebuffer->AttachTexture(frameContext.GBuffer.NormalBuffer);
            framebuffer->AttachTexture(frameContext.GBuffer.DepthBuffer);
            framebuffer->AttachTexture(frameContext.OffscreenRenderTarget);
            framebuffer->EndBuildingFramebuffer();
        }

        // TODO: Resize the render graph and pass new offscreen textures to it for outputs

    }

} // Renderer