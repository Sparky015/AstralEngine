//
// Created by Andrew Fagan on 11/29/24.
//

#pragma once

#include "Common/Material.h"
#include "Common/Mesh.h"
#include "Renderer/RHI/Resources/VertexBuffer.h"
#include "Renderer/Cameras/OrthographicCamera.h"
#include "RHI/RendererCommands.h"
#include "RHI/Resources/Framebuffer.h"
#include "RHI/Resources/Renderpass.h"
#include "Window/WindowEvents.h"

namespace Astral {

    class SceneRenderer
    {
    public:
        static void Init();
        static void Shutdown();

        static void BeginScene(const OrthographicCamera& orthographicCamera);
        static void EndScene();

        static void Submit(Mesh& mesh, Material& material, Mat4& transform);

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
            PipelineStateObjectHandle TempPipelineState;
            std::vector<DescriptorSetHandle> TempDescriptorSets;
            RenderTargetHandle SceneRenderTarget;
        };

        struct SceneRendererContext
        {
            std::vector<FrameContext> FrameContexts;
            uint32 CurrentFrameIndex = -1;
            RenderPassHandle RenderPass;
            EventListener<FramebufferResizedEvent> WindowResizedListener{[](FramebufferResizedEvent){}};
            bool IsSceneStarted = false;
        };

        static GraphicsOwnedPtr<SceneRendererContext> m_RendererContext;
    };

} // Astral
