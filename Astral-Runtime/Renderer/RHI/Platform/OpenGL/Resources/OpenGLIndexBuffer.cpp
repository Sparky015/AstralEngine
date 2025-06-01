//
// Created by Andrew Fagan on 7/19/24.
//

#include "OpenGLIndexBuffer.h"

#include <complex.h>

#include "Debug/Utilities/Loggers.h"
#include "glad/glad.h"
#include "Renderer/RHI/Platform/OpenGL/OpenGLErrorTracking.h"
#include "Renderer/RHI/Resources/CommandBuffer.h"

namespace Astral {

    OpenGLIndexBuffer::OpenGLIndexBuffer(uint32* indices, uint32 count) : m_RendererID(0), m_Count(count)
    {
        LOG("Creating Index Buffer Object!");
        glGenBuffers(1, &m_RendererID);
        // Bind(TODO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32) * count, indices, GL_STATIC_DRAW);
        Unbind();
        GLCheckError();
    }


    OpenGLIndexBuffer::~OpenGLIndexBuffer()
    {
        LOG("Deleting Index Buffer Object!");
        glDeleteBuffers(1, &m_RendererID);
        GLCheckError();
    }


    void OpenGLIndexBuffer::Bind(CommandBufferHandle commandBufferHandle)
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