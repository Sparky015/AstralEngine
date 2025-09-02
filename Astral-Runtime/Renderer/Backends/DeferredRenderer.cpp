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
#include "ECS/SceneManager.h"
#include "Window/WindowManager.h"

namespace Astral {

    void DeferredRenderer::Init()
    {
        PROFILE_SCOPE("SceneRenderer::Init")

        m_WindowResizedListener = EventListener<FramebufferResizedEvent>{[this](FramebufferResizedEvent event) { ResizeWindowImages(event.Width, event.Height); }};
        m_WindowResizedListener.StartListening();

        m_ViewportSize = RendererAPI::GetContext().GetFramebufferSize();
        m_CurrentFrameIndex = 0;


        // Building the imgui render pass
        BuildImGuiEditorRenderPass();


        AttachmentDescription irradianceAttachmentDescription = {
            .Format = ImageFormat::R16G16B16A16_SFLOAT,
            .ImageUsageFlags = IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
            .LoadOp = AttachmentLoadOp::CLEAR,
            .StoreOp = AttachmentStoreOp::STORE,
            .InitialLayout = ImageLayout::SHADER_READ_ONLY_OPTIMAL,
            .FinalLayout = ImageLayout::SHADER_READ_ONLY_OPTIMAL,
            .ClearColor = UVec4(0, 0, 0, 1.0f)
        };

        m_IrradianceCalcPass = RendererAPI::GetDevice().CreateRenderPass();
        m_IrradianceCalcPass->BeginBuildingRenderPass();
        m_IrradianceCalcPass->BeginBuildingSubpass();
        AttachmentIndex irradianceAttachment = m_IrradianceCalcPass->DefineAttachment(irradianceAttachmentDescription);
        m_IrradianceCalcPass->AddColorAttachment(irradianceAttachment, ImageLayout::COLOR_ATTACHMENT_OPTIMAL);
        m_IrradianceCalcPass->EndBuildingSubpass();
        m_IrradianceCalcPass->EndBuildingRenderPass();


        // Initializing the resources that are allocated per swapchain image
        InitializeFrameResources();

        // BuildRenderGraphForDeferred();
        BuildRenderGraphForForward();

        m_PipelineStateCache.SetDescriptorSetStack(m_FrameContexts[0].SceneDataDescriptorSet);
        m_CurrentViewportTexture.push(m_FrameContexts[1].OffscreenDescriptorSet);

        Engine::Get().GetRendererManager().GetContext().InitImGuiForAPIBackend(m_ImGuiRenderPass);
        AssetRegistry& registry = Engine::Get().GetAssetManager().GetRegistry();
        m_GeometryPassShader = registry.CreateAsset<Shader>("Shaders/DeferredGeometryPassUnpacked.frag");
        m_LightingShader = registry.CreateAsset<Shader>("Shaders/DeferredLightingPass.frag");

        TextureHandle toneMappingLUT = registry.CreateAsset<Texture>("LUTs/acescg_to_rec709_linear_no_shaper.cube");
        m_ToneMappingLUTDescriptorSet = RendererAPI::GetDevice().CreateDescriptorSet();
        m_ToneMappingLUTDescriptorSet->BeginBuildingSet();
        m_ToneMappingLUTDescriptorSet->AddDescriptorImageSampler(toneMappingLUT, ShaderStage::FRAGMENT);
        m_ToneMappingLUTDescriptorSet->EndBuildingSet();




        // Renderer Settings
        RendererSettings rendererSettings;
        rendererSettings.IsVSyncEnabled = true;

        SetRendererSettings(rendererSettings);
    }


    void DeferredRenderer::Shutdown()
    {
        PROFILE_SCOPE("SceneRenderer::Shutdown")
        m_WindowResizedListener.StopListening();

        Device& device = RendererAPI::GetDevice();
        device.WaitIdle();

        Engine::Get().GetRendererManager().GetContext().ShutdownImGuiForAPIBackend();
        m_FrameContexts.clear();
        m_RenderGraph.ClearResourceHold();
    }


    static constexpr uint32 EnvironmentMapIrradianceSize = 64;

    void DeferredRenderer::BeginScene(const SceneDescription& sceneDescription)
    {
        PROFILE_SCOPE("SceneRenderer::BeginScene")
        Device& device = RendererAPI::GetDevice();
        Swapchain& swapchain = device.GetSwapchain();

        RenderTargetHandle renderTarget;
        {
            PROFILE_SCOPE("SceneRenderer::BeginScene::AcquireNextImage")
            renderTarget = swapchain.AcquireNextImage();
        }

        m_IsSceneStarted = true;
        m_CurrentFrameIndex++;
        if (m_CurrentFrameIndex == 3) { m_CurrentFrameIndex = 0; }

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
            frameContext.SceneDataDescriptorSet->UpdateStorageBufferBinding(1, frameContext.SceneLightsBuffer); // Reallocation will create a new buffer, so re-add that buffer to descriptor set
        }
        frameContext.SceneLightsBuffer->CopyDataToBuffer(sceneDescription.Lights.data(), sizeof(Light) * sceneData.NumLights);

        if (sceneDescription.EnvironmentMap)
        {
            frameContext.EnvironmentMap = sceneDescription.EnvironmentMap;
            frameContext.EnvironmentMapDescriptorSet->UpdateImageSamplerBinding(0, sceneDescription.EnvironmentMap->PrefilteredEnvironment, ImageLayout::GENERAL);

            if (!sceneDescription.EnvironmentMap->Irradiance)
            {
                TextureCreateInfo irradianceTextureCreateInfo = {
                    .Format = ImageFormat::R16G16B16A16_SFLOAT,
                    .Layout = ImageLayout::GENERAL,
                    .UsageFlags = IMAGE_USAGE_SAMPLED_BIT | IMAGE_USAGE_STORAGE_BIT,
                    .Dimensions = UVec2(EnvironmentMapIrradianceSize, EnvironmentMapIrradianceSize),
                    .ImageData = (uint8*)nullptr,
                };

                sceneDescription.EnvironmentMap->Irradiance = Texture::CreateCubemap(irradianceTextureCreateInfo);
                // std::string irradianceMapName = std::string("Cubemaps/pretoria_gardens_4k.hdr_Irradiance");
                // RendererAPI::NameObject(sceneDescription.EnvironmentMap->Irradiance, irradianceMapName);

                m_EnvironmentMapStorageImagesSet = device.CreateDescriptorSet();
                m_EnvironmentMapStorageImagesSet->BeginBuildingSet();
                m_EnvironmentMapStorageImagesSet->AddDescriptorImageSampler(frameContext.EnvironmentMap->Environment, ShaderStage::COMPUTE);
                m_EnvironmentMapStorageImagesSet->AddDescriptorStorageImage(frameContext.EnvironmentMap->Irradiance, ShaderStage::COMPUTE, ImageLayout::GENERAL);
                m_EnvironmentMapStorageImagesSet->AddDescriptorStorageImage(frameContext.EnvironmentMap->PrefilteredEnvironment, ShaderStage::COMPUTE, ImageLayout::GENERAL);
                m_EnvironmentMapStorageImagesSet->EndBuildingSet();

                frameContext.IsIrradianceMapCalculationNeeded = true;
            }

            frameContext.EnvironmentMapDescriptorSet->UpdateImageSamplerBinding(1, sceneDescription.EnvironmentMap->Irradiance, ImageLayout::GENERAL);
        }

        frameContext.Meshes.clear();
        frameContext.Materials.clear();
        frameContext.Transforms.clear();

        m_SceneExposure = sceneDescription.Exposure;
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


    void DeferredRenderer::SetRendererSettings(const RendererSettings& rendererSettings)
    {
        if (m_RendererSettings.IsVSyncEnabled != rendererSettings.IsVSyncEnabled)
        {
            m_RendererSettings.IsVSyncEnabled = rendererSettings.IsVSyncEnabled;
            SetVSync(m_RendererSettings.IsVSyncEnabled);
        }
    }


    const RendererSettings& DeferredRenderer::GetRendererSettings()
    {
        return m_RendererSettings;
    }


    DescriptorSetHandle DeferredRenderer::GetViewportTexture()
    {
        DescriptorSetHandle& descriptorSet = m_CurrentViewportTexture.front();
        m_CurrentViewportTexture.pop();
        return descriptorSet;
    }


    void DeferredRenderer::BuildRenderGraphForDeferred()
    {

        AttachmentDescription albedoBufferDescription = {
            .Format = ImageFormat::R8G8B8A8_UNORM,
            .ImageUsageFlags = IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
            .LoadOp = AttachmentLoadOp::CLEAR,
            .StoreOp = AttachmentStoreOp::STORE,
            .InitialLayout = ImageLayout::COLOR_ATTACHMENT_OPTIMAL,
            .FinalLayout = ImageLayout::COLOR_ATTACHMENT_OPTIMAL,
            .ClearColor = Vec4(0.0, 0.0, 0.0, 1.0)
        };

        AttachmentDescription metallicBufferDescription = {
            .Format = ImageFormat::R8_UNORM,
            .ImageUsageFlags = IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
            .LoadOp = AttachmentLoadOp::CLEAR,
            .StoreOp = AttachmentStoreOp::STORE,
            .InitialLayout = ImageLayout::COLOR_ATTACHMENT_OPTIMAL,
            .FinalLayout = ImageLayout::COLOR_ATTACHMENT_OPTIMAL,
            .ClearColor = Vec4(0.0, 0.0, 0.0, 1.0)
        };

        AttachmentDescription roughnessBufferDescription = {
            .Format = ImageFormat::R8_UNORM,
            .ImageUsageFlags = IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
            .LoadOp = AttachmentLoadOp::CLEAR,
            .StoreOp = AttachmentStoreOp::STORE,
            .InitialLayout = ImageLayout::COLOR_ATTACHMENT_OPTIMAL,
            .FinalLayout = ImageLayout::COLOR_ATTACHMENT_OPTIMAL,
            .ClearColor = Vec4(0.0, 0.0, 0.0, 1.0)
        };

        AttachmentDescription emissionBufferDescription = {
            .Format = ImageFormat::R8G8B8A8_UNORM,
            .ImageUsageFlags = IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
            .LoadOp = AttachmentLoadOp::CLEAR,
            .StoreOp = AttachmentStoreOp::STORE,
            .InitialLayout = ImageLayout::COLOR_ATTACHMENT_OPTIMAL,
            .FinalLayout = ImageLayout::COLOR_ATTACHMENT_OPTIMAL,
            .ClearColor = Vec4(0.0, 0.0, 0.0, 1.0)
        };

        AttachmentDescription normalBufferDescription = {
            .Format = ImageFormat::R8G8B8A8_UNORM,
            .ImageUsageFlags = IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
            .LoadOp = AttachmentLoadOp::CLEAR,
            .StoreOp = AttachmentStoreOp::STORE,
            .InitialLayout = ImageLayout::COLOR_ATTACHMENT_OPTIMAL,
            .FinalLayout = ImageLayout::COLOR_ATTACHMENT_OPTIMAL,
            .ClearColor = Vec4(0.0, 0.0, 0.0, 1.0)
        };

        AttachmentDescription depthBufferDescription = {
            .Format = ImageFormat::D32_SFLOAT_S8_UINT,
            .ImageUsageFlags = IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
            .LoadOp = AttachmentLoadOp::CLEAR,
            .StoreOp = AttachmentStoreOp::STORE,
            .InitialLayout = ImageLayout::DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
            .FinalLayout = ImageLayout::DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
            .ClearColor = Vec4(1.0, 0.0, 0.0, 1.0)
        };

        RenderGraphPass geometryPass = RenderGraphPass(OutputAttachmentDimensions, "GBuffer Pass", [&](){ GeometryPass(); });
        geometryPass.CreateColorAttachment(albedoBufferDescription, "GBuffer_Albedo", ImageLayout::COLOR_ATTACHMENT_OPTIMAL);
        geometryPass.CreateColorAttachment(metallicBufferDescription, "GBuffer_Metallic", ImageLayout::COLOR_ATTACHMENT_OPTIMAL);
        geometryPass.CreateColorAttachment(roughnessBufferDescription, "GBuffer_Roughness", ImageLayout::COLOR_ATTACHMENT_OPTIMAL);
        geometryPass.CreateColorAttachment(emissionBufferDescription, "GBuffer_Emission", ImageLayout::COLOR_ATTACHMENT_OPTIMAL);
        geometryPass.CreateColorAttachment(normalBufferDescription, "GBuffer_Normals", ImageLayout::COLOR_ATTACHMENT_OPTIMAL);
        geometryPass.CreateDepthStencilAttachment(depthBufferDescription, "GBuffer_Depth_Buffer", ImageLayout::DEPTH_STENCIL_ATTACHMENT_OPTIMAL);



        AttachmentDescription lightingTextureDescription = {
            .Format = ImageFormat::R16G16B16A16_SFLOAT,
            .ImageUsageFlags = IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
            .LoadOp = AttachmentLoadOp::CLEAR,
            .StoreOp = AttachmentStoreOp::STORE,
            .InitialLayout = ImageLayout::SHADER_READ_ONLY_OPTIMAL,
            .FinalLayout = ImageLayout::SHADER_READ_ONLY_OPTIMAL,
            .ClearColor = Vec4(0.0, 0.0, 1.0, 1.0)
        };


        RenderGraphPass lightingPass = RenderGraphPass(OutputAttachmentDimensions, "Lighting Pass", [&](){ DeferredLightingPass(); });
        lightingPass.LinkReadInputAttachment(&geometryPass, "GBuffer_Albedo", ImageLayout::SHADER_READ_ONLY_OPTIMAL);
        lightingPass.LinkReadInputAttachment(&geometryPass, "GBuffer_Metallic", ImageLayout::SHADER_READ_ONLY_OPTIMAL);
        lightingPass.LinkReadInputAttachment(&geometryPass, "GBuffer_Roughness", ImageLayout::SHADER_READ_ONLY_OPTIMAL);
        lightingPass.LinkReadInputAttachment(&geometryPass, "GBuffer_Emission", ImageLayout::SHADER_READ_ONLY_OPTIMAL);
        lightingPass.LinkReadInputAttachment(&geometryPass, "GBuffer_Normals", ImageLayout::SHADER_READ_ONLY_OPTIMAL);
        lightingPass.LinkReadInputAttachment(&geometryPass, "GBuffer_Depth_Buffer", ImageLayout::SHADER_READ_ONLY_OPTIMAL);

        lightingPass.CreateColorAttachment(lightingTextureDescription, "Deferred_Lighting_Buffer", ImageLayout::COLOR_ATTACHMENT_OPTIMAL);


        RenderGraphPass cubemapPass = RenderGraphPass(OutputAttachmentDimensions, "Cubemap Pass", [&](){ EnvironmentMapPass(); });
        cubemapPass.LinkWriteInputAttachment(&lightingPass, "Deferred_Lighting_Buffer", ImageLayout::COLOR_ATTACHMENT_OPTIMAL);
        cubemapPass.LinkWriteInputAttachment(&geometryPass, "GBuffer_Depth_Buffer", ImageLayout::DEPTH_STENCIL_ATTACHMENT_OPTIMAL);


        AttachmentDescription toneMappingOutputTextureDescription = {
            .Format = ImageFormat::R8G8B8A8_UNORM,
            .ImageUsageFlags = IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
            .LoadOp = AttachmentLoadOp::CLEAR,
            .StoreOp = AttachmentStoreOp::STORE,
            .InitialLayout = ImageLayout::COLOR_ATTACHMENT_OPTIMAL,
            .FinalLayout = ImageLayout::COLOR_ATTACHMENT_OPTIMAL,
            .ClearColor = Vec4(0.0, 0.0, 1.0, 1.0)
        };

        RenderGraphPass tonemappingPass = RenderGraphPass(OutputAttachmentDimensions, "Tonemapping Pass", [&](){ ToneMappingPass(); });
        tonemappingPass.LinkReadInputAttachment(&lightingPass, "Deferred_Lighting_Buffer", ImageLayout::SHADER_READ_ONLY_OPTIMAL);
        tonemappingPass.CreateColorAttachment(toneMappingOutputTextureDescription, "Tonemapping_Output_Buffer", ImageLayout::COLOR_ATTACHMENT_OPTIMAL);
        tonemappingPass.AddDependency(&cubemapPass);

        AttachmentDescription fxaaOutputTextureDescription = {
            .Format = ImageFormat::B8G8R8A8_UNORM,
            .ImageUsageFlags = IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
            .LoadOp = AttachmentLoadOp::CLEAR,
            .StoreOp = AttachmentStoreOp::STORE,
            .InitialLayout = ImageLayout::COLOR_ATTACHMENT_OPTIMAL,
            .FinalLayout = ImageLayout::COLOR_ATTACHMENT_OPTIMAL,
            .ClearColor = Vec4(0.0, 0.0, 1.0, 1.0)
        };

        RenderGraphPass fxaaPass = RenderGraphPass(OutputAttachmentDimensions, "FXAA Pass", [&](){ FXAAPass(); });
        fxaaPass.LinkReadInputAttachment(&tonemappingPass, "Tonemapping_Output_Buffer", ImageLayout::SHADER_READ_ONLY_OPTIMAL);
        fxaaPass.CreateColorAttachment(fxaaOutputTextureDescription, "FXAA_Output_Buffer", ImageLayout::COLOR_ATTACHMENT_OPTIMAL);


        std::vector<TextureHandle> outputTextures;
        constexpr int numFramesInFlight = 3;
        outputTextures.reserve(numFramesInFlight);
        for (int i = 0; i < numFramesInFlight; i++)
        {
            TextureHandle offscreenOutput = m_FrameContexts[i].OffscreenRenderTarget;
            outputTextures.push_back(offscreenOutput);
        }

        uint32 maxFramesInFlight = m_FrameContexts.size();


        m_RenderGraph.BeginBuildingRenderGraph(maxFramesInFlight, "World Rendering");
        m_RenderGraph.AddPass(geometryPass);
        m_RenderGraph.AddPass(lightingPass);
        m_RenderGraph.AddPass(cubemapPass);
        m_RenderGraph.AddPass(tonemappingPass);
        m_RenderGraph.AddOutputPass(fxaaPass);
        m_RenderGraph.SetOutputAttachment(fxaaPass, "FXAA_Output_Buffer", outputTextures);
        m_RenderGraph.EndBuildingRenderGraph();
    }


    void DeferredRenderer::BuildRenderGraphForForward()
    {

        AttachmentDescription depthBufferDescription = {
            .Format = ImageFormat::D32_SFLOAT_S8_UINT,
            .ImageUsageFlags = IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
            .LoadOp = AttachmentLoadOp::CLEAR,
            .StoreOp = AttachmentStoreOp::STORE,
            .InitialLayout = ImageLayout::DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
            .FinalLayout = ImageLayout::DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
            .ClearColor = Vec4(1.0, 0.0, 0.0, 1.0)
        };

        RenderGraphPass depthPrePass = RenderGraphPass(OutputAttachmentDimensions, "Depth Pre-Pass", [&](){ DepthPrePass(); });
        depthPrePass.CreateDepthStencilAttachment(depthBufferDescription, "Forward_Depth_Buffer", ImageLayout::DEPTH_STENCIL_ATTACHMENT_OPTIMAL);



        AttachmentDescription lightingTextureDescription = {
            .Format = ImageFormat::R16G16B16A16_SFLOAT,
            .ImageUsageFlags = IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
            .LoadOp = AttachmentLoadOp::CLEAR,
            .StoreOp = AttachmentStoreOp::STORE,
            .InitialLayout = ImageLayout::SHADER_READ_ONLY_OPTIMAL,
            .FinalLayout = ImageLayout::SHADER_READ_ONLY_OPTIMAL,
            .ClearColor = Vec4(0.0, 0.0, 1.0, 1.0)
        };


        RenderGraphPass lightingPass = RenderGraphPass(OutputAttachmentDimensions, "Lighting Pass", [&](){ ForwardLightingPass(); });
        lightingPass.CreateColorAttachment(lightingTextureDescription, "Forward_Lighting_Buffer", ImageLayout::COLOR_ATTACHMENT_OPTIMAL);
        lightingPass.LinkWriteInputAttachment(&depthPrePass, "Forward_Depth_Buffer", ImageLayout::DEPTH_STENCIL_ATTACHMENT_OPTIMAL);


        RenderGraphPass cubemapPass = RenderGraphPass(OutputAttachmentDimensions, "Cubemap Pass", [&](){ EnvironmentMapPass(); });
        cubemapPass.LinkWriteInputAttachment(&lightingPass, "Forward_Lighting_Buffer", ImageLayout::COLOR_ATTACHMENT_OPTIMAL);
        cubemapPass.LinkWriteInputAttachment(&depthPrePass, "Forward_Depth_Buffer", ImageLayout::DEPTH_STENCIL_ATTACHMENT_OPTIMAL);


        AttachmentDescription toneMappingOutputTextureDescription = {
            .Format = ImageFormat::R8G8B8A8_UNORM,
            .ImageUsageFlags = IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
            .LoadOp = AttachmentLoadOp::CLEAR,
            .StoreOp = AttachmentStoreOp::STORE,
            .InitialLayout = ImageLayout::COLOR_ATTACHMENT_OPTIMAL,
            .FinalLayout = ImageLayout::COLOR_ATTACHMENT_OPTIMAL,
            .ClearColor = Vec4(0.0, 0.0, 1.0, 1.0)
        };

        RenderGraphPass tonemappingPass = RenderGraphPass(OutputAttachmentDimensions, "Tonemapping Pass", [&](){ ToneMappingPass(); });
        tonemappingPass.LinkReadInputAttachment(&lightingPass, "Forward_Lighting_Buffer", ImageLayout::SHADER_READ_ONLY_OPTIMAL);
        tonemappingPass.CreateColorAttachment(toneMappingOutputTextureDescription, "Tonemapping_Output_Buffer", ImageLayout::COLOR_ATTACHMENT_OPTIMAL);
        tonemappingPass.AddDependency(&cubemapPass);

        AttachmentDescription fxaaOutputTextureDescription = {
            .Format = ImageFormat::B8G8R8A8_UNORM,
            .ImageUsageFlags = IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
            .LoadOp = AttachmentLoadOp::CLEAR,
            .StoreOp = AttachmentStoreOp::STORE,
            .InitialLayout = ImageLayout::COLOR_ATTACHMENT_OPTIMAL,
            .FinalLayout = ImageLayout::COLOR_ATTACHMENT_OPTIMAL,
            .ClearColor = Vec4(0.0, 0.0, 1.0, 1.0)
        };

        RenderGraphPass fxaaPass = RenderGraphPass(OutputAttachmentDimensions, "FXAA Pass", [&](){ FXAAPass(); });
        fxaaPass.LinkReadInputAttachment(&tonemappingPass, "Tonemapping_Output_Buffer", ImageLayout::SHADER_READ_ONLY_OPTIMAL);
        fxaaPass.CreateColorAttachment(fxaaOutputTextureDescription, "FXAA_Output_Buffer", ImageLayout::COLOR_ATTACHMENT_OPTIMAL);


        std::vector<TextureHandle> outputTextures;
        constexpr int numFramesInFlight = 3;
        outputTextures.reserve(numFramesInFlight);
        for (int i = 0; i < numFramesInFlight; i++)
        {
            TextureHandle offscreenOutput = m_FrameContexts[i].OffscreenRenderTarget;
            outputTextures.push_back(offscreenOutput);
        }

        uint32 maxFramesInFlight = m_FrameContexts.size();


        m_RenderGraph.BeginBuildingRenderGraph(maxFramesInFlight, "World Rendering");
        m_RenderGraph.AddPass(depthPrePass);
        m_RenderGraph.AddPass(lightingPass);
        m_RenderGraph.AddPass(cubemapPass);
        m_RenderGraph.AddPass(tonemappingPass);
        m_RenderGraph.AddOutputPass(fxaaPass);
        m_RenderGraph.SetOutputAttachment(fxaaPass, "FXAA_Output_Buffer", outputTextures);
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
            .ClearColor = Vec4(0.0, 0.0, 0.0, 1.0)
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
        AssetRegistry& registry = Engine::Get().GetAssetManager().GetRegistry();

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
                .UsageFlags = IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
                .Dimensions = renderTargets[0]->GetDimensions(),
                .ImageData = nullptr,
                .LayerCount = 1,
                .MipMapCount = 1,
            };
            context.OffscreenRenderTarget = device.CreateTexture(textureCreateInfo);
            std::string offscreenRenderTargetName = std::string("Offscreen_Render_Target_") + std::to_string(i);
            RendererAPI::NameObject(context.OffscreenRenderTarget, offscreenRenderTargetName);




            context.OffscreenDescriptorSet = device.CreateDescriptorSet();
            context.OffscreenDescriptorSet->BeginBuildingSet();
            context.OffscreenDescriptorSet->AddDescriptorImageSampler(context.OffscreenRenderTarget, ShaderStage::FRAGMENT);
            context.OffscreenDescriptorSet->EndBuildingSet();


            context.SceneCommandBuffer = device.AllocateCommandBuffer();
            std::string commandBufferName = std::string("Scene_Command_Buffer_") + std::to_string(i);
            RendererAPI::NameObject(context.SceneCommandBuffer, commandBufferName);
            context.SceneRenderTarget = nullptr;


            context.SceneDataBuffer = device.CreateUniformBuffer(nullptr, sizeof(SceneData));
            std::string sceneDataBufferName = std::string("Scene_Data_Buffer_") + std::to_string(i);
            RendererAPI::NameObject(context.SceneDataBuffer, sceneDataBufferName);

            context.SceneLightsBuffer = device.CreateStorageBuffer(nullptr, 1024);
            std::string sceneLightsBufferName = std::string("Scene_Lights_Buffer_") + std::to_string(i);
            RendererAPI::NameObject(context.SceneLightsBuffer, sceneLightsBufferName);

            context.SceneDataDescriptorSet = device.CreateDescriptorSet();
            context.SceneDataDescriptorSet->BeginBuildingSet();
            context.SceneDataDescriptorSet->AddDescriptorUniformBuffer(context.SceneDataBuffer, ShaderStage::ALL);
            context.SceneDataDescriptorSet->AddDescriptorStorageBuffer(context.SceneLightsBuffer, ShaderStage::ALL);
            context.SceneDataDescriptorSet->EndBuildingSet();
            std::string sceneDataDescriptorSetName = std::string("Scene_Data_Descriptor_Set_") + std::to_string(i);
            RendererAPI::NameObject(context.SceneDataDescriptorSet, sceneDataDescriptorSetName);

            context.WindowFramebuffer = device.CreateFramebuffer(m_ImGuiRenderPass);
            UVec2 frameBufferDimensions = renderingContext.GetFramebufferSize();
            context.WindowFramebuffer->BeginBuildingFramebuffer(frameBufferDimensions.x, frameBufferDimensions.y);
            context.WindowFramebuffer->AttachRenderTarget(renderTargets[i]);
            context.WindowFramebuffer->EndBuildingFramebuffer();
            std::string windowFramebufferName = std::string("Window_Framebuffer_") + std::to_string(i);
            RendererAPI::NameObject(context.WindowFramebuffer, windowFramebufferName);


            Ref<EnvironmentMap> environmentMap = registry.CreateAsset<EnvironmentMap>("Cubemaps/pretoria_gardens_4k.hdr");
            context.EnvironmentMap = environmentMap;

            if (!environmentMap->Irradiance)
            {
                TextureCreateInfo irradianceTextureCreateInfo = {
                    .Format = ImageFormat::R16G16B16A16_SFLOAT,
                    .Layout = ImageLayout::GENERAL,
                    .UsageFlags = IMAGE_USAGE_SAMPLED_BIT | IMAGE_USAGE_STORAGE_BIT,
                    .Dimensions = UVec2(EnvironmentMapIrradianceSize, EnvironmentMapIrradianceSize),
                    .ImageData = (uint8*)nullptr,
                };

                environmentMap->Irradiance = Texture::CreateCubemap(irradianceTextureCreateInfo);
                std::string irradianceMapName = std::string("Cubemaps/pretoria_gardens_4k.hdr_Irradiance_") + std::to_string(i);
                RendererAPI::NameObject(environmentMap->Irradiance, irradianceMapName);
            }


            TextureHandle brdfLut = registry.CreateAsset<Texture>("LUTs/ibl_brdf_lut.dds");

            context.EnvironmentMapDescriptorSet = device.CreateDescriptorSet();
            context.EnvironmentMapDescriptorSet->BeginBuildingSet();
            context.EnvironmentMapDescriptorSet->AddDescriptorImageSampler(environmentMap->PrefilteredEnvironment, ShaderStage::FRAGMENT, ImageLayout::GENERAL);
            context.EnvironmentMapDescriptorSet->AddDescriptorImageSampler(environmentMap->Irradiance, ShaderStage::FRAGMENT, ImageLayout::GENERAL);
            context.EnvironmentMapDescriptorSet->AddDescriptorImageSampler(brdfLut, ShaderStage::FRAGMENT);
            context.EnvironmentMapDescriptorSet->EndBuildingSet();
            std::string environmentMapDescriptorSetName = std::string("Environment_Map_Descriptor_Set_") + std::to_string(i);
            RendererAPI::NameObject(context.EnvironmentMapDescriptorSet, environmentMapDescriptorSetName);

            context.IsIrradianceMapCalculationNeeded = true;
        }


        FrameContext& context = m_FrameContexts[0];
        m_EnvironmentMapStorageImagesSet = device.CreateDescriptorSet();
        m_EnvironmentMapStorageImagesSet->BeginBuildingSet();
        m_EnvironmentMapStorageImagesSet->AddDescriptorImageSampler(context.EnvironmentMap->Environment, ShaderStage::COMPUTE);
        m_EnvironmentMapStorageImagesSet->AddDescriptorStorageImage(context.EnvironmentMap->Irradiance, ShaderStage::COMPUTE, ImageLayout::GENERAL);
        m_EnvironmentMapStorageImagesSet->AddDescriptorStorageImage(context.EnvironmentMap->PrefilteredEnvironment, ShaderStage::COMPUTE, ImageLayout::GENERAL);
        m_EnvironmentMapStorageImagesSet->EndBuildingSet();
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

        // Irradiance Map calculation if empty
        if (frameContext.IsIrradianceMapCalculationNeeded)
        {
            RendererAPI::ExecuteOneTimeAndBlock([this](CommandBufferHandle asyncCommandBuffer){ ComputeIrradianceMap(asyncCommandBuffer); });

            uint32 totalMipLevels = frameContext.EnvironmentMap->PrefilteredEnvironment->GetNumMipLevels();
            uint32 mipWidth = frameContext.EnvironmentMap->PrefilteredEnvironment->GetWidth();
            uint32 mipHeight = frameContext.EnvironmentMap->PrefilteredEnvironment->GetHeight();

            for (uint32 mipLevel = 0; mipLevel < totalMipLevels; mipLevel++)
            {
                m_EnvironmentMapStorageImagesSet->UpdateStorageImageBinding(2, frameContext.EnvironmentMap->PrefilteredEnvironment, mipLevel, ImageLayout::GENERAL);
                UVec2 mipDimensions = UVec2(mipWidth, mipHeight);
                RendererAPI::ExecuteOneTimeAndBlock([&](CommandBufferHandle asyncCommandBuffer){ ComputePrefilteredEnvironmentMap(asyncCommandBuffer, mipLevel, mipDimensions); });
                if (mipWidth > 1)  { mipWidth /= 2; }
                if (mipHeight > 1) { mipHeight /= 2; }
            }
            frameContext.IsIrradianceMapCalculationNeeded = false;
        }


        // Viewport Rendering
        m_RenderGraph.Execute(commandBuffer, m_CurrentFrameIndex);


        TextureHandle offscreenRenderTarget = m_FrameContexts[m_CurrentFrameIndex].OffscreenRenderTarget;
        ImageLayout initialLayout = offscreenRenderTarget->GetLayout();
        {
            PipelineBarrier pipelineBarrier = {};
            pipelineBarrier.SourceStageMask = PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
            pipelineBarrier.DestinationStageMask = PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
            pipelineBarrier.DependencyFlags = DependencyFlags::BY_REGION_BIT;


            ImageMemoryBarrier imageMemoryBarrier = {};
            imageMemoryBarrier.SourceAccessMask = ACCESS_FLAGS_COLOR_ATTACHMENT_WRITE_BIT;
            imageMemoryBarrier.DestinationAccessMask = ACCESS_FLAGS_SHADER_READ_BIT;
            imageMemoryBarrier.OldLayout = offscreenRenderTarget->GetLayout();
            imageMemoryBarrier.NewLayout = ImageLayout::SHADER_READ_ONLY_OPTIMAL;
            imageMemoryBarrier.SourceQueueFamilyIndex = QueueFamilyIgnored;
            imageMemoryBarrier.DestinationQueueFamilyIndex = QueueFamilyIgnored;
            imageMemoryBarrier.Image = offscreenRenderTarget;
            imageMemoryBarrier.ImageSubresourceRange = {
                .AspectMask = offscreenRenderTarget->GetImageAspect(),
                .BaseMipLevel = 0,
                .LevelCount = offscreenRenderTarget->GetNumMipLevels(),
                .BaseArrayLayer = 0,
                .LayerCount = offscreenRenderTarget->GetNumLayers()
            };
            pipelineBarrier.ImageMemoryBarriers.push_back(imageMemoryBarrier);

            RendererAPI::SetPipelineBarrier(commandBuffer, pipelineBarrier);
        }


        // ImGui Rendering
        RendererAPI::BeginLabel(commandBuffer, "ImGui Render Draws", Vec4(0.0f, 0.0f, 1.0f, 1.0f));
        m_ImGuiRenderPass->BeginRenderPass(commandBuffer, m_FrameContexts[renderTarget->GetImageIndex()].WindowFramebuffer);
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
            imageMemoryBarrier.OldLayout = offscreenRenderTarget->GetLayout();
            imageMemoryBarrier.NewLayout = initialLayout;
            imageMemoryBarrier.SourceQueueFamilyIndex = QueueFamilyIgnored;
            imageMemoryBarrier.DestinationQueueFamilyIndex = QueueFamilyIgnored;
            imageMemoryBarrier.Image = offscreenRenderTarget;
            imageMemoryBarrier.ImageSubresourceRange = {
                .AspectMask = offscreenRenderTarget->GetImageAspect(),
                .BaseMipLevel = 0,
                .LevelCount = 1,
                .BaseArrayLayer = 0,
                .LayerCount = 1
            };
            pipelineBarrier.ImageMemoryBarriers.push_back(imageMemoryBarrier);

            RendererAPI::SetPipelineBarrier(commandBuffer, pipelineBarrier);
        }


        commandBuffer->EndRecording();

        CommandQueueHandle commandQueue = device.GetPrimaryCommandQueue();
        commandQueue->Submit(commandBuffer, renderTarget);
        commandQueue->Present(renderTarget);

        uint32 nextFrameIndex = (m_CurrentFrameIndex + 1) % 3;
        if (m_CurrentViewportTexture.size() == 0)
        {
            m_CurrentViewportTexture.push(m_FrameContexts[nextFrameIndex].OffscreenDescriptorSet);
        }
    }


    void DeferredRenderer::ResizeWindowImages(uint32 width, uint32 height)
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

            std::string windowFramebufferName = std::string("Window_Framebuffer_") + std::to_string(i);
            RendererAPI::NameObject(frameContext.WindowFramebuffer, windowFramebufferName);

            std::string swapchainRenderTarget = std::string("Swapchain_Render_Target_") + std::to_string(i);
            RendererAPI::NameObject(renderTargets[i]->GetAsTexture(), swapchainRenderTarget);
        }
    }


    void DeferredRenderer::SetVSync(bool isVSyncEnabled)
    {
        Device& device = RendererAPI::GetDevice();
        Swapchain& swapchain = device.GetSwapchain();
        device.WaitIdle();
        swapchain.RecreateSwapchain(isVSyncEnabled);

        Vec2 windowSize = Engine::Get().GetWindowManager().GetWindow().GetFramebufferDimensions();
        std::vector<RenderTargetHandle> renderTargets = swapchain.GetRenderTargets();

        for (int i = 0; i < swapchain.GetNumberOfImages(); i++)
        {
            FrameContext& frameContext = m_FrameContexts[i];
            frameContext.WindowFramebuffer = device.CreateFramebuffer(m_ImGuiRenderPass);
            FramebufferHandle framebuffer = frameContext.WindowFramebuffer;

            framebuffer->BeginBuildingFramebuffer(windowSize.x, windowSize.y);
            framebuffer->AttachRenderTarget(renderTargets[i]);
            framebuffer->EndBuildingFramebuffer();

            std::string windowFramebufferName = "Window_Framebuffer_" + std::to_string(i);
            RendererAPI::NameObject(frameContext.WindowFramebuffer, windowFramebufferName);
            std::string swapchainRenderTargetName = "Swapchain_Render_Target_" + std::to_string(i);
            RendererAPI::NameObject(renderTargets[i]->GetAsTexture(), swapchainRenderTargetName);
        }
    }


    void DeferredRenderer::DepthPrePass()
    {
        const RenderGraphPassExecutionContext& executionContext = m_RenderGraph.GetExecutionContext();
        FrameContext& frameContext = m_FrameContexts[m_CurrentFrameIndex];
        CommandBufferHandle commandBuffer = executionContext.CommandBuffer;
        AssetRegistry& registry = Engine::Get().GetAssetManager().GetRegistry();


        for (uint32 i = 0; i < frameContext.Meshes.size(); i++)
        {
            Mesh& mesh = frameContext.Meshes[i];
            Material material = frameContext.Materials[i];

            if (material.ShaderModel != ShaderModel::PBR) { continue; }

            if (material.DescriptorSet == nullptr) { continue; }
            material.DescriptorSet = nullptr;

            material.FragmentShader = registry.CreateAsset<Shader>("Shaders/DepthWriteOnly.frag");

            PipelineStateHandle pipeline = m_PipelineStateCache.GetGraphicsPipeline(executionContext.RenderPass, material, mesh, 0);
            pipeline->BindPipeline(commandBuffer);
            pipeline->SetViewportAndScissor(commandBuffer, m_ViewportSize);

            pipeline->BindDescriptorSet(commandBuffer, frameContext.SceneDataDescriptorSet, 0);

            mesh.VertexBuffer->Bind(commandBuffer);
            mesh.IndexBuffer->Bind(commandBuffer);

            RendererAPI::PushConstants(commandBuffer, pipeline, &frameContext.Transforms[i], sizeof(frameContext.Transforms[i]));

            RendererAPI::DrawElementsIndexed(commandBuffer, mesh.IndexBuffer);
        }

    }



    void DeferredRenderer::ForwardLightingPass()
    {
        struct ForwardLightingPassPushData
        {
            Mat4 ModelMatrix;
            uint32 HasNormalMap;
            uint32 HasDirectXNormals;
        };
        static_assert(sizeof(ForwardLightingPassPushData) <= MaxPushConstantRange, "Push constant can not be greater than MaxPushConstantRange (usually 128) bytes in size");


        const RenderGraphPassExecutionContext& executionContext = m_RenderGraph.GetExecutionContext();
        FrameContext& frameContext = m_FrameContexts[m_CurrentFrameIndex];
        CommandBufferHandle commandBuffer = executionContext.CommandBuffer;
        AssetRegistry& registry = Engine::Get().GetAssetManager().GetRegistry();

        m_PipelineStateCache.SetDescriptorSetStack({frameContext.SceneDataDescriptorSet, frameContext.EnvironmentMapDescriptorSet});

        for (uint32 i = 0; i < frameContext.Meshes.size(); i++)
        {
            Mesh& mesh = frameContext.Meshes[i];
            Material& material = frameContext.Materials[i];

            if (material.ShaderModel != ShaderModel::PBR) { continue; }

            if (material.DescriptorSet == nullptr) { continue; }

            if (material.TextureConvention == TextureConvention::UNPACKED)
            {
                material.FragmentShader = registry.CreateAsset<Shader>("Shaders/ForwardLightingPassUnpacked.frag");
            }
            else if (material.TextureConvention == TextureConvention::ORM_PACKED)
            {
                material.FragmentShader = registry.CreateAsset<Shader>("Shaders/ForwardLightingPassORM.frag");
            }

            Ref<Shader> vertexShader = mesh.VertexShader;

            PipelineStateHandle pipeline = m_PipelineStateCache.GetGraphicsPipeline(executionContext.RenderPass, material, mesh, 0);
            pipeline->BindPipeline(commandBuffer);
            pipeline->SetViewportAndScissor(commandBuffer, m_ViewportSize);

            pipeline->BindDescriptorSet(commandBuffer, frameContext.SceneDataDescriptorSet, 0);
            pipeline->BindDescriptorSet(commandBuffer, frameContext.EnvironmentMapDescriptorSet, 1);
            pipeline->BindDescriptorSet(commandBuffer, material.DescriptorSet, 2);

            mesh.VertexBuffer->Bind(commandBuffer);
            mesh.IndexBuffer->Bind(commandBuffer);

            ForwardLightingPassPushData pushConstantData = {
                .ModelMatrix = frameContext.Transforms[i],
                .HasNormalMap = material.HasNormalMap,
                .HasDirectXNormals = material.HasDirectXNormals
            };

            RendererAPI::PushConstants(commandBuffer, pipeline, &pushConstantData, sizeof(ForwardLightingPassPushData));

            RendererAPI::DrawElementsIndexed(commandBuffer, mesh.IndexBuffer);
        }

        m_PipelineStateCache.SetDescriptorSetStack({frameContext.SceneDataDescriptorSet});
    }




    void DeferredRenderer::GeometryPass()
    {
        struct GeometryPassPushData
        {
            Mat4 ModelMatrix;
            uint32 HasNormalMap;
            uint32 HasDirectXNormals;
        };
        static_assert(sizeof(GeometryPassPushData) <= MaxPushConstantRange, "Push constant can not be greater than MaxPushConstantRange (usually 128) bytes in size");


        const RenderGraphPassExecutionContext& executionContext = m_RenderGraph.GetExecutionContext();
        FrameContext& frameContext = m_FrameContexts[m_CurrentFrameIndex];
        CommandBufferHandle commandBuffer = executionContext.CommandBuffer;
        AssetRegistry& registry = Engine::Get().GetAssetManager().GetRegistry();

        for (uint32 i = 0; i < frameContext.Meshes.size(); i++)
        {
            Mesh& mesh = frameContext.Meshes[i];
            Material& material = frameContext.Materials[i];

            if (material.ShaderModel != ShaderModel::PBR) { continue; }

            DescriptorSetHandle& materialDescriptorSet = material.DescriptorSet;

            if (material.TextureConvention == TextureConvention::UNPACKED)
            {
                material.FragmentShader = registry.CreateAsset<Shader>("Shaders/DeferredGeometryPassUnpacked.frag");
            }
            else if (material.TextureConvention == TextureConvention::ORM_PACKED)
            {
                material.FragmentShader = registry.CreateAsset<Shader>("Shaders/DeferredGeometryPassORM.frag");
            }

            PipelineStateHandle pipeline = m_PipelineStateCache.GetGraphicsPipeline(executionContext.RenderPass, material, mesh, 0);
            pipeline->BindPipeline(commandBuffer);
            pipeline->SetViewportAndScissor(commandBuffer, m_ViewportSize);

            GeometryPassPushData pushConstantData = {
                .ModelMatrix = frameContext.Transforms[i],
                .HasNormalMap = material.HasNormalMap,
                .HasDirectXNormals = material.HasDirectXNormals
            };

            RendererAPI::PushConstants(commandBuffer, pipeline, &pushConstantData, sizeof(GeometryPassPushData));

            pipeline->BindDescriptorSet(commandBuffer, frameContext.SceneDataDescriptorSet, 0);
            pipeline->BindDescriptorSet(commandBuffer, materialDescriptorSet, 1);

            mesh.VertexBuffer->Bind(commandBuffer);
            mesh.IndexBuffer->Bind(commandBuffer);
            RendererAPI::DrawElementsIndexed(commandBuffer, mesh.IndexBuffer);
        }

    }


    void DeferredRenderer::DeferredLightingPass()
    {
        const RenderGraphPassExecutionContext& executionContext = m_RenderGraph.GetExecutionContext();
        FrameContext& frameContext = m_FrameContexts[m_CurrentFrameIndex];
        CommandBufferHandle commandBuffer = executionContext.CommandBuffer;
        AssetRegistry& registry = Engine::Get().GetAssetManager().GetRegistry();

        m_PipelineStateCache.SetDescriptorSetStack({frameContext.SceneDataDescriptorSet, frameContext.EnvironmentMapDescriptorSet});

        Mesh mesh = *registry.GetAsset<Mesh>("Meshes/Quad.obj"); 
        mesh.VertexShader = registry.CreateAsset<Shader>("Shaders/NoTransform.vert");
        frameContext.Meshes.push_back(mesh); // Hold onto reference so it is not destroyed early
        Material material{};
        material.FragmentShader = m_LightingShader;
        material.DescriptorSet = executionContext.ReadAttachments;

        Ref<Shader> vertexShader = mesh.VertexShader;

        PipelineStateHandle pipeline = m_PipelineStateCache.GetGraphicsPipeline(executionContext.RenderPass, material, mesh, 0);
        pipeline->BindPipeline(commandBuffer);
        pipeline->SetViewportAndScissor(commandBuffer, m_ViewportSize);

        pipeline->BindDescriptorSet(commandBuffer, frameContext.SceneDataDescriptorSet, 0);
        pipeline->BindDescriptorSet(commandBuffer, frameContext.EnvironmentMapDescriptorSet, 1);
        pipeline->BindDescriptorSet(commandBuffer, executionContext.ReadAttachments, 2);

        mesh.VertexBuffer->Bind(commandBuffer);
        mesh.IndexBuffer->Bind(commandBuffer);
        RendererAPI::DrawElementsIndexed(commandBuffer, mesh.IndexBuffer);

        m_PipelineStateCache.SetDescriptorSetStack({frameContext.SceneDataDescriptorSet});
    }


    void DeferredRenderer::EnvironmentMapPass()
    {
        const RenderGraphPassExecutionContext& executionContext = m_RenderGraph.GetExecutionContext();
        FrameContext& frameContext = m_FrameContexts[m_CurrentFrameIndex];
        CommandBufferHandle commandBuffer = executionContext.CommandBuffer;

        // Cubemap
        AssetRegistry& registry = Engine::Get().GetAssetManager().GetRegistry();
        Scene& activeScene = Engine::Get().GetSceneManager().GetActiveScene();

        Mesh& cubemapMesh = *registry.GetAsset<Mesh>("Meshes/Cube.obj");
        cubemapMesh.VertexShader = registry.CreateAsset<Shader>("Shaders/Cubemap.vert");
        frameContext.Meshes.push_back(cubemapMesh); // Hold onto reference so it is not destroyed early

        Material environmentMapMaterial{};
        environmentMapMaterial.FragmentShader = registry.CreateAsset<Shader>("Shaders/EnvironmentMap.frag");
        environmentMapMaterial.DescriptorSet = frameContext.EnvironmentMapDescriptorSet;

        PipelineStateHandle cubemapPipeline = m_PipelineStateCache.GetGraphicsPipeline(executionContext.RenderPass, environmentMapMaterial, cubemapMesh, 0);
        cubemapPipeline->BindPipeline(commandBuffer);
        cubemapPipeline->SetViewportAndScissor(commandBuffer, m_ViewportSize);

        cubemapPipeline->BindDescriptorSet(commandBuffer, frameContext.SceneDataDescriptorSet, 0);
        cubemapPipeline->BindDescriptorSet(commandBuffer, environmentMapMaterial.DescriptorSet, 1);

        cubemapMesh.VertexBuffer->Bind(commandBuffer);
        cubemapMesh.IndexBuffer->Bind(commandBuffer);

        RendererAPI::PushConstants(commandBuffer, cubemapPipeline, &activeScene.EnvironmentMapBlur, sizeof(activeScene.EnvironmentMapBlur));
        RendererAPI::DrawElementsIndexed(commandBuffer, cubemapMesh.IndexBuffer);
    }


    void DeferredRenderer::ToneMappingPass()
    {
        const RenderGraphPassExecutionContext& executionContext = m_RenderGraph.GetExecutionContext();
        FrameContext& frameContext = m_FrameContexts[m_CurrentFrameIndex];
        CommandBufferHandle commandBuffer = executionContext.CommandBuffer;

        AssetRegistry& registry = Engine::Get().GetAssetManager().GetRegistry();

        Mesh& quadMesh = *registry.GetAsset<Mesh>("Meshes/Quad.obj");
        quadMesh.VertexShader = registry.CreateAsset<Shader>("Shaders/NoTransform.vert");
        frameContext.Meshes.push_back(quadMesh); // Hold onto reference so it is not destroyed early

        m_PipelineStateCache.SetDescriptorSetStack({frameContext.SceneDataDescriptorSet, executionContext.ReadAttachments});

        Material toneMapperMaterial{};
        toneMapperMaterial.FragmentShader = registry.CreateAsset<Shader>("Shaders/ToneMapping.frag");
        toneMapperMaterial.DescriptorSet = m_ToneMappingLUTDescriptorSet;

        PipelineStateHandle toneMappingPipeline = m_PipelineStateCache.GetGraphicsPipeline(executionContext.RenderPass, toneMapperMaterial, quadMesh, 0);
        toneMappingPipeline->BindPipeline(commandBuffer);
        toneMappingPipeline->SetViewportAndScissor(commandBuffer, m_ViewportSize);

        toneMappingPipeline->BindDescriptorSet(commandBuffer, frameContext.SceneDataDescriptorSet, 0);
        toneMappingPipeline->BindDescriptorSet(commandBuffer, executionContext.ReadAttachments, 1);
        toneMappingPipeline->BindDescriptorSet(commandBuffer, toneMapperMaterial.DescriptorSet, 2);

        RendererAPI::PushConstants(commandBuffer, toneMappingPipeline, &m_SceneExposure, sizeof(m_SceneExposure));

        quadMesh.VertexBuffer->Bind(commandBuffer);
        quadMesh.IndexBuffer->Bind(commandBuffer);
        RendererAPI::DrawElementsIndexed(commandBuffer, quadMesh.IndexBuffer);

        m_PipelineStateCache.SetDescriptorSetStack(frameContext.SceneDataDescriptorSet);
    }


    void DeferredRenderer::FXAAPass()
    {
        const RenderGraphPassExecutionContext& executionContext = m_RenderGraph.GetExecutionContext();
        FrameContext& frameContext = m_FrameContexts[m_CurrentFrameIndex];
        CommandBufferHandle commandBuffer = executionContext.CommandBuffer;

        AssetRegistry& registry = Engine::Get().GetAssetManager().GetRegistry();

        Mesh& quadMesh = *registry.GetAsset<Mesh>("Meshes/Quad.obj");
        quadMesh.VertexShader = registry.CreateAsset<Shader>("Shaders/NoTransform.vert");
        frameContext.Meshes.push_back(quadMesh); // Hold onto reference so it is not destroyed early

        Material toneMapperMaterial{};
        toneMapperMaterial.FragmentShader = registry.CreateAsset<Shader>("Shaders/FXAAPass.frag");
        toneMapperMaterial.DescriptorSet = executionContext.ReadAttachments;

        PipelineStateHandle toneMappingPipeline = m_PipelineStateCache.GetGraphicsPipeline(executionContext.RenderPass, toneMapperMaterial, quadMesh, 0);
        toneMappingPipeline->BindPipeline(commandBuffer);
        toneMappingPipeline->SetViewportAndScissor(commandBuffer, m_ViewportSize);

        toneMappingPipeline->BindDescriptorSet(commandBuffer, frameContext.SceneDataDescriptorSet, 0);
        toneMappingPipeline->BindDescriptorSet(commandBuffer, executionContext.ReadAttachments, 1);

        quadMesh.VertexBuffer->Bind(commandBuffer);
        quadMesh.IndexBuffer->Bind(commandBuffer);
        RendererAPI::DrawElementsIndexed(commandBuffer, quadMesh.IndexBuffer);
    }


    struct ComputeIrradianceMapPushConstants
    {
        float FaceIndex;
    };
    static_assert(sizeof(ComputeIrradianceMapPushConstants) <= MaxPushConstantRange, "Push constant can not be greater than MaxPushConstantRange (usually 128) bytes in size");


    void DeferredRenderer::ComputeIrradianceMap(const CommandBufferHandle& commandBuffer)
    {
        RendererAPI::BeginLabel(commandBuffer, "IrradianceMapCalculation", Vec4(1.0f, 0.0f, 1.0f, 1.0f));

        FrameContext& frameContext = m_FrameContexts[m_CurrentFrameIndex];
        AssetRegistry& registry = Engine::Get().GetAssetManager().GetRegistry();

        m_PipelineStateCache.SetDescriptorSetStack(std::vector<DescriptorSetHandle>{});

        ShaderHandle irradianceCalcShader = registry.CreateAsset<Shader>("Shaders/ComputeIrradianceMap.comp");
        ;
        PipelineStateHandle computePipeline = m_PipelineStateCache.GetComputePipeline(irradianceCalcShader, m_EnvironmentMapStorageImagesSet);
        computePipeline->BindPipeline(commandBuffer);
        computePipeline->BindDescriptorSet(commandBuffer, m_EnvironmentMapStorageImagesSet, 0);


        ComputeIrradianceMapPushConstants computeIrradianceMapPushConstants;

        constexpr uint32 numFaces = 6;
        for (uint32 i = 0; i < numFaces; i++)
        {
            computeIrradianceMapPushConstants.FaceIndex = i;
            RendererAPI::PushConstants(commandBuffer, computePipeline, &computeIrradianceMapPushConstants, sizeof(computeIrradianceMapPushConstants));

            // Dispatching 2x2 blocks as the local layout is 8x8 and the irradiance faces are 16x16
            uint32 groupCountSize = EnvironmentMapIrradianceSize / 8;
            RendererAPI::Dispatch(commandBuffer, groupCountSize, groupCountSize, 1);
        }

        m_PipelineStateCache.SetDescriptorSetStack(frameContext.SceneDataDescriptorSet);

        RendererAPI::EndLabel(commandBuffer);
    }


    struct ComputePrefilteredEnvironmentMapPushData
    {
        uint32 FaceIndex;
        float Roughness;
    };


    void DeferredRenderer::ComputePrefilteredEnvironmentMap(const CommandBufferHandle& commandBuffer, uint32 mipLevel, UVec2 mipDimensions)
    {
        RendererAPI::BeginLabel(commandBuffer, "PrefilteredEnvironmentMapCalc", Vec4(1.0f, 0.0f, 1.0f, 1.0f));

        FrameContext& frameContext = m_FrameContexts[m_CurrentFrameIndex];
        AssetRegistry& registry = Engine::Get().GetAssetManager().GetRegistry();

        m_PipelineStateCache.SetDescriptorSetStack(std::vector<DescriptorSetHandle>{});

        ShaderHandle prefilterCalcShader = registry.CreateAsset<Shader>("Shaders/ComputePrefilteredEnvironmentMap.comp");

        PipelineStateHandle computePipeline = m_PipelineStateCache.GetComputePipeline(prefilterCalcShader, m_EnvironmentMapStorageImagesSet);
        computePipeline->BindPipeline(commandBuffer);
        computePipeline->BindDescriptorSet(commandBuffer, m_EnvironmentMapStorageImagesSet, 0);

        uint32 mipWidth = mipDimensions.x;
        uint32 mipHeight = mipDimensions.y;

        float totalMipLevels = frameContext.EnvironmentMap->Environment->GetNumMipLevels();
        float roughness = mipLevel / totalMipLevels;

        ComputePrefilteredEnvironmentMapPushData prefilteredEnvironmentMapPushData;
        prefilteredEnvironmentMapPushData.Roughness = roughness;

        constexpr uint32 numFaces = 6;
        for (uint32 i = 0; i < numFaces; i++)
        {
            prefilteredEnvironmentMapPushData.FaceIndex = i;

            RendererAPI::PushConstants(commandBuffer, computePipeline, &prefilteredEnvironmentMapPushData, sizeof(prefilteredEnvironmentMapPushData));

            uint32 groupSizeX = std::max(mipWidth / 32, 1u);
            uint32 groupSizeY = std::max(mipHeight / 32, 1u);
            RendererAPI::Dispatch(commandBuffer, groupSizeX, groupSizeY, 1);
        }

        m_PipelineStateCache.SetDescriptorSetStack(frameContext.SceneDataDescriptorSet);

        RendererAPI::EndLabel(commandBuffer);
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
                .UsageFlags = IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
                .Dimensions = UVec2(width, height),
                .ImageData = nullptr,
                .LayerCount = 1,
                .MipMapCount = 1,
            };

            frameContext.OffscreenRenderTarget = device.CreateTexture(textureCreateInfo);
            std::string offscreenRenderTargetName = std::string("Offscreen_Render_Target_") + std::to_string(i);
            RendererAPI::NameObject(frameContext.OffscreenRenderTarget, offscreenRenderTargetName);

            frameContext.OffscreenDescriptorSet->UpdateImageSamplerBinding(0, frameContext.OffscreenRenderTarget);
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