/**
* @file DeferredRenderer.h
* @author Andrew Fagan
* @date 7/1/2025
*/


#pragma once

#include "../Common/Material.h"
#include "../Common/Mesh.h"
#include "Core/Events/EventPublisher.h"
#include "Renderer/Cameras/Camera.h"
#include "../RHI/RendererCommands.h"
#include "../RHI/Resources/Framebuffer.h"
#include "../RHI/Resources/Renderpass.h"
#include "../RHI/Resources/PipelineStateCache.h"
#include "Window/WindowEvents.h"
#include "Renderer/Renderer.h"
#include "Renderer/RenderGraph/RenderGraph.h"

#include <queue>

namespace Astral {

    struct SceneDescription; // Forward declared

    class DeferredRenderer : public Renderer
    {
    public:
        ~DeferredRenderer() override = default;

        void Init() override;
        void Shutdown() override;

        void BeginScene(const SceneDescription& sceneDescription) override;
        void EndScene() override;

        void Submit(Mesh& mesh, Material& material, Mat4& transform) override;

        DescriptorSetHandle GetViewportTexture() override;
        void ResizeViewport(uint32 width, uint32 height) override;
        UVec2 GetViewportSize() override { return m_ViewportSize; }

        RendererType GetType() override { return RendererType::DEFERRED; }

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
        };

        void BuildRenderGraph();
        void BuildImGuiEditorRenderPass();
        void InitializeFrameResources();

        void RenderScene();

        void ResizeImages(uint32 width, uint32 height);

        void GeometryPass();
        void LightingPass();
        void CubemapPass();


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

        ShaderHandle m_GeometryPassShader;
        ShaderHandle m_LightingShader;
        DescriptorSetHandle m_Cubemap;
    };

}
