/**
* @file ForwardRenderer.h
* @author Andrew Fagan
* @date 6/29/25
*/

#pragma once

#include "Renderer/Common/Material.h"
#include "Renderer/Common/Mesh.h"
#include "Core/Events/EventPublisher.h"
#include "Renderer/Cameras/Camera.h"
#include "Renderer/RHI/RendererCommands.h"
#include "Renderer/RHI/Resources/Framebuffer.h"
#include "Renderer/RHI/Resources/Renderpass.h"
#include "Renderer/RHI/Resources/PipelineStateCache.h"
#include "Window/WindowEvents.h"
#include "Renderer/Renderer.h"
#include "Renderer/SceneRenderer.h"

#include <queue>


namespace Astral {

    struct SceneDescription;

    class ForwardRenderer : public Renderer
    {
    public:
        void Init() override;
        void Shutdown() override;

        void BeginScene(const SceneDescription& sceneDescription) override;
        void EndScene() override;

        void Submit(Mesh& mesh, Material& material, Mat4& transform) override;

        void SetRendererSettings(const RendererSettings& rendererSettings) override;
        const RendererSettings& GetRendererSettings() override;
        DescriptorSetHandle GetViewportTexture() override;
        void ResizeViewport(uint32 width, uint32 height) override;
        UVec2 GetViewportSize() override { return m_ViewportSize; }

        RendererType GetType() override { return RendererType::FORWARD; }

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
            TextureHandle OffscreenDepthBuffer;
            DescriptorSetHandle OffscreenDescriptorSet;

            CommandBufferHandle SceneCommandBuffer;
            FramebufferHandle SceneFramebuffer;
            RenderTargetHandle SceneRenderTarget;
            BufferHandle SceneDataBuffer;
            BufferHandle SceneLightsBuffer;
            DescriptorSetHandle SceneDataDescriptorSet;

            FramebufferHandle WindowFramebuffer;

            std::vector<DescriptorSetHandle> ImGuiTexturesToBeFreed;
            std::vector<TextureHandle> TexturesToBeFreed;
            uint32 FramesTillFree = 2;
        };


        void BuildRenderPasses();
        void InitializeFrameResources();

        void RenderScene();

        void ResizeImages(uint32 width, uint32 height);


        RendererSettings m_RendererSettings{};

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
    };

}
