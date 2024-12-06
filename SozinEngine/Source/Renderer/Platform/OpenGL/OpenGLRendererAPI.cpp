/**
* @file OpenGLRendererAPI.cpp
* @author Andrew Fagan
* @date 12/5/2024
*/
#include "OpenGLRendererAPI.h"

#include "glad/glad.h"

namespace Renderer {

    void Renderer::OpenGLRendererAPI::Clear()
    {
        glClear(GL_COLOR_BUFFER_BIT);
    }


    void Renderer::OpenGLRendererAPI::SetClearColor(float r, float g, float b, float a)
    {
        glClearColor(r, g, b, a);
    }


    void Renderer::OpenGLRendererAPI::DrawElements(Renderer::VertexArrayObject* vertexArrayObject)
    {
        vertexArrayObject->Bind();
        glDrawElements(GL_TRIANGLES, vertexArrayObject->GetElementCount(), GL_UNSIGNED_INT, nullptr);
    }

}