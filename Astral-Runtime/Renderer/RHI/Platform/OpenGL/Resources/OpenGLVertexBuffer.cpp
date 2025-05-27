//
// Created by Andrew Fagan on 7/19/24.
//

#include "OpenGLVertexBuffer.h"

#include <complex.h>
#include <utility>

#include "Debug/Utilities/Loggers.h"
#include "glad/glad.h"
#include "Renderer/RHI/Platform/OpenGL/OpenGLErrorTracking.h"
#include "Renderer/RHI/Resources/CommandBuffer.h"


namespace Astral {

    OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, unsigned int size, VertexBufferLayout bufferLayout) : m_BufferLayout(std::move(bufferLayout)), m_RendererID(0)
    {
        glGenBuffers(1, &m_RendererID);
        // Bind(TODO);
        glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
        Unbind();
        GLCheckError();
    }


    OpenGLVertexBuffer::~OpenGLVertexBuffer()
    {
        glDeleteBuffers(1, &m_RendererID);
        GLCheckError();
    }


    void OpenGLVertexBuffer::Bind(CommandBufferHandle commandBufferHandle)
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
    }


    void OpenGLVertexBuffer::Unbind()
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }


    void OpenGLVertexBuffer::SetLayout(VertexBufferLayout& bufferLayout)
    {
        m_BufferLayout = bufferLayout;
    }


    // BufferLayout& OpenGLVertexBuffer::GetLayout()
    // {
    //     return m_BufferLayout;
    // }


} // namespace Renderer
