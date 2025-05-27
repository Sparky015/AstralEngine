//
// Created by Andrew Fagan on 11/29/24.
//

#pragma once

#include "Common/Material.h"
#include "Common/Mesh.h"
#include "Renderer/RHI/Resources/IndexBuffer.h"
#include "Renderer/RHI/Resources/VertexBuffer.h"

#include "Renderer/RHI/Resources/VertexArrayObject.h"
#include "Renderer/RHI/Resources/Shader.h"
#include "Renderer/Cameras/OrthographicCamera.h"
#include "RHI/RendererCommands.h"
#include "RHI/Platform/OpenGL/OpenGLRendererContext.h"
#include "RHI/Resources/Framebuffer.h"

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


        static void TestUpdate();

    private:

        static void RenderScene();

        struct FrameContext
        {
            std::vector<Mesh> Meshes;
            std::vector<Material> Materials;
            std::vector<Mat4> Transforms;
            CommandBufferHandle SceneCommandBuffer;
            FramebufferHandle SceneFramebuffer;
            PipelineStateObjectHandle TempPipelineState;
            std::vector<DescriptorSetHandle> TempDescriptorSets;
        };

        struct SceneRendererContext
        {
            std::vector<FrameContext> FrameContexts;
            uint32 CurrentFrameIndex = -1;
            RenderPassHandle RenderPass;

            bool IsSceneStarted = false;
        };

        static GraphicsOwnedPtr<SceneRendererContext> m_RendererContext;
    };

} // Astral
