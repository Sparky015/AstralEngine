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
        void Submit(Mesh& mesh, Material& material, Mat4& transform);

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

            BufferHandle ShadowLightMatrices;
            DescriptorSetHandle ShadowLightMatricesDescriptorSet;

            bool IsEnvironmentMapIBLCalculationNeeded;
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

        void ComputeEnvironmentIBL();
        void ComputeIrradianceMap(const CommandBufferHandle& commandBuffer);
        void ComputePrefilteredEnvironmentMap(const CommandBufferHandle& commandBuffer, uint32 mipLevel, UVec2 mipDimensions);

        // Editor
        void DrawEditorUI(CommandBufferHandle commandBuffer, RenderTargetHandle renderTarget);

        float CalcCascadeZFar(float zNear, float zFar, float cascadeNum, float totalCascades); // CSM Helper
        bool ShouldCullMesh(const Mesh& mesh, const Mat4& modelTransform); // Frustom Culling



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

        // Depth Pre-Pass & Cascaded Shadow Maps
        ShaderHandle m_DepthWriteOnlyShader;

        // Cascaded Shadow Maps
        ShaderHandle m_ShadowMapShader;

        // Environment Map Pass
        DescriptorSetHandle m_EnvironmentMapStorageImagesSet;

        // ACES Color Transforms and Tone Mapping
        DescriptorSetHandle m_RTT_ODT_LUT_DescriptorSet;

        // Cascaded Shadow Maps Pass
        Light m_FirstDirectionalLightInScene = {};
        std::vector<Mat4> m_LightSpaceMatrices{};


        float m_SceneExposure{};
        Mat4 m_SceneViewProjection{};
        Camera m_SceneCamera;
    };

}
