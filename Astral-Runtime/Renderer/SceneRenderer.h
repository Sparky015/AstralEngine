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

namespace Astral {

    class SceneRenderer
    {
    public:
        static void BeginScene(const OrthographicCamera& orthographicCamera);
        static void EndScene();

        static void Submit(Mesh& mesh, Material& material, Mat4& transform);

        static uint32 GetDrawCallsPerFrame();
        static API GetRendererAPIBackend();

        static void TestInit();
        static void TestShutdown();
        static void TestUpdate();

    private:

        struct SceneRendererContext
        {
            CommandBufferHandle SceneCommandBuffer;
        };

        static SceneRendererContext m_RendererContext;
    };

} // Renderer
