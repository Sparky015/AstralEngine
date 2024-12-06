//
// Created by Andrew Fagan on 12/4/24.
//

#include "RendererCommands.h"

#include "glad/glad.h"
#include "Renderer/Primitives/VertexArrayObject.h"
#include "Renderer/Platform/OpenGL/OpenGLRendererAPI.h"

namespace Renderer {

    RendererAPI* RendererCommand::s_RendererAPI = new OpenGLRendererAPI();

    void RendererCommand::Clear()
    {
        s_RendererAPI->Clear();
    }

    void RendererCommand::SetClearColor(float r, float g, float b, float a)
    {
        s_RendererAPI->SetClearColor(r, g, b, a);
    }

    void RendererCommand::DrawElements(VertexArrayObject* vertexArrayObject)
    {
        s_RendererAPI->DrawElements(vertexArrayObject);
    }

}