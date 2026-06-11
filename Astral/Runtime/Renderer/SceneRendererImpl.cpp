/**
* @file SceneRendererImpl.cpp
* @author Andrew Fagan
* @date 7/1/2025
*/

#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#include "SceneRendererImpl.h"

#include "RHI/RendererAPI.h"

#include "Asset/AssetManager.h"
#include "Core/Engine.h"
#include "Renderer/RHI/Resources/CommandBuffer.h"
#include "Renderer/RHI/Resources/RenderTarget.h"
#include "Renderer/RHI/Resources/Shader.h"
#include "Renderer/RendererManager.h"
#include "Common/CubeLUT.h"
#include "Debug/ImGui/ImGuiManager.h"
#include "Scenes/SceneManager.h"
#include "Window/WindowManager.h"


#include <future>
#include <numeric>
#include <glm/gtc/type_ptr.hpp>



namespace Astral {

    void SceneRendererImpl::Init()
    {
        m_WindowResizedListener = EventListener<FramebufferResizedEvent>{[this](FramebufferResizedEvent event) { ResizeWindowImages(event.Width, event.Height); }};
        m_WindowResizedListener.StartListening();

        m_ViewportSize = RendererAPI::GetContext().GetWindowFramebufferDimensions();
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
        rendererSettings.ShadowMapCascadeLogFactor = .8;

        SetRendererSettings(rendererSettings);


        PipelineStateCache& pipelineStateCache = RendererAPI::GetContext().GetPipelineStateCache();
        pipelineStateCache.SetDescriptorSetStack(m_FrameContexts[0].SceneDataDescriptorSet);
        m_CurrentViewportTexture = m_FrameContexts[1].OffscreenDescriptorSet;

        Engine::Get().GetRendererManager().GetContext().InitImGuiForAPIBackend(m_ImGuiRenderPass);

        Device& device = RendererAPI::GetDevice();
        Swapchain& swapchain = device.GetSwapchain();
        uint32 numSwapchainImages = swapchain.GetNumberOfImages();

        m_ForwardRendererPath.InitRenderPasses(numSwapchainImages);
        m_DeferredRendererPath.InitRenderPasses(numSwapchainImages);
    }


    void SceneRendererImpl::Shutdown()
    {
        PROFILE_SCOPE("SceneRenderer::Shutdown")

        m_DeferredRendererPath.ShutdownRenderPasses();
        m_ForwardRendererPath.ShutdownRenderPasses();

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

        RenderTargetHandle renderTarget = swapchain.AcquireNextImage();

        // If the swapchain could not acquire successfully, then rebuild swapchain and try again
        if (renderTarget == nullptr)
        {
            SetVSync(m_RendererSettings.IsVSyncEnabled); // This rebuilds the swapchain with the same settings and handles synchronizing resources
            return BeginScene(sceneDescription);
        }

        m_IsSceneStarted = true;
        m_CurrentFrameIndex++;
        if (m_CurrentFrameIndex == 3) { m_CurrentFrameIndex = 0; }

        SharedFrameContext& frameContext = m_FrameContexts[m_CurrentFrameIndex];
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

        frameContext.SceneDataBuffer->CopyDataToBuffer(&sceneData, sizeof(SceneData));

        if (sizeof(Light) * sceneData.NumLights > frameContext.SceneLightsBuffer->GetAllocatedSize())
        {
            uint32 currentBufferAllocation = frameContext.SceneLightsBuffer->GetAllocatedSize();
            frameContext.SceneLightsBuffer->ReallocateMemory(currentBufferAllocation * 2);
            frameContext.SceneDataDescriptorSet->UpdateStorageBufferBinding(1, frameContext.SceneLightsBuffer); // Reallocation will create a new buffer, so re-add that buffer to descriptor set
        }
        frameContext.SceneLightsBuffer->CopyDataToBuffer((void*)sceneDescription.Lights.data(), sizeof(Light) * sceneData.NumLights);

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


        frameContext.SceneDescription = sceneDescription;


        frameContext.MainList.Clear();
        frameContext.ShadowMapList.Clear();
    }


    void SceneRendererImpl::EndScene()
    {
        SharedFrameContext& frameContext = m_FrameContexts[m_CurrentFrameIndex];
        frameContext.MainList.SortByMaterial(frameContext.SceneDescription.Camera.GetPosition());
        frameContext.ShadowMapList.SortFrontToBack(frameContext.SceneDescription.Camera.GetPosition());

        {
            PROFILE_SCOPE("SceneRenderer::EndScene")
            m_IsSceneStarted = false;
        }

        RenderScene();
    }


    void SceneRendererImpl::Submit(const Ref<Mesh>& mesh, const Ref<Material>& material, const Mat4& transform)
    {
        ASSERT(m_IsSceneStarted, "Scene has not been started! Use SceneRenderer::BeginScene")
        SharedFrameContext& frameContext = m_FrameContexts[m_CurrentFrameIndex];

        if (!mesh) { AE_WARN("Empty mesh submitted! Skipping!"); return; }
        if (!material) { AE_WARN("Empty material submitted! Skipping!"); return; }

        frameContext.ShadowMapList.Submit(mesh, material, transform);

        if (m_RendererSettings.IsFrustumCullingEnabled)
        {
            if (ShouldCullMesh(*mesh, transform)) { return; }
        }

        frameContext.MainList.Submit(mesh, material, transform);
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
        m_RendererSettings.ShadowMapCascadeLogFactor = rendererSettings.ShadowMapCascadeLogFactor;

        if (isRenderGraphRebuildNeeded)
        {
            BuildRenderGraph();
        }
    }


    const RendererSettings& SceneRendererImpl::GetRendererSettings()
    {
        return m_RendererSettings;
    }


    DescriptorSetHandle SceneRendererImpl::GetViewportTexture()
    {
        DescriptorSetHandle descriptorSet = m_CurrentViewportTexture;
        m_CurrentViewportTexture = nullptr;
        return descriptorSet;
    }


    void SceneRendererImpl::BuildRenderGraph()
    {
        std::vector<TextureHandle> outputTextures;
        constexpr int numFramesInFlight = 3;
        outputTextures.reserve(numFramesInFlight);
        for (int i = 0; i < numFramesInFlight; i++)
        {
            TextureHandle offscreenOutput = m_FrameContexts[i].OffscreenRenderTarget;
            outputTextures.push_back(offscreenOutput);
        }


        if (m_RendererSettings.RendererType == RendererType::DEFERRED)
        {
            m_DeferredRendererPath.BuildRenderGraph(m_RenderGraph, outputTextures);
        }
        else if (m_RendererSettings.RendererType == RendererType::FORWARD)
        {
            m_ForwardRendererPath.BuildRenderGraph(m_RenderGraph, outputTextures);
        }
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
        imguiRenderPass->AddColorAttachment(renderTargetIndex, ImageLayout::COLOR_ATTACHMENT_OPTIMAL);
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
            m_FrameContexts.emplace_back(SharedFrameContext());
            SharedFrameContext& context = m_FrameContexts[i];
            context.MainList.Clear();
            context.ShadowMapList.Clear();


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


            context.SceneDataBuffer = device.CreateUniformBuffer(nullptr, sizeof(SceneData), GPUMemoryType::HOST_VISIBLE);
            std::string sceneDataBufferName = std::string("Scene_Data_Buffer_") + std::to_string(i);
            RendererAPI::NameObject(context.SceneDataBuffer, sceneDataBufferName);

            context.SceneLightsBuffer = device.CreateStorageBuffer(nullptr, 1024, GPUMemoryType::HOST_VISIBLE);
            std::string sceneLightsBufferName = std::string("Scene_Lights_Buffer_") + std::to_string(i);
            RendererAPI::NameObject(context.SceneLightsBuffer, sceneLightsBufferName);

            context.SceneDataDescriptorSet = device.CreateDescriptorSet();
            context.SceneDataDescriptorSet->BeginBuildingSet();
            context.SceneDataDescriptorSet->AddDescriptorUniformBuffer(context.SceneDataBuffer, ShaderStage::ALL);
            context.SceneDataDescriptorSet->AddDescriptorStorageBuffer(context.SceneLightsBuffer, ShaderStage::ALL);
            context.SceneDataDescriptorSet->EndBuildingSet();
            std::string sceneDataDescriptorSetName = std::string("Scene_Data_Descriptor_Set_") + std::to_string(i);
            RendererAPI::NameObject(context.SceneDataDescriptorSet, sceneDataDescriptorSetName);


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

            context.ShadowLightMatrices = device.CreateUniformBuffer(nullptr, sizeof(Mat4) * 8, GPUMemoryType::HOST_VISIBLE);
            context.ShadowLightMatricesDescriptorSet = device.CreateDescriptorSet();
            context.ShadowLightMatricesDescriptorSet->BeginBuildingSet();
            context.ShadowLightMatricesDescriptorSet->AddDescriptorUniformBuffer(context.ShadowLightMatrices, ShaderStage::ALL);
            context.ShadowLightMatricesDescriptorSet->EndBuildingSet();

            context.IsEnvironmentMapIBLCalculationNeeded = true;
        }


        SharedFrameContext& context = m_FrameContexts[0];
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

        SharedFrameContext& frameContext = m_FrameContexts[m_CurrentFrameIndex];
        RenderTargetHandle renderTarget = frameContext.SceneRenderTarget;
        CommandBufferHandle commandBuffer = frameContext.SceneCommandBuffer;


        commandBuffer->BeginRecording();

        if (frameContext.IsEnvironmentMapIBLCalculationNeeded)
        {
            ComputeEnvironmentIBL();
        }

        // Viewport Rendering
        m_RenderGraph.Execute(frameContext, m_CurrentFrameIndex, frameContext.OffscreenRenderTarget);

        // Editor UI rendering to swapchain image
        DrawEditorUI(commandBuffer, renderTarget);

        commandBuffer->EndRecording();


        CommandQueueHandle commandQueue = device.GetPrimaryCommandQueue();
        commandQueue->Submit(commandBuffer, renderTarget);
        commandQueue->Present(renderTarget);


        uint32 nextFrameIndex = (m_CurrentFrameIndex + 1) % 3;
        if (m_CurrentViewportTexture == nullptr)
        {
            m_CurrentViewportTexture = m_FrameContexts[nextFrameIndex].OffscreenDescriptorSet;
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
            SharedFrameContext& frameContext = m_FrameContexts[i];

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

        std::vector<RenderTargetHandle> renderTargets = swapchain.GetRenderTargets();

        for (int i = 0; i < swapchain.GetNumberOfImages(); i++)
        {
            std::string swapchainRenderTargetName = "Swapchain_Render_Target_" + std::to_string(i);
            RendererAPI::NameObject(renderTargets[i]->GetAsTexture(), swapchainRenderTargetName);
        }
    }


    void SceneRendererImpl::ComputeEnvironmentIBL()
    {
        PROFILE_SCOPE("SceneRendererImpl::ComputeEnvironmentIBL")

        SharedFrameContext& frameContext = m_FrameContexts[m_CurrentFrameIndex];

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
        PROFILE_SCOPE("SceneRendererImpl::ComputeIrradianceMap")

        commandBuffer->BeginLabel("IrradianceMapCalculation", Vec4(1.0f, 0.0f, 1.0f, 1.0f));

        SharedFrameContext& frameContext = m_FrameContexts[m_CurrentFrameIndex];
        AssetRegistry& registry = Engine::Get().GetAssetManager().GetRegistry();

        PipelineStateCache& pipelineStateCache = RendererAPI::GetContext().GetPipelineStateCache();
        pipelineStateCache.SetDescriptorSetStack(std::vector<DescriptorSetHandle>{});

        ShaderHandle irradianceCalcShader = registry.CreateAsset<Shader>("Shaders/ComputeIrradianceMap.comp");
        ;
        PipelineStateHandle computePipeline = pipelineStateCache.GetComputePipeline(irradianceCalcShader, m_EnvironmentMapStorageImagesSet);
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

        pipelineStateCache.SetDescriptorSetStack(frameContext.SceneDataDescriptorSet);

        commandBuffer->EndLabel();
    }


    struct ComputePrefilteredEnvironmentMapPushData
    {
        uint32 FaceIndex;
        float Roughness;
    };


    void SceneRendererImpl::ComputePrefilteredEnvironmentMap(const CommandBufferHandle& commandBuffer, uint32 mipLevel, UVec2 mipDimensions)
    {
        PROFILE_SCOPE("SceneRendererImpl::ComputePrefilteredEnvironmentMap")

        commandBuffer->BeginLabel("PrefilteredEnvironmentMapCalc", Vec4(1.0f, 0.0f, 1.0f, 1.0f));

        SharedFrameContext& frameContext = m_FrameContexts[m_CurrentFrameIndex];
        AssetRegistry& registry = Engine::Get().GetAssetManager().GetRegistry();

        PipelineStateCache& pipelineStateCache = RendererAPI::GetContext().GetPipelineStateCache();
        pipelineStateCache.SetDescriptorSetStack(std::vector<DescriptorSetHandle>{});

        ShaderHandle prefilterCalcShader = registry.CreateAsset<Shader>("Shaders/ComputePrefilteredEnvironmentMap.comp");

        PipelineStateHandle computePipeline = pipelineStateCache.GetComputePipeline(prefilterCalcShader, m_EnvironmentMapStorageImagesSet);
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

        pipelineStateCache.SetDescriptorSetStack(frameContext.SceneDataDescriptorSet);

        commandBuffer->EndLabel();
    }


    void SceneRendererImpl::DrawEditorUI(CommandBufferHandle commandBuffer, RenderTargetHandle renderTarget)
    {
        PROFILE_SCOPE("SceneRendererImpl::DrawEditorUI")

        TextureHandle offscreenRenderTarget = m_FrameContexts[m_CurrentFrameIndex].OffscreenRenderTarget;
        ImageLayout initialLayout = offscreenRenderTarget->GetLayout();
        {
            PipelineBarrier pipelineBarrier = {};
            pipelineBarrier.SourceStageMask = PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
            pipelineBarrier.DestinationStageMask = PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
            pipelineBarrier.DependencyFlags = DependencyFlags::BY_REGION_BIT;


            ImageMemoryBarrier viewportImageMemoryBarrier = {};
            viewportImageMemoryBarrier.SourceAccessMask = ACCESS_FLAGS_COLOR_ATTACHMENT_WRITE_BIT;
            viewportImageMemoryBarrier.DestinationAccessMask = ACCESS_FLAGS_SHADER_READ_BIT;
            viewportImageMemoryBarrier.OldLayout = offscreenRenderTarget->GetLayout();
            viewportImageMemoryBarrier.NewLayout = ImageLayout::SHADER_READ_ONLY_OPTIMAL;
            viewportImageMemoryBarrier.SourceQueueFamilyIndex = QueueFamilyIgnored;
            viewportImageMemoryBarrier.DestinationQueueFamilyIndex = QueueFamilyIgnored;
            viewportImageMemoryBarrier.Image = offscreenRenderTarget;
            viewportImageMemoryBarrier.ImageSubresourceRange = {
                .AspectMask = offscreenRenderTarget->GetImageAspect(),
                .BaseMipLevel = 0,
                .LevelCount = offscreenRenderTarget->GetNumMipLevels(),
                .BaseArrayLayer = 0,
                .LayerCount = offscreenRenderTarget->GetNumLayers()
            };
            ImageMemoryBarrier renderTargetImageMemoryBarrier = {};
            renderTargetImageMemoryBarrier.SourceAccessMask = ACCESS_FLAGS_MEMORY_READ_BIT;
            renderTargetImageMemoryBarrier.DestinationAccessMask = ACCESS_FLAGS_MEMORY_WRITE_BIT;
            renderTargetImageMemoryBarrier.OldLayout = ImageLayout::UNDEFINED;
            renderTargetImageMemoryBarrier.NewLayout = ImageLayout::COLOR_ATTACHMENT_OPTIMAL;
            renderTargetImageMemoryBarrier.SourceQueueFamilyIndex = QueueFamilyIgnored;
            renderTargetImageMemoryBarrier.DestinationQueueFamilyIndex = QueueFamilyIgnored;
            renderTargetImageMemoryBarrier.Image = renderTarget->GetAsTexture();
            renderTargetImageMemoryBarrier.ImageSubresourceRange = {
                .AspectMask = offscreenRenderTarget->GetImageAspect(),
                .BaseMipLevel = 0,
                .LevelCount = 1,
                .BaseArrayLayer = 0,
                .LayerCount = 1
            };

            pipelineBarrier.ImageMemoryBarriers.push_back(viewportImageMemoryBarrier);
            pipelineBarrier.ImageMemoryBarriers.push_back(renderTargetImageMemoryBarrier);

            commandBuffer->SetPipelineBarrier(pipelineBarrier);
        }


        // ImGui Rendering
        commandBuffer->BeginLabel("ImGui Render Draws", Vec4(0.0f, 0.0f, 1.0f, 1.0f));
        AttachmentResource attachmentResource = {
            .Resource = renderTarget->GetAsTexture(),
            .MipLevel = FullSubresourceRange,
            .ArrayLayer = FullSubresourceRange
        };
        commandBuffer->BeginRenderPass(m_ImGuiRenderPass, {attachmentResource});
        RendererAPI::CallImGuiDraws(commandBuffer);
        commandBuffer->EndRenderPass();
        commandBuffer->EndLabel();


        {
            PipelineBarrier pipelineBarrier = {};
            pipelineBarrier.SourceStageMask = PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
            pipelineBarrier.DestinationStageMask = PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
            pipelineBarrier.DependencyFlags = DependencyFlags::BY_REGION_BIT;

            ImageMemoryBarrier viewportImageMemoryBarrier = {};
            viewportImageMemoryBarrier.SourceAccessMask = ACCESS_FLAGS_SHADER_READ_BIT;
            viewportImageMemoryBarrier.DestinationAccessMask = ACCESS_FLAGS_COLOR_ATTACHMENT_WRITE_BIT;
            viewportImageMemoryBarrier.OldLayout = offscreenRenderTarget->GetLayout();
            viewportImageMemoryBarrier.NewLayout = initialLayout;
            viewportImageMemoryBarrier.SourceQueueFamilyIndex = QueueFamilyIgnored;
            viewportImageMemoryBarrier.DestinationQueueFamilyIndex = QueueFamilyIgnored;
            viewportImageMemoryBarrier.Image = offscreenRenderTarget;
            viewportImageMemoryBarrier.ImageSubresourceRange = {
                .AspectMask = offscreenRenderTarget->GetImageAspect(),
                .BaseMipLevel = 0,
                .LevelCount = 1,
                .BaseArrayLayer = 0,
                .LayerCount = 1
            };

            ImageMemoryBarrier renderTargetImageMemoryBarrier = {};
            renderTargetImageMemoryBarrier.SourceAccessMask = ACCESS_FLAGS_MEMORY_WRITE_BIT;
            renderTargetImageMemoryBarrier.DestinationAccessMask = ACCESS_FLAGS_MEMORY_READ_BIT;
            renderTargetImageMemoryBarrier.OldLayout = ImageLayout::UNDEFINED;
            renderTargetImageMemoryBarrier.NewLayout = ImageLayout::PRESENT_SRC_KHR;
            renderTargetImageMemoryBarrier.SourceQueueFamilyIndex = QueueFamilyIgnored;
            renderTargetImageMemoryBarrier.DestinationQueueFamilyIndex = QueueFamilyIgnored;
            renderTargetImageMemoryBarrier.Image = renderTarget->GetAsTexture();
            renderTargetImageMemoryBarrier.ImageSubresourceRange = {
                .AspectMask = offscreenRenderTarget->GetImageAspect(),
                .BaseMipLevel = 0,
                .LevelCount = 1,
                .BaseArrayLayer = 0,
                .LayerCount = 1
            };
            pipelineBarrier.ImageMemoryBarriers.push_back(viewportImageMemoryBarrier);
            pipelineBarrier.ImageMemoryBarriers.push_back(renderTargetImageMemoryBarrier);

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
        SharedFrameContext& frameContext = m_FrameContexts[m_CurrentFrameIndex];
        Mat4 m_SceneViewProjection = frameContext.SceneDescription.Camera.GetViewProjectionMatrix();

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
        PROFILE_SCOPE("SceneRendererImpl::ResizeViewport")

        m_ViewportSize = UVec2(width, height);
        m_ViewportResizedPublisher.PublishEvent(ViewportResizedEvent(width, height));

        Device& device = RendererAPI::GetDevice();
        Swapchain& swapchain = device.GetSwapchain();
        std::vector<RenderTargetHandle> renderTargets = swapchain.GetRenderTargets();
        device.WaitIdle();



        for (int i = 0; i < swapchain.GetNumberOfImages(); i++)
        {
            SharedFrameContext& frameContext = m_FrameContexts[i];

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
        m_CurrentViewportTexture = m_FrameContexts[nextFrameIndex].OffscreenDescriptorSet;

        // Resizing Render Graph

        std::vector<TextureHandle> outputTextures;
        constexpr int numFramesInFlight = 3;
        outputTextures.reserve(numFramesInFlight);
        for (int i = 0; i < numFramesInFlight; i++)
        {
            TextureHandle offscreenOutput = m_FrameContexts[i].OffscreenRenderTarget;
            outputTextures.push_back(offscreenOutput);
        }

        m_RenderGraph.ResizeResources(outputTextures[0]->GetDimensions());
    }


    UVec2 SceneRendererImpl::GetViewportSize()
    {
        return m_ViewportSize;
    }


    RendererType SceneRendererImpl::GetType() const
    {
        return m_RendererSettings.RendererType;
    }

} // Renderer