/**
* @file OpenGLRendererAPI.cpp
* @author Andrew Fagan
* @date 12/5/2024
*/
#include "OpenGLRendererAPI.h"

#include "glad/glad.h"

namespace Graphics {

    void Graphics::OpenGLRendererAPI::Clear()
    {
        glClear(GL_COLOR_BUFFER_BIT);
    }


    void Graphics::OpenGLRendererAPI::SetClearColor(float r, float g, float b, float a)
    {
        glClearColor(r, g, b, a);
    }


    void Graphics::OpenGLRendererAPI::DrawElements(Graphics::VertexArrayObject* vertexArrayObject)
    {
        vertexArrayObject->Bind();
        glDrawElements(GL_TRIANGLES, vertexArrayObject->GetElementCount(), GL_UNSIGNED_INT, nullptr);
    }


    void OpenGLRendererAPI::SetBlending(bool enable)
    {
        if (enable)
        {
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        }
        else
        {
            glDisable(GL_BLEND);
        }

    }

}