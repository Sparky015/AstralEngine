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
#include "RHI/Resources/PipelineStateCache.h"
#include "Window/WindowEvents.h"
#include "Renderer/RenderGraph/RenderGraph.h"
#include "ECS/Components/PointLightComponent.h"
#include "Renderer/Common/SceneRendererTypes.h"
#include "RenderPasses/CascadedShadowMapRenderPass.h"
#include "RenderPasses/DeferredGeometryRenderPass.h"
#include "RenderPasses/DeferredLightingRenderPass.h"
#include "RenderPasses/DepthRenderPass.h"
#include "RenderPasses/EnvironmentMapPass.h"
#include "RenderPasses/ForwardLightingRenderPass.h"
#include "RenderPasses/ToneMappingPass.h"

#include <queue>

namespace Astral {


    class SceneRendererImpl
    {
    public:
        ~SceneRendererImpl() = default;

        /**
         * @brief Initializes the scene renderer
         */
        void Init();

        /**
         * @brief Shuts down the scene renderer
         */
        void Shutdown();

        /**
         * @brief Marks the beginning of submitting objects to render
         * @param sceneDescription The description of the scene to render
         * @post   @ref EndScene is called to close object submission
         */
        void BeginScene(const SceneDescription& sceneDescription);

        /**
         * @brief Marks the end of scene object submission to renderer
         * @pre   @ref BeginScene is called first to initialize object submission
         */
        void EndScene();

        /**
         * @brief Submits an object to the renderer
         * @param mesh The mesh of the object
         * @param material The material of the object
         * @param transform The model transform of the object
         */
        void Submit(const Ref<Mesh>& mesh, const Ref<Material>& material, const Mat4& transform);

        /**
         * @brief Updates the renderer settings with the given renderer settings
         * @param rendererSettings The renderer settings to apply to the renderer
         */
        void SetRendererSettings(const RendererSettings& rendererSettings);

        /**
         * @brief Gets the current renderer settings
         * @return The current renderer settings
         */
        const RendererSettings& GetRendererSettings();

        /**
         * @brief Gets the viewport texture of the current frame
         * @return The viewport texture of the current frame
         */
        DescriptorSetHandle GetViewportTexture();

        /**
         * @brief Resizes the viewport being rendered to
         * @param width Width to set viewport to
         * @param height Height to set viewport to
         */
        void ResizeViewport(uint32 width, uint32 height);

        /**
         * @brief Gets the dimensions of the viewport
         * @return The dimensions of the viewport
         */
        UVec2 GetViewportSize() { return m_ViewportSize; }

        /**
         * @brief Gets the renderer path being used
         * @return The renderer path being used
         */
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
            uint32 NumShadowCascades;
        };

        void BuildRenderGraphForDeferred();
        void BuildRenderGraphForForward();
        void BuildImGuiEditorRenderPass();
        void InitializeFrameResources();

        void RenderScene();

        void ResizeWindowImages(uint32 width, uint32 height);
        void SetVSync(bool isVSyncEnabled);

        // Forward

        void ComputeEnvironmentIBL();
        void ComputeIrradianceMap(const CommandBufferHandle& commandBuffer);
        void ComputePrefilteredEnvironmentMap(const CommandBufferHandle& commandBuffer, uint32 mipLevel, UVec2 mipDimensions);

        // Editor
        void DrawEditorUI(CommandBufferHandle commandBuffer, RenderTargetHandle renderTarget);

        bool ShouldCullMesh(const Mesh& mesh, const Mat4& modelTransform); // Frustom Culling


        RendererSettings m_RendererSettings{};
        RenderGraph m_RenderGraph;

        std::vector<SharedFrameContext> m_FrameContexts;
        uint32 m_CurrentFrameIndex = -1;
        RenderPassHandle m_ImGuiRenderPass;
        EventListener<FramebufferResizedEvent> m_WindowResizedListener{[](FramebufferResizedEvent){}};
        EventPublisher<ViewportResizedEvent> m_ViewportResizedPublisher;
        bool m_IsSceneStarted = false;
        std::queue<DescriptorSetHandle> m_CurrentViewportTexture; // TODO: Remove queue and just make single instance that is nullable

        UVec2 m_ViewportSize{};

        // Deferred Geometry Pass
        DeferredGeometryRenderPass m_DeferredGeometryRenderPass;

        // Deferred Lighting Pass
        DeferredLightingRenderPass m_DeferredLightingRenderPass;

        // Forward Lighting Pass
        ForwardLightingRenderPass m_ForwardLightingRenderPass;

        // Depth Pre-Pass
        DepthRenderPass m_DepthRenderPass;

        // Cascaded Shadow Maps
        CascadedShadowMapRenderPass m_CascadedShadowMapRenderPass;

        // Environment Map Pass
        EnvironmentMapRenderPass m_EnvironmentMapRenderPass;
        DescriptorSetHandle m_EnvironmentMapStorageImagesSet; // Write access to environment map for other passes to populate environment map data

        // ACES Color Transforms and Tone Mapping
        ToneMappingRenderPass m_ToneMappingRenderPass;

        float m_SceneExposure{};
        Mat4 m_SceneViewProjection{};
        Camera m_SceneCamera;
    };

}
