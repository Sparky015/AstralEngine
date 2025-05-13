//
// Created by Andrew Fagan on 11/29/24.
//

#include "Renderer.h"

#include "RHI/RendererCommands.h"

namespace Astral {

    void Renderer::BeginScene(const OrthographicCamera& orthographicCamera)
    {

    }

    void Renderer::EndScene()
    {

    }

    void Renderer::Submit(Shader& shaderProgram, VertexArrayObject* vertexArrayBuffer, Mat4& transform)
    {
        RendererCommand::DrawElements(shaderProgram, vertexArrayBuffer, transform);
    }

    uint32 Renderer::GetDrawCallsPerFrame()
    {
        return RendererCommand::GetRendererAPIBackend()->GetNumberOfDrawCalls();
    }

    API Renderer::GetRendererAPIBackend()
    {
        return RendererCommand::GetRendererAPIBackend()->GetAPI();
    }

} // Renderer