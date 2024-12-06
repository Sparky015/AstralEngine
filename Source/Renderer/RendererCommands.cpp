//
// Created by Andrew Fagan on 12/4/24.
//

#include "RendererCommands.h"

#include "glad/glad.h"
#include "Renderer/Primitives/VertexArrayObject.h"

namespace Renderer {

    void RendererCommand::Clear()
    {
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void RendererCommand::SetClearColor(float r, float g, float b, float a)
    {
        glClearColor(r, g, b, a);
    }

    void RendererCommand::DrawElements(VertexArrayObject* vertexArrayObject)
    {
        vertexArrayObject->Bind();
        glDrawElements(GL_TRIANGLES, vertexArrayObject->GetElementCount(), GL_UNSIGNED_INT, nullptr);
    }

}