//
// Created by Andrew Fagan on 11/29/24.
//

#pragma once

#include "Common/Material.h"
#include "Common/Mesh.h"
#include "Core/Events/EventPublisher.h"
#include "Renderer/RHI/Resources/VertexBuffer.h"
#include "Renderer/Cameras/OrthographicCamera.h"
#include "RHI/RendererCommands.h"
#include "RHI/Resources/Framebuffer.h"
#include "RHI/Resources/Renderpass.h"
#include "Window/WindowEvents.h"

#include <queue>

namespace Astral {

    struct SceneDescription
    {
        OrthographicCamera& Camera;

    };

    class SceneRenderer
    {
    public:
        static void Init();
        static void Shutdown();

        static void BeginScene(const SceneDescription& sceneDescription);
        static void EndScene();

        static void Submit(Mesh& mesh, Material& material, Mat4& transform);

        static TextureHandle GetViewportTexture();
        static void ResizeViewport(uint32 width, uint32 height);

        static uint32 GetDrawCallsPerFrame();
        static API GetRendererAPIBackend();

    private:

        static void RenderScene();

        static void ResizeImages(uint32 width, uint32 height);

        struct FrameContext
        {
            std::vector<Mesh> Meshes;
            std::vector<Material> Materials;
            std::vector<Mat4> Transforms;
            CommandBufferHandle SceneCommandBuffer;
            FramebufferHandle SceneFramebuffer;
            FramebufferHandle WindowFramebuffer;
            PipelineStateObjectHandle TempPipelineState;
            RenderTargetHandle SceneRenderTarget;
            BufferHandle SceneCameraBuffer;
            DescriptorSetHandle SceneCameraDescriptorSet;

            TextureHandle OffscreenRenderTarget;

            std::vector<TextureHandle> ImGuiTexturesToBeFreed;
        };

        struct SceneRendererContext
        {
            std::vector<FrameContext> FrameContexts;
            uint32 CurrentFrameIndex = -1;
            RenderPassHandle MainRenderPass;
            RenderPassHandle ImGuiRenderPass;
            EventListener<FramebufferResizedEvent> WindowResizedListener{[](FramebufferResizedEvent){}};
            EventPublisher<ViewportResizedEvent> ViewportResizedPublisher;
            bool IsSceneStarted = false;
            std::queue<TextureHandle> CurrentViewportTexture;

            UVec2 ViewportSize;
        };

        static GraphicsOwnedPtr<SceneRendererContext> m_RendererContext;
    };

} // Astral
