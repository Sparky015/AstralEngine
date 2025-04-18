//
// Created by Andrew Fagan on 12/4/24.
//

#include "RendererCommands.h"

#include "Renderer/Platform/OpenGL/OpenGLRendererAPI.h"
#include "Renderer/Primitives/VertexArrayObject.h"


namespace Graphics {

    RendererAPI* RendererCommand::s_RendererAPI = new OpenGLRendererAPI();

    void RendererCommand::Clear()
    {
        s_RendererAPI->Clear();
    }


    void RendererCommand::SetClearColor(float r, float g, float b, float a)
    {
        s_RendererAPI->SetClearColor(r, g, b, a);
    }


    void RendererCommand::DrawElements(ShaderProgram& shaderProgram, VertexArrayObject* vertexArrayObject, Mat4& transform)
    {
        shaderProgram.SetUniform("u_Transform", transform);
        shaderProgram.Bind();
        s_RendererAPI->DrawElements(vertexArrayObject);
    }


    void RendererCommand::SetBlending(bool enable)
    {
        s_RendererAPI->SetBlending(enable);
    }

    uint32 RendererCommand::GetNumberOfDrawCallsPerFrame()
    {
        return s_RendererAPI->GetNumberOfDrawCalls();
    }
}
