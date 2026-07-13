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
#include "RenderingPaths/DeferredRenderingPath.h"
#include "RenderingPaths/ForwardRenderingPath.h"

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
        void BeginSceneSubmission(const SceneDescription& sceneDescription);

        /**
         * @brief Submits an object to the renderer
         * @param mesh The mesh of the object
         * @param material The material of the object
         * @param transform The model transform of the object
         */
        void Submit(const Ref<Mesh>& mesh, const Ref<Material>& material, const Mat4& transform);

        /**
         * @brief Marks the end of scene object submission to renderer
         * @pre   @ref BeginScene is called first to initialize object submission
         */
        void EndSceneSubmission();

        /**
         * @brief Renders submitted objects
         * @pre   @ref EndSceneSubmission is called first to end object submission
         */
        void RenderScene();

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
        ImTextureID GetViewportTexture();

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
        UVec2 GetViewportSize();

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

        void BuildRenderGraph();
        void BuildImGuiEditorRenderPass();
        void InitializeFrameResources();

        void ResizeWindowImages(uint32 width, uint32 height);
        void SetVSync(bool isVSyncEnabled);


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
        DescriptorSetHandle m_CurrentViewportTexture;

        UVec2 m_ViewportSize{};

        DeferredRenderingPath m_DeferredRendererPath{};
        ForwardRenderingPath m_ForwardRendererPath{};

        // Environment Map Pass
        DescriptorSetHandle m_EnvironmentMapStorageImagesSet; // Write access to environment map for other passes to populate environment map data
    };

}
