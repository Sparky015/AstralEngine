//
// Created by Andrew Fagan on 11/29/24.
//

#include "SceneRenderer.h"

#include "RHI/RendererAPI.h"

namespace Astral {

    void SceneRenderer::BeginScene(const OrthographicCamera& orthographicCamera)
    {

    }

    void SceneRenderer::EndScene()
    {

    }


    void SceneRenderer::Submit(Shader& shaderProgram, VertexArrayObject* vertexArrayBuffer, Mat4& transform)
    {
        RendererAPI::DrawElements(shaderProgram, vertexArrayBuffer, transform);
    }


    uint32 SceneRenderer::GetDrawCallsPerFrame()
    {
        return RendererAPI::GetRendererAPIBackend()->GetNumberOfDrawCalls();
    }


    API SceneRenderer::GetRendererAPIBackend()
    {
        return RendererAPI::GetRendererAPIBackend()->GetAPI();
    }

} // Renderer