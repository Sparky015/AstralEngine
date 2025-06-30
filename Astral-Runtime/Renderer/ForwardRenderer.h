/**
* @file ForwardRenderer.h
* @author Andrew Fagan
* @date 6/29/25
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

#include <queue>


namespace Astral {

    struct SceneDescription;

    class ForwardRenderer
    {
    public:
        void Init();
        void Shutdown();

        void BeginScene(const SceneDescription& sceneDescription);
        void EndScene();

        void Submit(Mesh& mesh, Material& material, Mat4& transform);

        DescriptorSetHandle GetViewportTexture();
        void ResizeViewport(uint32 width, uint32 height);
        UVec2 GetViewportSize() { return m_ViewportSize; }

        RendererDebugStats GetRendererDebugStats();

        API GetRendererAPIBackend();

    private:

        struct SceneData
        {
            Mat4 CameraViewProjection;
            alignas(16) Vec3 CameraPosition;
            alignas(4) uint32 NumLights;
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