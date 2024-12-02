//
// Created by Andrew Fagan on 7/19/24.
//

#include "OpenGLIndexBuffer.h"

#include "glad/glad.h"
#include "Renderer/Renderer.h"

namespace Renderer {

    OpenGLIndexBuffer::OpenGLIndexBuffer(uint32* indices, uint32 count) : m_Count(count), m_RendererID(0)
    {
        LOG("Creating Index Buffer Object!");
        glGenBuffers(1, &m_RendererID);
        Bind();
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32) * count, indices, GL_STATIC_DRAW);
        GLCheckError();
    }


    OpenGLIndexBuffer::~OpenGLIndexBuffer()
    {
        LOG("Deleting Index Buffer Object!");
        glDeleteBuffers(1, &m_RendererID);
        GLCheckError();
    }


    void OpenGLIndexBuffer::Bind() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
    }


    void OpenGLIndexBuffer::Unbind() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }


    uint32 OpenGLIndexBuffer::GetCount() const
    {
        return m_Count;
    }


} // namespace Renderer