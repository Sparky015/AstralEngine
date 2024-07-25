//
// Created by Andrew Fagan on 7/19/24.
//

#include "VertexBuffer.h"

#include "glad/glad.h"


namespace Ayla::Renderer {

    VertexBuffer::VertexBuffer(void* data, unsigned int size)
    {
        AY_LOG("Creating Vertex Buffer!");
        glGenBuffers(1, &m_RendererID);
        Bind();
        glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    }


    VertexBuffer::~VertexBuffer()
    {
        AY_LOG("Deleting Vertex Buffer!");
        glDeleteBuffers(1, &m_RendererID);
    }


    void VertexBuffer::Bind()
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
    }


    void VertexBuffer::Unbind()
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

} // namespace Ayla::Renderer
