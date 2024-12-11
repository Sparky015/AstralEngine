//
// Created by Andrew Fagan on 11/29/24.
//

#include "Renderer.h"

#include "Renderer/RendererCommands.h"

namespace Graphics {

    void Renderer::Submit(ShaderProgram& shaderProgram, VertexArrayObject* vertexArrayBuffer, Mat4& transform)
    {
        RendererCommand::DrawElements(shaderProgram, vertexArrayBuffer, transform);
    }

} // Renderer