//
// Created by Andrew Fagan on 12/4/24.
//

#include "RendererAPI.h"

#include "Platform/OpenGL/OpenGLRendererCommands.h"
#include "Resources/VertexArrayObject.h"


namespace Astral {

    RendererCommands* RendererAPI::s_RendererCommands = new OpenGLRendererCommands();

    void RendererAPI::Clear()
    {
        s_RendererCommands->Clear();
    }


    void RendererAPI::SetClearColor(float r, float g, float b, float a)
    {
        s_RendererCommands->SetClearColor(r, g, b, a);
    }


    void RendererAPI::DrawElements(Shader& shaderProgram, VertexArrayObject* vertexArrayObject, Mat4& transform)
    {
        shaderProgram.SetUniform("u_Transform", transform);
        shaderProgram.Bind();
        s_RendererCommands->DrawElements(vertexArrayObject);
    }


    void RendererAPI::SetBlending(bool enable)
    {
        s_RendererCommands->SetBlending(enable);
    }

}
