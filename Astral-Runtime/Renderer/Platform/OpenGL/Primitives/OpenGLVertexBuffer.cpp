//
// Created by Andrew Fagan on 7/19/24.
//

#include "OpenGLVertexBuffer.h"

#include <utility>

#include "Debug/Utilities/Loggers.h"
#include "glad/glad.h"
#include "Renderer/Platform/OpenGL/OpenGLErrorTracking.h"


namespace Astral {

    OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, unsigned int size, BufferLayout bufferLayout) : m_BufferLayout(std::move(bufferLayout)), m_RendererID(0)
    {
        glGenBuffers(1, &m_RendererID);
        Bind();
        glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
        Unbind();
        GLCheckError();
    }


    OpenGLVertexBuffer::~OpenGLVertexBuffer()
    {
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


    void OpenGLVertexBuffer::SetLayout(BufferLayout& bufferLayout)
    {
        m_BufferLayout = bufferLayout;
    }


    BufferLayout& OpenGLVertexBuffer::GetLayout()
    {
        return m_BufferLayout;
    }


} // namespace Renderer
