//
// Created by Andrew Fagan on 7/19/24.
//

#include "OpenGLVertexArrayObject.h"

#include "glad/glad.h"
#include "Renderer/Renderer.h" // For GLCheckError()

namespace Renderer {


    OpenGLVertexArrayObject::OpenGLVertexArrayObject()
    {
        LOG("Creating Vertex Array Object!");
        GLCheckError();
        glGenVertexArrays(1, &m_RendererID);
        Bind();
        GLCheckError();
    }


    OpenGLVertexArrayObject::~OpenGLVertexArrayObject()
    {
        LOG("Deleting Vertex Array Object!");
        GLCheckError();
        glDeleteVertexArrays(1, &m_RendererID);
        GLCheckError();
    }


    void OpenGLVertexArrayObject::Bind()
    {
        glBindVertexArray(m_RendererID);
    }


    void OpenGLVertexArrayObject::Unbind()
    {
        glBindVertexArray(0);
    }




} // namespace Renderer