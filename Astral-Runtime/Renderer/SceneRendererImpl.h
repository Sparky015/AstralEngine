/**
* @file SceneRendererImpl.h
* @author Andrew Fagan
* @date 7/1/2025
*/


#pragma once

#include "Common/Material.h"
#include "Common/Mesh.h"
#include "Core/Events/EventPublisher.h"
#include "Renderer/Cameras/Camera.h"
#include "RHI/RendererCommands.h"
#include "RHI/Resources/Framebuffer.h"
#include "RHI/Resources/Renderpass.h"
#include "RHI/Resources/PipelineStateCache.h"
#include "Window/WindowEvents.h"
#include "Renderer/RenderGraph/RenderGraph.h"
#include "ECS/Components/PointLightComponent.h"
#include "Renderer/Common/SceneRendererTypes.h"

#include <queue>



namespace Astral {


    class SceneRendererImpl
    {
    public:
        ~SceneRendererImpl() = default;

        void Init();
        void Shutdown();

        void BeginScene(const SceneDescription& sceneDescription);
        void EndScene();

        void Submit(Mesh& mesh, Material& material, Mat4& transform);

        void SetRendererSettings(const RendererSettings& rendererSettings);
        const RendererSettings& GetRendererSettings();
        DescriptorSetHandle GetViewportTexture();
        void ResizeViewport(uint32 width, uint32 height);
        UVec2 GetViewportSize() { return m_ViewportSize; }

        RendererType GetType() const;

    private:

        struct SceneData
        {
            Mat4 CameraViewProjection;
            Mat4 CameraView;
            Mat4 CameraProjection;
            Mat4 CameraInverseViewMat;
            Mat4 CameraInverseProjectionMat;
            Vec2 ScreenSize;
            alignas(16) Vec3 CameraPosition;
            uint32 NumLights;
            float AmbientLightConstant;
        };


        struct FrameContext
        {
            std::vector<Mesh> Meshes;
            std::vector<Material> Materials;
            std::vector<Mat4> Transforms;


            TextureHandle OffscreenRenderTarget;
            DescriptorSetHandle OffscreenDescriptorSet;

            CommandBufferHandle SceneCommandBuffer;
            RenderTargetHandle SceneRenderTarget;
            BufferHandle SceneDataBuffer;
            BufferHandle SceneLightsBuffer;
            DescriptorSetHandle SceneDataDescriptorSet;

            FramebufferHandle WindowFramebuffer;

            Ref<EnvironmentMap> EnvironmentMap;
            DescriptorSetHandle EnvironmentMapDescriptorSet;

            BufferHandle ForwardShadowLightMatrices;
            DescriptorSetHandle ForwardShadowLightMatricesDescriptorSet;

            bool IsIrradianceMapCalculationNeeded;
        };

        void BuildRenderGraphForDeferred();
        void BuildRenderGraphForForward();
        void BuildImGuiEditorRenderPass();
        void InitializeFrameResources();

        void RenderScene();

        void ResizeWindowImages(uint32 width, uint32 height);
        void SetVSync(bool isVSyncEnabled);

        // Forward
        void DepthPrePass();
        void ForwardLightingPass();
        void MSAAEnvironmentPass();

        // Deferred
        void GeometryPass();
        void DeferredLightingPass();

        // Both
        void CascadedShadowMapsPass();
        void EnvironmentMapPass();
        void ToneMappingPass();
        void FXAAPass();

        void ComputeIrradianceMap(const CommandBufferHandle& commandBuffer);
        void ComputePrefilteredEnvironmentMap(const CommandBufferHandle& commandBuffer, uint32 mipLevel, UVec2 mipDimensions);

        bool ShouldCullMesh(const Mesh& mesh, const Mat4& modelTransform);


        RendererSettings m_RendererSettings{};
        RenderGraph m_RenderGraph;

        std::vector<FrameContext> m_FrameContexts;
        uint32 m_CurrentFrameIndex = -1;
        RenderPassHandle m_MainRenderPass;
        RenderPassHandle m_ImGuiRenderPass;
        EventListener<FramebufferResizedEvent> m_WindowResizedListener{[](FramebufferResizedEvent){}};
        EventPublisher<ViewportResizedEvent> m_ViewportResizedPublisher;
        bool m_IsSceneStarted = false;
        std::queue<DescriptorSetHandle> m_CurrentViewportTexture; // TODO: Remove queue and just make single instance that is nullable

        UVec2 m_ViewportSize{};
        PipelineStateCache m_PipelineStateCache;

        // Deferred Geometry Pass
        ShaderHandle m_DeferredGeometryPassUnpackedShader;
        ShaderHandle m_DeferredGeometryPassORMShader;

        // Deferred Lighting Pass
        ShaderHandle m_DeferredLightingShader;

        // Forward Lighting Pass
        ShaderHandle m_ForwardUnpackedLightingShader;
        ShaderHandle m_ForwardORMLightingShader;

        // Depth Pre-Pass
        ShaderHandle m_DepthWriteOnlyShader;

        // Environment Map Pass
        DescriptorSetHandle m_EnvironmentMapStorageImagesSet;

        // ACES Color Transforms and Tone Mapping
        DescriptorSetHandle m_RTT_ODT_LUT_DescriptorSet;

        // Cascaded Shadow Maps Pass
        Light m_FirstDirectionalLightInScene = {};
        Mat4 m_LightSpaceMatrix{};


        float m_SceneExposure{};
        Mat4 m_SceneViewProjection{};
    };

}
