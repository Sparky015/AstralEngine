//
// Created by Andrew Fagan on 7/19/24.
//

#include "VertexArrayObject.h"
#include "IndexBufferObject.h"
#include "glad/glad.h"


namespace Solas::Renderer {


    VertexArrayObject::VertexArrayObject()
    {
        AY_LOG("Creating Vertex Array Object!");
        GLCheckError();
        glGenVertexArrays(1, &m_RendererID);
        Bind();
        GLCheckError();
    }


    VertexArrayObject::~VertexArrayObject()
    {
        AY_LOG("Deleting Vertex Array Object!");
        GLCheckError();
        glDeleteVertexArrays(1, &m_RendererID);
        GLCheckError();
    }


    void VertexArrayObject::Bind()
    {
        glBindVertexArray(m_RendererID);
    }


    void VertexArrayObject::Unbind()
    {
        glBindVertexArray(0);
    }




} // namespace Solas::Renderer