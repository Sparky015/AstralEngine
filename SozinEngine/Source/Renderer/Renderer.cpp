//
// Created by Andrew Fagan on 11/29/24.
//

#include "Renderer.h"

#include "Renderer/RendererCommands.h"

namespace Renderer {

    void Renderer::Submit(VertexArrayObject* vertexArrayBuffer)
    {
        RendererCommand::DrawElements(vertexArrayBuffer);
    }

} // Renderer