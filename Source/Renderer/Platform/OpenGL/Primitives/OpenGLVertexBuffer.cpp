//
// Created by Andrew Fagan on 7/19/24.
//

#include "OpenGLVertexBuffer.h"

#include "glad/glad.h"
#include "Renderer/Renderer.h"


namespace Renderer {

    OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, unsigned int size)
    {
        LOG("Creating Vertex Buffer!");
        glGenBuffers(1, &m_RendererID);
        Bind();
        glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
        GLCheckError();
    }


    OpenGLVertexBuffer::~OpenGLVertexBuffer()
    {
        LOG("Deleting Vertex Buffer!");
        glDeleteBuffers(1, &m_RendererID);
        GLCheckError();
    }


    void OpenGLVertexBuffer::Bind()
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
    }


    void OpenGLVertexBuffer::Unbind()
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

} // namespace Renderer
