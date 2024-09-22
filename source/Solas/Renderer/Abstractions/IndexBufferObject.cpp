//
// Created by Andrew Fagan on 7/19/24.
//

#include "IndexBufferObject.h"

#include "glad/glad.h"

void GLCheckError()
{
    while (GLenum error = glGetError())
    {
    AY_WARN_SS("[OpenGL] Error Code: " << error);
    }
}


namespace Solas::Renderer {

    IndexBufferObject::IndexBufferObject(void* data, unsigned int count)
    {
        AY_LOG("Creating Index Buffer Object!");
        glGenBuffers(1, &m_RendererID);
        Bind();
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * count, data, GL_STATIC_DRAW);
    }


    IndexBufferObject::~IndexBufferObject()
    {
        AY_LOG("Deleting Index Buffer Object!");
        glDeleteBuffers(1, &m_RendererID);
    }


    void IndexBufferObject::Bind() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
    }


    void IndexBufferObject::Unbind()
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }


} // namespace Solas::Renderer