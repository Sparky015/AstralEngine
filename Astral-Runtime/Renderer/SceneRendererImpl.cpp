/**
* @file SceneRendererImpl.cpp
* @author Andrew Fagan
* @date 7/1/2025
*/


#include "SceneRendererImpl.h"

#include "Asset/AssetManager.h"
#include "Core/Engine.h"
#include "Common/CubeLUT.h"
#include "Debug/ImGui/ImGuiManager.h"
#include "RHI/RendererAPI.h"
#include "RHI/Resources/CommandBuffer.h"
#include "RHI/Resources/RenderTarget.h"
#include "RHI/Resources/Shader.h"
#include "Renderer/RendererManager.h"
#include "Scenes/SceneManager.h"
#include "Window/WindowManager.h"

#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/gtc/type_ptr.hpp>

#include <future>


namespace Astral {

    void SceneRendererImpl::Init()
    {
        PROFILE_SCOPE("SceneRenderer::Init")

        m_WindowResizedListener = EventListener<FramebufferResizedEvent>{[this](FramebufferResizedEvent event) { ResizeWindowImages(event.Width, event.Height); }};
        m_WindowResizedListener.StartListening();

        m_ViewportSize = RendererAPI::GetContext().GetFramebufferSize();
        m_CurrentFrameIndex = 0;





        // Building the imgui render pass
        BuildImGuiEditorRenderPass();


        // Initializing the resources that are allocated per swapchain image
        InitializeFrameResources();

        // Renderer Settings
        RendererSettings rendererSettings{};
        rendererSettings.RendererType = RendererType::DEFERRED;
        rendererSettings.IsVSyncEnabled = true;
        rendererSettings.IsFrustumCullingEnabled = true;
        rendererSettings.IsShadowsOn = true;
        rendererSettings.NumShadowCascades = 3;
        rendererSettings.ShadowMapResolution = 4096;
        rendererSettings.ShadowMapBias = .02;
        rendererSettings.ShadowMapZMultiplier = 1;

        SetRendererSettings(rendererSettings);


        m_PipelineStateCache.SetDescriptorSetStack(m_FrameContexts[0].SceneDataDescriptorSet);
        m_CurrentViewportTexture.push(m_FrameContexts[1].OffscreenDescriptorSet);

        Engine::Get().GetRendererManager().GetContext().InitImGuiForAPIBackend(m_ImGuiRenderPass);
        AssetRegistry& registry = Engine::Get().GetAssetManager().GetRegistry();
        m_DeferredGeometryPassUnpackedShader = registry.GetOrCreateAsset<Shader>("Shaders/DeferredGeometryPassUnpacked.frag");
        m_DeferredGeometryPassORMShader = registry.GetOrCreateAsset<Shader>("Shaders/DeferredGeometryPassORM.frag");
        m_DeferredLightingShader = registry.GetOrCreateAsset<Shader>("Shaders/DeferredLightingPass.frag");
        m_ForwardUnpackedLightingShader = registry.GetOrCreateAsset<Shader>("Shaders/ForwardLightingPassUnpacked.frag");
        m_ForwardORMLightingShader = registry.GetOrCreateAsset<Shader>("Shaders/ForwardLightingPassORM.frag");
        m_DepthWriteOnlyShader = registry.GetOrCreateAsset<Shader>("Shaders/DepthWriteOnly.frag");

        Ref<CubeLUT> toneMappingLUT = registry.GetOrCreateAsset<CubeLUT>("LUTs/ACEScg_to_sRGB_RRT_ODT.cube");
        m_RTT_ODT_LUT_DescriptorSet = RendererAPI::GetDevice().CreateDescriptorSet();
        m_RTT_ODT_LUT_DescriptorSet->BeginBuildingSet();
        m_RTT_ODT_LUT_DescriptorSet->AddDescriptorImageSampler(toneMappingLUT->LUT3D, ShaderStage::FRAGMENT);
        m_RTT_ODT_LUT_DescriptorSet->AddDescriptorImageSampler(toneMappingLUT->Shaper1D, ShaderStage::FRAGMENT);
        m_RTT_ODT_LUT_DescriptorSet->EndBuildingSet();
    }


    void SceneRendererImpl::Shutdown()
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

    void SceneRendererImpl::BeginScene(const SceneDescription& sceneDescription)
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
            .CameraViewProjection = sceneDescription.Camera.GetViewProjectionMatrix(),
            .CameraView = sceneDescription.Camera.GetViewMatrix(),
            .CameraProjection = sceneDescription.Camera.GetProjectionMatrix(),
            .CameraInverseViewMat = glm::inverse(sceneDescription.Camera.GetViewMatrix()),
            .CameraInverseProjectionMat = glm::inverse(sceneDescription.Camera.GetProjectionMatrix()),
            .ScreenSize = m_ViewportSize,
            .CameraPosition = sceneDescription.Camera.GetPosition(),
            .NumLights = (uint32)sceneDescription.Lights.size(),
            .AmbientLightConstant = sceneDescription.AmbientLightConstant,
            .NumShadowCascades = 3,
        };

        m_SceneCamera = sceneDescription.Camera;

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

                frameContext.IsEnvironmentMapIBLCalculationNeeded = true;
            }

            frameContext.EnvironmentMapDescriptorSet->UpdateImageSamplerBinding(1, sceneDescription.EnvironmentMap->Irradiance, ImageLayout::GENERAL);
        }


        for (Light& light : sceneDescription.Lights)
        {
            if (light.LightType == LightType::DIRECTIONAL)
            {
                m_FirstDirectionalLightInScene = light;
            }
        }




        m_SceneExposure = sceneDescription.Exposure;
        m_SceneViewProjection = sceneDescription.Camera.GetViewProjectionMatrix();


        frameContext.Meshes.clear();
        frameContext.Materials.clear();
        frameContext.Transforms.clear();
    }


    void SceneRendererImpl::EndScene()
    {
        {
            PROFILE_SCOPE("SceneRenderer::EndScene")
            m_IsSceneStarted = false;
        }

        RenderScene();
    }


    void SceneRendererImpl::Submit(Mesh& mesh, Material& material, Mat4& transform)
    {
        ASSERT(m_IsSceneStarted, "Scene has not been started! Use SceneRenderer::BeginScene")
        FrameContext& frameContext = m_FrameContexts[m_CurrentFrameIndex];

        if (m_RendererSettings.IsFrustumCullingEnabled)
        {
            if (ShouldCullMesh(mesh, transform)) { return; }
        }

        frameContext.Meshes.push_back(mesh);
        frameContext.Materials.push_back(material);
        frameContext.Transforms.push_back(transform);
    }


    void SceneRendererImpl::SetRendererSettings(const RendererSettings& rendererSettings)
    {
        bool isRenderGraphRebuildNeeded = false;

        if (m_RendererSettings.RendererType != rendererSettings.RendererType)
        {
            m_RendererSettings.RendererType = rendererSettings.RendererType;
            isRenderGraphRebuildNeeded = true;
        }

        if (m_RendererSettings.IsVSyncEnabled != rendererSettings.IsVSyncEnabled)
        {
            m_RendererSettings.IsVSyncEnabled = rendererSettings.IsVSyncEnabled;
            SetVSync(m_RendererSettings.IsVSyncEnabled);
        }

        if (m_RendererSettings.DebugView != rendererSettings.DebugView)
        {
            m_RendererSettings.DebugView = rendererSettings.DebugView;

            if (m_RendererSettings.RendererType == RendererType::DEFERRED)
            {
                isRenderGraphRebuildNeeded = true;
            }
        }

        if (m_RendererSettings.NumShadowCascades != rendererSettings.NumShadowCascades)
        {
            m_RendererSettings.NumShadowCascades = rendererSettings.NumShadowCascades;
            isRenderGraphRebuildNeeded = true;
        }

        if (m_RendererSettings.ShadowMapResolution != rendererSettings.ShadowMapResolution)
        {
            m_RendererSettings.ShadowMapResolution = rendererSettings.ShadowMapResolution;
            isRenderGraphRebuildNeeded = true;
        }

        m_RendererSettings.IsFrustumCullingEnabled = rendererSettings.IsFrustumCullingEnabled;
        m_RendererSettings.IsShadowsOn = rendererSettings.IsShadowsOn;
        m_RendererSettings.ShadowMapBias = rendererSettings.ShadowMapBias;
        m_RendererSettings.ShadowMapZMultiplier = rendererSettings.ShadowMapZMultiplier;

        if (isRenderGraphRebuildNeeded)
        {
            if (m_RendererSettings.RendererType == RendererType::DEFERRED)
            {
                BuildRenderGraphForDeferred();
            }
            else if (m_RendererSettings.RendererType == RendererType::FORWARD)
            {
                BuildRenderGraphForForward();
            }
        }
    }


    const RendererSettings& SceneRendererImpl::GetRendererSettings()
    {
        return m_RendererSettings;
    }


    DescriptorSetHandle SceneRendererImpl::GetViewportTexture()
    {
        DescriptorSetHandle& descriptorSet = m_CurrentViewportTexture.front();
        m_CurrentViewportTexture.pop();
        return descriptorSet;
    }


    void SceneRendererImpl::BuildRenderGraphForDeferred()
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


        AttachmentDescription lightDepthBufferDescription = {
            .Format = ImageFormat::D32_SFLOAT_S8_UINT,
            .ImageUsageFlags = IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
            .LoadOp = AttachmentLoadOp::CLEAR,
            .StoreOp = AttachmentStoreOp::STORE,
            .InitialLayout = ImageLayout::DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
            .FinalLayout = ImageLayout::DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
            .ClearColor = Vec4(1.0, 0.0, 0.0, 1.0),
            .LayerCount = (uint32)m_RendererSettings.NumShadowCascades,
            .TextureType = TextureType::IMAGE_2D_ARRAY
        };

        RenderGraphPass shadowMapPass = RenderGraphPass(Vec3(m_RendererSettings.ShadowMapResolution, m_RendererSettings.ShadowMapResolution, m_RendererSettings.NumShadowCascades), "Shadow Map Pass", [&](){ CascadedShadowMapsPass(); });
        shadowMapPass.CreateDepthStencilAttachment(lightDepthBufferDescription, "Light_Depth_Buffer", ImageLayout::DEPTH_STENCIL_ATTACHMENT_OPTIMAL);


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
        lightingPass.LinkReadInputAttachment(&shadowMapPass, "Light_Depth_Buffer", ImageLayout::SHADER_READ_ONLY_OPTIMAL);

        lightingPass.CreateColorAttachment(lightingTextureDescription, "Deferred_Lighting_Buffer", ImageLayout::COLOR_ATTACHMENT_OPTIMAL);
        lightingPass.AddDependency(&shadowMapPass);


        RenderGraphPass environmentMapPass = RenderGraphPass(OutputAttachmentDimensions, "Environment Map Pass", [&](){ EnvironmentMapPass(); });
        environmentMapPass.LinkWriteInputAttachment(&lightingPass, "Deferred_Lighting_Buffer", ImageLayout::COLOR_ATTACHMENT_OPTIMAL);
        environmentMapPass.LinkWriteInputAttachment(&geometryPass, "GBuffer_Depth_Buffer", ImageLayout::DEPTH_STENCIL_ATTACHMENT_OPTIMAL);


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
        tonemappingPass.AddDependency(&environmentMapPass);

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
        m_RenderGraph.AddPass(shadowMapPass);
        m_RenderGraph.AddPass(lightingPass);
        m_RenderGraph.AddPass(environmentMapPass);
        m_RenderGraph.AddPass(tonemappingPass);
        m_RenderGraph.AddOutputPass(fxaaPass);

        switch (m_RendererSettings.DebugView)
        {
            case RendererDebugView::NONE: m_RenderGraph.SetOutputAttachment(fxaaPass, "FXAA_Output_Buffer", outputTextures); break;
            case RendererDebugView::GBUFFER_ALBEDO: m_RenderGraph.SetOutputAttachment(geometryPass, "GBuffer_Albedo", outputTextures); break;
            case RendererDebugView::GBUFFER_ROUGHNESS: m_RenderGraph.SetOutputAttachment(geometryPass, "GBuffer_Roughness", outputTextures); break;
            case RendererDebugView::GBUFFER_METALLIC: m_RenderGraph.SetOutputAttachment(geometryPass, "GBuffer_Metallic", outputTextures); break;
            case RendererDebugView::GBUFFER_EMISSION: m_RenderGraph.SetOutputAttachment(geometryPass, "GBuffer_Emission", outputTextures); break;
            case RendererDebugView::GBUFFER_NORMAL: m_RenderGraph.SetOutputAttachment(geometryPass, "GBuffer_Normals", outputTextures); break;
            case RendererDebugView::DEPTH: m_RenderGraph.SetOutputAttachment(geometryPass, "GBuffer_Depth_Buffer", outputTextures); break;
            default: m_RenderGraph.SetOutputAttachment(fxaaPass, "FXAA_Output_Buffer", outputTextures); break;
        }

        m_RenderGraph.EndBuildingRenderGraph();
    }

    static constexpr SampleCount ForwardMSAASampleCount = SampleCount::SAMPLE_4_BIT;

    void SceneRendererImpl::BuildRenderGraphForForward()
    {

        AttachmentDescription depthMSAABufferDescription = {
            .Format = ImageFormat::D32_SFLOAT_S8_UINT,
            .ImageUsageFlags = IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
            .LoadOp = AttachmentLoadOp::CLEAR,
            .StoreOp = AttachmentStoreOp::STORE,
            .InitialLayout = ImageLayout::DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
            .FinalLayout = ImageLayout::DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
            .ClearColor = Vec4(1.0, 0.0, 0.0, 1.0),
            .MSAASamples = ForwardMSAASampleCount
        };

        RenderGraphPass depthPrePass = RenderGraphPass(OutputAttachmentDimensions, "Depth Pre-Pass", [&](){ DepthPrePass(); });
        depthPrePass.CreateDepthStencilAttachment(depthMSAABufferDescription, "Forward_Depth_MSSA_Buffer", ImageLayout::DEPTH_STENCIL_ATTACHMENT_OPTIMAL);


        AttachmentDescription shadowMapBufferDescription = {
            .Format = ImageFormat::D32_SFLOAT_S8_UINT,
            .ImageUsageFlags = IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
            .LoadOp = AttachmentLoadOp::CLEAR,
            .StoreOp = AttachmentStoreOp::STORE,
            .InitialLayout = ImageLayout::DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
            .FinalLayout = ImageLayout::DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
            .ClearColor = Vec4(1.0, 0.0, 0.0, 1.0),
            .LayerCount = (uint32)m_RendererSettings.NumShadowCascades,
            .TextureType = TextureType::IMAGE_2D_ARRAY
        };

        RenderGraphPass shadowMapPass = RenderGraphPass(Vec3(m_RendererSettings.ShadowMapResolution, m_RendererSettings.ShadowMapResolution, m_RendererSettings.NumShadowCascades), "Shadow Map Pass", [&](){ CascadedShadowMapsPass(); });
        shadowMapPass.CreateDepthStencilAttachment(shadowMapBufferDescription, "Shadow_Map_Buffer", ImageLayout::DEPTH_STENCIL_ATTACHMENT_OPTIMAL);


        AttachmentDescription lightingMSAATextureDescription = {
            .Format = ImageFormat::R16G16B16A16_SFLOAT,
            .ImageUsageFlags = IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
            .LoadOp = AttachmentLoadOp::CLEAR,
            .StoreOp = AttachmentStoreOp::STORE,
            .InitialLayout = ImageLayout::SHADER_READ_ONLY_OPTIMAL,
            .FinalLayout = ImageLayout::SHADER_READ_ONLY_OPTIMAL,
            .ClearColor = Vec4(0.0, 0.0, 0.0, 1.0),
            .MSAASamples = ForwardMSAASampleCount,
        };


        RenderGraphPass lightingPass = RenderGraphPass(OutputAttachmentDimensions, "Lighting Pass", [&](){ ForwardLightingPass(); });
        lightingPass.LinkReadInputAttachment(&shadowMapPass, "Shadow_Map_Buffer", ImageLayout::SHADER_READ_ONLY_OPTIMAL);
        lightingPass.CreateColorAttachment(lightingMSAATextureDescription, "Forward_Lighting_MSAA_Buffer", ImageLayout::COLOR_ATTACHMENT_OPTIMAL);
        lightingPass.LinkWriteInputAttachment(&depthPrePass, "Forward_Depth_MSSA_Buffer", ImageLayout::DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
        lightingPass.AddDependency(&shadowMapPass);

        AttachmentDescription lightingResolveTextureDescription = {
            .Format = ImageFormat::R16G16B16A16_SFLOAT,
            .ImageUsageFlags = IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
            .LoadOp = AttachmentLoadOp::DONT_CARE,
            .StoreOp = AttachmentStoreOp::STORE,
            .InitialLayout = ImageLayout::SHADER_READ_ONLY_OPTIMAL,
            .FinalLayout = ImageLayout::SHADER_READ_ONLY_OPTIMAL,
            .ClearColor = Vec4(0.0, 0.0, 0.0, 1.0),
        };

        RenderGraphPass environmentMapPass = RenderGraphPass(OutputAttachmentDimensions, "Environment Map Pass", [&](){ MSAAEnvironmentPass(); });
        environmentMapPass.LinkWriteInputAttachment(&lightingPass, "Forward_Lighting_MSAA_Buffer", ImageLayout::COLOR_ATTACHMENT_OPTIMAL);
        environmentMapPass.CreateResolveAttachment(lightingResolveTextureDescription, "Forward_Lighting_Buffer", ImageLayout::COLOR_ATTACHMENT_OPTIMAL);
        environmentMapPass.LinkWriteInputAttachment(&depthPrePass, "Forward_Depth_MSSA_Buffer", ImageLayout::DEPTH_STENCIL_ATTACHMENT_OPTIMAL);



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
        tonemappingPass.LinkReadInputAttachment(&environmentMapPass, "Forward_Lighting_Buffer", ImageLayout::SHADER_READ_ONLY_OPTIMAL);
        tonemappingPass.CreateColorAttachment(toneMappingOutputTextureDescription, "Tonemapping_Output_Buffer", ImageLayout::COLOR_ATTACHMENT_OPTIMAL);
        tonemappingPass.AddDependency(&environmentMapPass);


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
        m_RenderGraph.AddPass(shadowMapPass);
        m_RenderGraph.AddPass(lightingPass);
        m_RenderGraph.AddPass(environmentMapPass);
        m_RenderGraph.AddOutputPass(tonemappingPass);
        m_RenderGraph.SetOutputAttachment(tonemappingPass, "Tonemapping_Output_Buffer", outputTextures);
        m_RenderGraph.EndBuildingRenderGraph();
    }


    void SceneRendererImpl::BuildImGuiEditorRenderPass()
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



    void SceneRendererImpl::InitializeFrameResources()
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


            Ref<EnvironmentMap> environmentMap = registry.GetOrCreateAsset<EnvironmentMap>("Cubemaps/pretoria_gardens_4k.hdr");
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


            TextureHandle brdfLut = registry.GetOrCreateAsset<Texture>("LUTs/ibl_brdf_lut.dds");

            context.EnvironmentMapDescriptorSet = device.CreateDescriptorSet();
            context.EnvironmentMapDescriptorSet->BeginBuildingSet();
            context.EnvironmentMapDescriptorSet->AddDescriptorImageSampler(environmentMap->PrefilteredEnvironment, ShaderStage::FRAGMENT, ImageLayout::GENERAL);
            context.EnvironmentMapDescriptorSet->AddDescriptorImageSampler(environmentMap->Irradiance, ShaderStage::FRAGMENT, ImageLayout::GENERAL);
            context.EnvironmentMapDescriptorSet->AddDescriptorImageSampler(brdfLut, ShaderStage::FRAGMENT);
            context.EnvironmentMapDescriptorSet->EndBuildingSet();
            std::string environmentMapDescriptorSetName = std::string("Environment_Map_Descriptor_Set_") + std::to_string(i);
            RendererAPI::NameObject(context.EnvironmentMapDescriptorSet, environmentMapDescriptorSetName);

            context.IsEnvironmentMapIBLCalculationNeeded = true;

            context.ShadowLightMatrices = device.CreateUniformBuffer(nullptr, sizeof(Mat4) * 8);
            context.ShadowLightMatricesDescriptorSet = device.CreateDescriptorSet();
            context.ShadowLightMatricesDescriptorSet->BeginBuildingSet();
            context.ShadowLightMatricesDescriptorSet->AddDescriptorUniformBuffer(context.ShadowLightMatrices, ShaderStage::ALL);
            context.ShadowLightMatricesDescriptorSet->EndBuildingSet();
        }


        FrameContext& context = m_FrameContexts[0];
        m_EnvironmentMapStorageImagesSet = device.CreateDescriptorSet();
        m_EnvironmentMapStorageImagesSet->BeginBuildingSet();
        m_EnvironmentMapStorageImagesSet->AddDescriptorImageSampler(context.EnvironmentMap->Environment, ShaderStage::COMPUTE);
        m_EnvironmentMapStorageImagesSet->AddDescriptorStorageImage(context.EnvironmentMap->Irradiance, ShaderStage::COMPUTE, ImageLayout::GENERAL);
        m_EnvironmentMapStorageImagesSet->AddDescriptorStorageImage(context.EnvironmentMap->PrefilteredEnvironment, ShaderStage::COMPUTE, ImageLayout::GENERAL);
        m_EnvironmentMapStorageImagesSet->EndBuildingSet();
    }


    void SceneRendererImpl::RenderScene()
    {
        PROFILE_SCOPE("SceneRenderer::RenderScene")

        // TODO: Sort the meshes by material
        Device& device = RendererAPI::GetDevice();

        FrameContext& frameContext = m_FrameContexts[m_CurrentFrameIndex];
        RenderTargetHandle renderTarget = frameContext.SceneRenderTarget;
        CommandBufferHandle commandBuffer = frameContext.SceneCommandBuffer;


        commandBuffer->BeginRecording();

        if (frameContext.IsEnvironmentMapIBLCalculationNeeded)
        {
            ComputeEnvironmentIBL();
        }

        // Viewport Rendering
        m_RenderGraph.Execute(commandBuffer, m_CurrentFrameIndex);

        // Editor UI rendering to swapchain image
        DrawEditorUI(commandBuffer, renderTarget);

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


    void SceneRendererImpl::ResizeWindowImages(uint32 width, uint32 height)
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


    void SceneRendererImpl::SetVSync(bool isVSyncEnabled)
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


    void SceneRendererImpl::DepthPrePass()
    {
        PROFILE_SCOPE("DepthPrePass")

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

            material.FragmentShader = m_DepthWriteOnlyShader;

            PipelineStateHandle pipeline = m_PipelineStateCache.GetGraphicsPipeline(executionContext.RenderPass, material, mesh, 0, CullMode::NONE, SampleCount::SAMPLE_4_BIT);
            commandBuffer->BindPipeline(pipeline);
            commandBuffer->SetViewportAndScissor(m_ViewportSize);

            commandBuffer->BindDescriptorSet(frameContext.SceneDataDescriptorSet, 0);

            commandBuffer->BindVertexBuffer(mesh.VertexBuffer);
            commandBuffer->BindIndexBuffer(mesh.IndexBuffer);

            commandBuffer->PushConstants(&frameContext.Transforms[i], sizeof(frameContext.Transforms[i]));

            commandBuffer->DrawElementsIndexed(mesh.IndexBuffer);
        }

    }



    void SceneRendererImpl::ForwardLightingPass()
    {
        PROFILE_SCOPE("ForwardLightingPass")

        struct ForwardLightingPassPushData
        {
            Mat4 ModelMatrix;
            uint32 HasNormalMap;
            uint32 HasDirectXNormals;
            float CameraZNear;
            float CameraZFar;
            int32 NumShadowCascades;
            uint32 ShowCascadeDebugView;
            float ShadowMapBias;
        };
        static_assert(sizeof(ForwardLightingPassPushData) <= MaxPushConstantRange, "Push constant can not be greater than MaxPushConstantRange (usually 128) bytes in size");


        const RenderGraphPassExecutionContext& executionContext = m_RenderGraph.GetExecutionContext();
        FrameContext& frameContext = m_FrameContexts[m_CurrentFrameIndex];
        CommandBufferHandle commandBuffer = executionContext.CommandBuffer;
        AssetRegistry& registry = Engine::Get().GetAssetManager().GetRegistry();

        m_PipelineStateCache.SetDescriptorSetStack({frameContext.SceneDataDescriptorSet, frameContext.EnvironmentMapDescriptorSet, executionContext.ReadAttachments, frameContext.ShadowLightMatricesDescriptorSet});


        for (uint32 i = 0; i < frameContext.Meshes.size(); i++)
        {
            Mesh& mesh = frameContext.Meshes[i];
            Material& material = frameContext.Materials[i];

            if (material.ShaderModel != ShaderModel::PBR) { continue; }

            if (material.DescriptorSet == nullptr) { continue; }

            if (material.TextureConvention == TextureConvention::UNPACKED)
            {
                material.FragmentShader = m_ForwardUnpackedLightingShader;
            }
            else if (material.TextureConvention == TextureConvention::ORM_PACKED)
            {
                material.FragmentShader = m_ForwardORMLightingShader;
            }

            Ref<Shader> vertexShader = mesh.VertexShader;

            PipelineStateHandle pipeline = m_PipelineStateCache.GetGraphicsPipeline(executionContext.RenderPass, material, mesh, 0, CullMode::NONE, SampleCount::SAMPLE_4_BIT);
            commandBuffer->BindPipeline(pipeline);
            commandBuffer->SetViewportAndScissor(m_ViewportSize);

            commandBuffer->BindDescriptorSet(frameContext.SceneDataDescriptorSet, 0);
            commandBuffer->BindDescriptorSet(frameContext.EnvironmentMapDescriptorSet, 1);
            commandBuffer->BindDescriptorSet(executionContext.ReadAttachments, 2);
            commandBuffer->BindDescriptorSet(frameContext.ShadowLightMatricesDescriptorSet, 3);
            commandBuffer->BindDescriptorSet(material.DescriptorSet, 4);

            commandBuffer->BindVertexBuffer(mesh.VertexBuffer);
            commandBuffer->BindIndexBuffer(mesh.IndexBuffer);


            ForwardLightingPassPushData pushConstantData = {
                .ModelMatrix = frameContext.Transforms[i],
                .HasNormalMap = material.HasNormalMap,
                .HasDirectXNormals = material.HasDirectXNormals,
                .CameraZNear = m_SceneCamera.GetNearPlane(),
                .CameraZFar = m_SceneCamera.GetFarPlane(),
                .NumShadowCascades = m_RendererSettings.NumShadowCascades,
                .ShowCascadeDebugView = m_RendererSettings.DebugView == RendererDebugView::CASCADED_SHADOW_MAP_BOUNDARIES,
                .ShadowMapBias = m_RendererSettings.ShadowMapBias,
            };

            commandBuffer->PushConstants(&pushConstantData, sizeof(ForwardLightingPassPushData));

            commandBuffer->DrawElementsIndexed(mesh.IndexBuffer);
        }

        m_PipelineStateCache.SetDescriptorSetStack({frameContext.SceneDataDescriptorSet});
    }


    void SceneRendererImpl::MSAAEnvironmentPass()
    {
        PROFILE_SCOPE("MSAAEnvironmentPass")

        const RenderGraphPassExecutionContext& executionContext = m_RenderGraph.GetExecutionContext();
        FrameContext& frameContext = m_FrameContexts[m_CurrentFrameIndex];
        CommandBufferHandle commandBuffer = executionContext.CommandBuffer;

        // Cubemap
        AssetRegistry& registry = Engine::Get().GetAssetManager().GetRegistry();
        Scene& activeScene = Engine::Get().GetSceneManager().GetActiveScene();

        Mesh& cubemapMesh = *registry.GetAsset<Mesh>("Meshes/Cube.obj");
        cubemapMesh.VertexShader = registry.GetOrCreateAsset<Shader>("Shaders/Cubemap.vert");
        frameContext.Meshes.push_back(cubemapMesh); // Hold onto reference so it is not destroyed early

        Material environmentMapMaterial{};
        environmentMapMaterial.FragmentShader = registry.GetOrCreateAsset<Shader>("Shaders/EnvironmentMap.frag");
        environmentMapMaterial.DescriptorSet = frameContext.EnvironmentMapDescriptorSet;

        PipelineStateHandle cubemapPipeline = m_PipelineStateCache.GetGraphicsPipeline(executionContext.RenderPass, environmentMapMaterial, cubemapMesh, 0, CullMode::NONE, ForwardMSAASampleCount);
        commandBuffer->BindPipeline(cubemapPipeline);
        commandBuffer->SetViewportAndScissor(m_ViewportSize);

        commandBuffer->BindDescriptorSet(frameContext.SceneDataDescriptorSet, 0);
        commandBuffer->BindDescriptorSet(environmentMapMaterial.DescriptorSet, 1);

        commandBuffer->BindVertexBuffer(cubemapMesh.VertexBuffer);
        commandBuffer->BindIndexBuffer(cubemapMesh.IndexBuffer);

        commandBuffer->PushConstants(&activeScene.EnvironmentMapBlur, sizeof(activeScene.EnvironmentMapBlur));
        commandBuffer->DrawElementsIndexed(cubemapMesh.IndexBuffer);
    }


    void SceneRendererImpl::GeometryPass()
    {
        PROFILE_SCOPE("GeometryPass")

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
                material.FragmentShader = m_DeferredGeometryPassUnpackedShader;
            }
            else if (material.TextureConvention == TextureConvention::ORM_PACKED)
            {
                material.FragmentShader = m_DeferredGeometryPassORMShader;
            }

            PipelineStateHandle pipeline = m_PipelineStateCache.GetGraphicsPipeline(executionContext.RenderPass, material, mesh, 0, CullMode::NONE);
            commandBuffer->BindPipeline(pipeline);
            commandBuffer->SetViewportAndScissor(m_ViewportSize);

            GeometryPassPushData pushConstantData = {
                .ModelMatrix = frameContext.Transforms[i],
                .HasNormalMap = material.HasNormalMap,
                .HasDirectXNormals = material.HasDirectXNormals
            };

            commandBuffer->PushConstants(&pushConstantData, sizeof(GeometryPassPushData));

            commandBuffer->BindDescriptorSet(frameContext.SceneDataDescriptorSet, 0);
            commandBuffer->BindDescriptorSet(materialDescriptorSet, 1);

            commandBuffer->BindVertexBuffer(mesh.VertexBuffer);
            commandBuffer->BindIndexBuffer(mesh.IndexBuffer);

            commandBuffer->DrawElementsIndexed(mesh.IndexBuffer);
        }

    }


    void SceneRendererImpl::DeferredLightingPass()
    {
        PROFILE_SCOPE("DeferredLightingPass")

        struct DeferredLightingPushConstants
        {
            float CameraZNear;
            float CameraZFar;
            int32 NumShadowCascades;
            uint32 ShowCascadeDebugView;
            float ShadowMapBias;
        };

        const RenderGraphPassExecutionContext& executionContext = m_RenderGraph.GetExecutionContext();
        FrameContext& frameContext = m_FrameContexts[m_CurrentFrameIndex];
        CommandBufferHandle commandBuffer = executionContext.CommandBuffer;
        AssetRegistry& registry = Engine::Get().GetAssetManager().GetRegistry();

        m_PipelineStateCache.SetDescriptorSetStack({frameContext.SceneDataDescriptorSet, frameContext.EnvironmentMapDescriptorSet, frameContext.ShadowLightMatricesDescriptorSet});

        Mesh mesh = *registry.GetAsset<Mesh>("Meshes/Quad.obj"); 
        mesh.VertexShader = registry.GetOrCreateAsset<Shader>("Shaders/NoTransform.vert");
        frameContext.Meshes.push_back(mesh); // Hold onto reference so it is not destroyed early
        Material material{};
        material.FragmentShader = m_DeferredLightingShader;
        material.DescriptorSet = executionContext.ReadAttachments;

        Ref<Shader> vertexShader = mesh.VertexShader;

        PipelineStateHandle pipeline = m_PipelineStateCache.GetGraphicsPipeline(executionContext.RenderPass, material, mesh, 0, CullMode::NONE);
        commandBuffer->BindPipeline(pipeline);
        commandBuffer->SetViewportAndScissor(m_ViewportSize);

        commandBuffer->BindDescriptorSet(frameContext.SceneDataDescriptorSet, 0);
        commandBuffer->BindDescriptorSet(frameContext.EnvironmentMapDescriptorSet, 1);
        commandBuffer->BindDescriptorSet(frameContext.ShadowLightMatricesDescriptorSet, 2);
        commandBuffer->BindDescriptorSet(executionContext.ReadAttachments, 3);

        commandBuffer->BindVertexBuffer(mesh.VertexBuffer);
        commandBuffer->BindIndexBuffer(mesh.IndexBuffer);

        DeferredLightingPushConstants deferredLightingPushConstants
        {
            .CameraZNear = m_SceneCamera.GetNearPlane(),
            .CameraZFar = m_SceneCamera.GetFarPlane(),
            .NumShadowCascades = m_RendererSettings.NumShadowCascades,
            .ShowCascadeDebugView = m_RendererSettings.DebugView == RendererDebugView::CASCADED_SHADOW_MAP_BOUNDARIES,
            .ShadowMapBias = m_RendererSettings.ShadowMapBias,
        };

        commandBuffer->PushConstants(&deferredLightingPushConstants, sizeof(deferredLightingPushConstants));

        commandBuffer->DrawElementsIndexed(mesh.IndexBuffer);

        m_PipelineStateCache.SetDescriptorSetStack({frameContext.SceneDataDescriptorSet});
    }



    static std::vector<Vec4> GetFrustumCornersWorldSpace(const Mat4& projectionView)
    {
        const Mat4 cameraInverse = glm::inverse(projectionView);

        std::vector<Vec4> frustumCorners;

        // Loops to get each corner of the frustum
        for (unsigned int x = 0; x < 2; ++x)
        {
            for (unsigned int y = 0; y < 2; ++y)
            {
                for (unsigned int z = 0; z < 2; ++z)
                {
                    const Vec4 point =
                        cameraInverse * Vec4(
                            2.0f * x - 1.0f,
                            2.0f * y - 1.0f,
                            2.0f * z - 1.0f,
                            1.0f);
                    frustumCorners.push_back(point / point.w);
                }
            }
        }

        return frustumCorners;
    }


    float CalcCascadeZFar(float zNear, float zFar, float cascadeNum, float totalCascades)
    {
        constexpr float blendFactor = .5;
        const float logComponent = blendFactor * (zNear * std::pow((zFar / zNear), cascadeNum / totalCascades));
        const float linearComponent = (1 - blendFactor) * (zNear + cascadeNum / totalCascades * (zFar - zNear));
        return logComponent + linearComponent;
    }


    void SceneRendererImpl::CascadedShadowMapsPass()
    {
        PROFILE_SCOPE("CascadedShadowMapsPass")

        if (!m_RendererSettings.IsShadowsOn) { return; }
        if (m_FirstDirectionalLightInScene.LightType != LightType::DIRECTIONAL) { return; }

        const RenderGraphPassExecutionContext& executionContext = m_RenderGraph.GetExecutionContext();
        FrameContext& frameContext = m_FrameContexts[m_CurrentFrameIndex];
        CommandBufferHandle commandBuffer = executionContext.CommandBuffer;

        AssetRegistry& registry = Engine::Get().GetAssetManager().GetRegistry();

        m_LightSpaceMatrices.clear();

        float zNear = m_SceneCamera.GetNearPlane();
        float zFar = m_SceneCamera.GetFarPlane();
        std::vector<float> frustumRanges;
        frustumRanges.reserve(m_RendererSettings.NumShadowCascades);
        frustumRanges.push_back(zNear);


        for (uint32 i = 0; i < m_RendererSettings.NumShadowCascades; i++)
        {
            float cascadeZFar = CalcCascadeZFar(zNear, zFar, i + 1, m_RendererSettings.NumShadowCascades);
            frustumRanges.push_back(cascadeZFar);
            Camera subfrustumCamera = Camera(CameraType::PERSPECTIVE, m_SceneCamera.GetAspectRatio(), frustumRanges[i], cascadeZFar);
            subfrustumCamera.SetPosition(m_SceneCamera.GetPosition());
            subfrustumCamera.SetRotation(m_SceneCamera.GetRotation());
            std::vector<Vec4> frustumCorners = GetFrustumCornersWorldSpace(subfrustumCamera.GetViewProjectionMatrix());


            // Find center of frustum

            Vec3 center = Vec3(0.0f);
            for (auto& cornerPosition : frustumCorners)
            {
                center += Vec3(cornerPosition);
            }
            center /= frustumCorners.size();

            const Vec3 lightDir = glm::normalize(m_FirstDirectionalLightInScene.Position);
            Vec3 up = Vec3(0.0f, 1.0f, 0.0f);

            // Check if the light direction is parallel to the default up vector
            if (glm::abs(glm::dot(lightDir, up)) > 0.999f)
            {
                up = Vec3(0.0f, 0.0f, 1.0f);
            }

            Mat4 lightView = glm::lookAt(center - lightDir,
                               center,
                               up);


            // Get the min and max positions of the frustum in world space
            float minX = std::numeric_limits<float>::max();
            float maxX = std::numeric_limits<float>::lowest();
            float minY = std::numeric_limits<float>::max();
            float maxY = std::numeric_limits<float>::lowest();
            float minZ = std::numeric_limits<float>::max();
            float maxZ = std::numeric_limits<float>::lowest();
            for (const auto& cornerPosition : frustumCorners)
            {
                const Vec4 trf = lightView * cornerPosition;
                minX = std::min(minX, trf.x);
                maxX = std::max(maxX, trf.x);
                minY = std::min(minY, trf.y);
                maxY = std::max(maxY, trf.y);
                minZ = std::min(minZ, trf.z);
                maxZ = std::max(maxZ, trf.z);
            }


            // Snap to texel boundaries for stability
            float shadowMapSize = m_RendererSettings.ShadowMapResolution;
            float texelSizeX = (maxX - minX) / shadowMapSize;
            float texelSizeY = (maxY - minY) / shadowMapSize;

            minX = floor(minX / texelSizeX) * texelSizeX;
            maxX = ceil(maxX / texelSizeX) * texelSizeX;
            minY = floor(minY / texelSizeY) * texelSizeY;
            maxY = ceil(maxY / texelSizeY) * texelSizeY;

            float zMult = m_RendererSettings.ShadowMapZMultiplier;
            const Mat4 lightProjection = glm::ortho(minX, maxX, minY, maxY, minZ * zMult, maxZ * zMult);


            m_LightSpaceMatrices.push_back(lightProjection * lightView);
        }

        frameContext.ShadowLightMatrices->CopyDataToBuffer(m_LightSpaceMatrices.data(), sizeof(Mat4) * m_LightSpaceMatrices.size());

        for (uint32 i = 0; i < frameContext.Meshes.size(); i++)
        {
            Mesh mesh = frameContext.Meshes[i];
            Material& material = frameContext.Materials[i];

            if (material.ShaderModel != ShaderModel::PBR) { continue; }

            mesh.VertexShader = registry.GetOrCreateAsset<Shader>("Shaders/ShadowMap.vert");

            Material shadowMapMaterial{};
            shadowMapMaterial.FragmentShader = registry.GetOrCreateAsset<Shader>("Shaders/DepthWriteOnly.frag");
            shadowMapMaterial.DescriptorSet = frameContext.ShadowLightMatricesDescriptorSet;

            PipelineStateHandle shadowMapPipeline = m_PipelineStateCache.GetGraphicsPipeline(executionContext.RenderPass, shadowMapMaterial, mesh, 0, CullMode::FRONT);
            commandBuffer->BindPipeline(shadowMapPipeline);
            commandBuffer->SetViewportAndScissor(Vec2(m_RendererSettings.ShadowMapResolution));

            commandBuffer->BindDescriptorSet(frameContext.SceneDataDescriptorSet, 0);
            commandBuffer->BindDescriptorSet(frameContext.ShadowLightMatricesDescriptorSet, 1);


            commandBuffer->BindVertexBuffer(mesh.VertexBuffer);
            commandBuffer->BindIndexBuffer(mesh.IndexBuffer);


            commandBuffer->PushConstants(&frameContext.Transforms[i], sizeof(Mat4));

            commandBuffer->DrawElementsInstanced(mesh.IndexBuffer, m_RendererSettings.NumShadowCascades);
        }
    }


    void SceneRendererImpl::EnvironmentMapPass()
    {
        PROFILE_SCOPE("EnvironmentMapPass")

        const RenderGraphPassExecutionContext& executionContext = m_RenderGraph.GetExecutionContext();
        FrameContext& frameContext = m_FrameContexts[m_CurrentFrameIndex];
        CommandBufferHandle commandBuffer = executionContext.CommandBuffer;

        // Cubemap
        AssetRegistry& registry = Engine::Get().GetAssetManager().GetRegistry();
        Scene& activeScene = Engine::Get().GetSceneManager().GetActiveScene();

        Mesh& cubemapMesh = *registry.GetAsset<Mesh>("Meshes/Cube.obj");
        cubemapMesh.VertexShader = registry.GetOrCreateAsset<Shader>("Shaders/Cubemap.vert");
        frameContext.Meshes.push_back(cubemapMesh); // Hold onto reference so it is not destroyed early

        Material environmentMapMaterial{};
        environmentMapMaterial.FragmentShader = registry.GetOrCreateAsset<Shader>("Shaders/EnvironmentMap.frag");
        environmentMapMaterial.DescriptorSet = frameContext.EnvironmentMapDescriptorSet;

        PipelineStateHandle cubemapPipeline = m_PipelineStateCache.GetGraphicsPipeline(executionContext.RenderPass, environmentMapMaterial, cubemapMesh, 0, CullMode::NONE);
        commandBuffer->BindPipeline(cubemapPipeline);
        commandBuffer->SetViewportAndScissor(m_ViewportSize);

        commandBuffer->BindDescriptorSet(frameContext.SceneDataDescriptorSet, 0);
        commandBuffer->BindDescriptorSet(environmentMapMaterial.DescriptorSet, 1);

        commandBuffer->BindVertexBuffer(cubemapMesh.VertexBuffer);
        commandBuffer->BindIndexBuffer(cubemapMesh.IndexBuffer);

        commandBuffer->PushConstants(&activeScene.EnvironmentMapBlur, sizeof(activeScene.EnvironmentMapBlur));
        commandBuffer->DrawElementsIndexed(cubemapMesh.IndexBuffer);
    }



    // For demo purposes to show benefits of ACES (ACES is used normally)
    enum class ToneMappingDebugView : uint32
    {
        NO_DEBUG_VIEW,
        DEBUG_VIEW_REINHARD,
        DEBUG_VIEW_NO_TONE_MAPPING,
    };


    void SceneRendererImpl::ToneMappingPass()
    {
        PROFILE_SCOPE("ToneMappingPass")

        struct ToneMappingPassPushConstants
        {
            float Exposure;
            Vec2 ShaperInputRange;
            ToneMappingDebugView ToneMappingDebugView;
        };

        const RenderGraphPassExecutionContext& executionContext = m_RenderGraph.GetExecutionContext();
        FrameContext& frameContext = m_FrameContexts[m_CurrentFrameIndex];
        CommandBufferHandle commandBuffer = executionContext.CommandBuffer;

        AssetRegistry& registry = Engine::Get().GetAssetManager().GetRegistry();
        Ref<CubeLUT> toneMappingLUT = registry.GetOrCreateAsset<CubeLUT>("LUTs/ACEScg_to_sRGB_RRT_ODT.cube");

        Mesh& quadMesh = *registry.GetAsset<Mesh>("Meshes/Quad.obj");
        quadMesh.VertexShader = registry.GetOrCreateAsset<Shader>("Shaders/NoTransform.vert");
        frameContext.Meshes.push_back(quadMesh); // Hold onto reference so it is not destroyed early

        m_PipelineStateCache.SetDescriptorSetStack({frameContext.SceneDataDescriptorSet, executionContext.ReadAttachments});

        Material toneMapperMaterial{};
        toneMapperMaterial.FragmentShader = registry.GetOrCreateAsset<Shader>("Shaders/ToneMapping.frag");
        toneMapperMaterial.DescriptorSet = m_RTT_ODT_LUT_DescriptorSet;

        PipelineStateHandle toneMappingPipeline = m_PipelineStateCache.GetGraphicsPipeline(executionContext.RenderPass, toneMapperMaterial, quadMesh, 0, CullMode::NONE);
        commandBuffer->BindPipeline(toneMappingPipeline);
        commandBuffer->SetViewportAndScissor(m_ViewportSize);

        commandBuffer->BindDescriptorSet(frameContext.SceneDataDescriptorSet, 0);
        commandBuffer->BindDescriptorSet(executionContext.ReadAttachments, 1);
        commandBuffer->BindDescriptorSet(toneMapperMaterial.DescriptorSet, 2);

        ToneMappingPassPushConstants toneMappingPushConstants;
        toneMappingPushConstants.Exposure = m_SceneExposure;
        toneMappingPushConstants.ShaperInputRange = toneMappingLUT->ShaperInputRange;
        toneMappingPushConstants.ToneMappingDebugView = ToneMappingDebugView::NO_DEBUG_VIEW;


        if (m_RendererSettings.DebugView == RendererDebugView::TONE_MAPPING_REINHARD)
        {
            toneMappingPushConstants.ToneMappingDebugView = ToneMappingDebugView::DEBUG_VIEW_REINHARD;
        }
        else if (m_RendererSettings.DebugView == RendererDebugView::TONE_MAPPING_NONE)
        {
            toneMappingPushConstants.ToneMappingDebugView = ToneMappingDebugView::DEBUG_VIEW_NO_TONE_MAPPING;
        }

        commandBuffer->PushConstants(&toneMappingPushConstants, sizeof(toneMappingPushConstants));

        commandBuffer->BindVertexBuffer(quadMesh.VertexBuffer);
        commandBuffer->BindIndexBuffer(quadMesh.IndexBuffer);

        commandBuffer->DrawElementsIndexed(quadMesh.IndexBuffer);

        m_PipelineStateCache.SetDescriptorSetStack(frameContext.SceneDataDescriptorSet);
    }


    void SceneRendererImpl::FXAAPass()
    {
        PROFILE_SCOPE("FXAAPass")

        const RenderGraphPassExecutionContext& executionContext = m_RenderGraph.GetExecutionContext();
        FrameContext& frameContext = m_FrameContexts[m_CurrentFrameIndex];
        CommandBufferHandle commandBuffer = executionContext.CommandBuffer;

        AssetRegistry& registry = Engine::Get().GetAssetManager().GetRegistry();

        Mesh& quadMesh = *registry.GetAsset<Mesh>("Meshes/Quad.obj");
        quadMesh.VertexShader = registry.GetOrCreateAsset<Shader>("Shaders/NoTransform.vert");
        frameContext.Meshes.push_back(quadMesh); // Hold onto reference so it is not destroyed early

        Material toneMapperMaterial{};
        toneMapperMaterial.FragmentShader = registry.GetOrCreateAsset<Shader>("Shaders/FXAAPass.frag");
        toneMapperMaterial.DescriptorSet = executionContext.ReadAttachments;
        PipelineStateHandle fxaaPipeline = m_PipelineStateCache.GetGraphicsPipeline(executionContext.RenderPass, toneMapperMaterial, quadMesh, 0, CullMode::NONE);
        commandBuffer->BindPipeline(fxaaPipeline);
        commandBuffer->SetViewportAndScissor(m_ViewportSize);

        commandBuffer->BindDescriptorSet(frameContext.SceneDataDescriptorSet, 0);
        commandBuffer->BindDescriptorSet(executionContext.ReadAttachments, 1);

        commandBuffer->BindVertexBuffer(quadMesh.VertexBuffer);
        commandBuffer->BindIndexBuffer(quadMesh.IndexBuffer);

        commandBuffer->DrawElementsIndexed(quadMesh.IndexBuffer);
    }


    void SceneRendererImpl::ComputeEnvironmentIBL()
    {
        PROFILE_SCOPE("ComputeEnvironmentIBL")

        FrameContext& frameContext = m_FrameContexts[m_CurrentFrameIndex];

        // -------------- Compute Irradiance -------------------------------------------------

        RendererAPI::ExecuteOneTimeAndBlock([this](CommandBufferHandle asyncCommandBuffer){ ComputeIrradianceMap(asyncCommandBuffer); });


        // -------------- Compute Prefiltered Environment -------------------------------------------------

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
        frameContext.IsEnvironmentMapIBLCalculationNeeded = false;
    }


    struct ComputeIrradianceMapPushConstants
    {
        float FaceIndex;
    };
    static_assert(sizeof(ComputeIrradianceMapPushConstants) <= MaxPushConstantRange, "Push constant can not be greater than MaxPushConstantRange (usually 128) bytes in size");


    void SceneRendererImpl::ComputeIrradianceMap(const CommandBufferHandle& commandBuffer)
    {
        PROFILE_SCOPE("ComputeIrradianceMap")

        commandBuffer->BeginLabel("IrradianceMapCalculation", Vec4(1.0f, 0.0f, 1.0f, 1.0f));

        FrameContext& frameContext = m_FrameContexts[m_CurrentFrameIndex];
        AssetRegistry& registry = Engine::Get().GetAssetManager().GetRegistry();

        m_PipelineStateCache.SetDescriptorSetStack(std::vector<DescriptorSetHandle>{});

        ShaderHandle irradianceCalcShader = registry.GetOrCreateAsset<Shader>("Shaders/ComputeIrradianceMap.comp");
        ;
        PipelineStateHandle computePipeline = m_PipelineStateCache.GetComputePipeline(irradianceCalcShader, m_EnvironmentMapStorageImagesSet);
        commandBuffer->BindPipeline(computePipeline);
        commandBuffer->BindDescriptorSet(m_EnvironmentMapStorageImagesSet, 0);


        ComputeIrradianceMapPushConstants computeIrradianceMapPushConstants;

        constexpr uint32 numFaces = 6;
        for (uint32 i = 0; i < numFaces; i++)
        {
            computeIrradianceMapPushConstants.FaceIndex = i;
            commandBuffer->PushConstants(&computeIrradianceMapPushConstants, sizeof(computeIrradianceMapPushConstants));

            // Dispatching 2x2 blocks as the local layout is 8x8 and the irradiance faces are 16x16
            uint32 groupCountSize = EnvironmentMapIrradianceSize / 8;
            commandBuffer->Dispatch(groupCountSize, groupCountSize, 1);
        }

        m_PipelineStateCache.SetDescriptorSetStack(frameContext.SceneDataDescriptorSet);

        commandBuffer->EndLabel();
    }


    struct ComputePrefilteredEnvironmentMapPushData
    {
        uint32 FaceIndex;
        float Roughness;
    };


    void SceneRendererImpl::ComputePrefilteredEnvironmentMap(const CommandBufferHandle& commandBuffer, uint32 mipLevel, UVec2 mipDimensions)
    {
        PROFILE_SCOPE("ComputePrefilteredEnvironmentMap")

        commandBuffer->BeginLabel("PrefilteredEnvironmentMapCalc", Vec4(1.0f, 0.0f, 1.0f, 1.0f));

        FrameContext& frameContext = m_FrameContexts[m_CurrentFrameIndex];
        AssetRegistry& registry = Engine::Get().GetAssetManager().GetRegistry();

        m_PipelineStateCache.SetDescriptorSetStack(std::vector<DescriptorSetHandle>{});

        ShaderHandle prefilterCalcShader = registry.GetOrCreateAsset<Shader>("Shaders/ComputePrefilteredEnvironmentMap.comp");

        PipelineStateHandle computePipeline = m_PipelineStateCache.GetComputePipeline(prefilterCalcShader, m_EnvironmentMapStorageImagesSet);
        commandBuffer->BindPipeline(computePipeline);
        commandBuffer->BindDescriptorSet(m_EnvironmentMapStorageImagesSet, 0);

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

            commandBuffer->PushConstants(&prefilteredEnvironmentMapPushData, sizeof(prefilteredEnvironmentMapPushData));

            uint32 groupSizeX = std::max(mipWidth / 32, 1u);
            uint32 groupSizeY = std::max(mipHeight / 32, 1u);
            commandBuffer->Dispatch(groupSizeX, groupSizeY, 1);
        }

        m_PipelineStateCache.SetDescriptorSetStack(frameContext.SceneDataDescriptorSet);

        commandBuffer->EndLabel();
    }


    void SceneRendererImpl::DrawEditorUI(CommandBufferHandle commandBuffer, RenderTargetHandle renderTarget)
    {
        PROFILE_SCOPE("DrawEditorUI")

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

            commandBuffer->SetPipelineBarrier(pipelineBarrier);
        }


        // ImGui Rendering
        commandBuffer->BeginLabel("ImGui Render Draws", Vec4(0.0f, 0.0f, 1.0f, 1.0f));
        commandBuffer->BeginRenderPass(m_ImGuiRenderPass, m_FrameContexts[renderTarget->GetImageIndex()].WindowFramebuffer);
        RendererAPI::CallImGuiDraws(commandBuffer);
        commandBuffer->EndRenderPass();
        commandBuffer->EndLabel();


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

            commandBuffer->SetPipelineBarrier(pipelineBarrier);
        }

    }


    static bool IsBoundingSphereOutsidePlane(Vec4 plane, const BoundingSphere& boundingSphere)
    {
        float distance = glm::dot(Vec3(plane), boundingSphere.Center) + plane.w;
        return distance <= -boundingSphere.Radius;
    }


    bool SceneRendererImpl::ShouldCullMesh(const Mesh& mesh, const Mat4& modelTransform)
    {
        PROFILE_SCOPE("ShouldCullMesh")

        std::array<Vec4, 6> frustum;
        BoundingSphere worldSpaceBoundingSphere;
        worldSpaceBoundingSphere.Center = modelTransform * glm::vec4(mesh.BoundingSphere.Center, 1.0f);

        Vec3 scale;
        scale.x = glm::length(Vec3(modelTransform[0]));
        scale.y = glm::length(Vec3(modelTransform[1]));
        scale.z = glm::length(Vec3(modelTransform[2]));
        float biggestScale = std::max(std::max(scale.x, scale.y), scale.z);
        worldSpaceBoundingSphere.Radius = biggestScale * mesh.BoundingSphere.Radius * 1.01f;

        // Right Plane
        frustum[0].x = m_SceneViewProjection[0][3] - m_SceneViewProjection[0][0];
        frustum[0].y = m_SceneViewProjection[1][3] - m_SceneViewProjection[1][0];
        frustum[0].z = m_SceneViewProjection[2][3] - m_SceneViewProjection[2][0];
        frustum[0].w = m_SceneViewProjection[3][3] - m_SceneViewProjection[3][0];

        // Left Plane
        frustum[1].x = m_SceneViewProjection[0][3] + m_SceneViewProjection[0][0];
        frustum[1].y = m_SceneViewProjection[1][3] + m_SceneViewProjection[1][0];
        frustum[1].z = m_SceneViewProjection[2][3] + m_SceneViewProjection[2][0];
        frustum[1].w = m_SceneViewProjection[3][3] + m_SceneViewProjection[3][0];

        // Top Plane
        frustum[2].x = m_SceneViewProjection[0][3] - m_SceneViewProjection[0][1];
        frustum[2].y = m_SceneViewProjection[1][3] - m_SceneViewProjection[1][1];
        frustum[2].z = m_SceneViewProjection[2][3] - m_SceneViewProjection[2][1];
        frustum[2].w = m_SceneViewProjection[3][3] - m_SceneViewProjection[3][1];

        // Bottom Plane
        frustum[3].x = m_SceneViewProjection[0][3] + m_SceneViewProjection[0][1];
        frustum[3].y = m_SceneViewProjection[1][3] + m_SceneViewProjection[1][1];
        frustum[3].z = m_SceneViewProjection[2][3] + m_SceneViewProjection[2][1];
        frustum[3].w = m_SceneViewProjection[3][3] + m_SceneViewProjection[3][1];

        // Far Plane
        frustum[4].x = m_SceneViewProjection[0][3] - m_SceneViewProjection[0][2];
        frustum[4].y = m_SceneViewProjection[1][3] - m_SceneViewProjection[1][2];
        frustum[4].z = m_SceneViewProjection[2][3] - m_SceneViewProjection[2][2];
        frustum[4].w = m_SceneViewProjection[3][3] - m_SceneViewProjection[3][2];

        // Near Plane
        frustum[5].x = m_SceneViewProjection[0][3] + m_SceneViewProjection[0][2];
        frustum[5].y = m_SceneViewProjection[1][3] + m_SceneViewProjection[1][2];
        frustum[5].z = m_SceneViewProjection[2][3] + m_SceneViewProjection[2][2];
        frustum[5].w = m_SceneViewProjection[3][3] + m_SceneViewProjection[3][2];

        for (Vec4& plane : frustum)
        {
            float normalLength = glm::length(Vec3(plane));
            plane /= normalLength;
            if (IsBoundingSphereOutsidePlane(plane, worldSpaceBoundingSphere)) { return true; }
        }

        return false;
    }


    void SceneRendererImpl::ResizeViewport(uint32 width, uint32 height)
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


    RendererType SceneRendererImpl::GetType() const
    {
        return m_RendererSettings.RendererType;
    }

} // Renderer