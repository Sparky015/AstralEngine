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

//        glEnableVertexAttribArray(0);
//        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);

//        glEnableVertexAttribArray(1);
//        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

    // slot, number of elements, something, something, size of stride (total size of each element), offset
    void OpenGLVertexArrayObject::AddBuffer(VertexBuffer* vertexBuffer)
    {
        GLint vaoID, vboID = 0;
        glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &vaoID);
        glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &vboID);

        Bind();
        vertexBuffer->Bind();

        BufferLayout bufferLayout = vertexBuffer->GetLayout();

        for (const BufferAttribute& bufferAttribute : bufferLayout)
        {
            m_StrideSize += bufferAttribute.GetAttributeSize();
        }

        int64 offset = 0;
        for (const BufferAttribute& bufferAttribute : bufferLayout)
        {
            int attributeSize = bufferAttribute.GetAttributeSize();
            int numOfComponents = bufferAttribute.GetAttributeComponentCount();
            glEnableVertexAttribArray(m_AttributeCounter);
            glVertexAttribPointer(m_AttributeCounter, numOfComponents, GL_FLOAT, GL_FALSE, m_StrideSize, (void*)offset);
            offset += attributeSize;
            m_AttributeCounter++;
        }

        // Rebind the previous buffers
        glBindVertexArray(vaoID);
        glBindBuffer(GL_ARRAY_BUFFER, vboID);
    }


} // namespace Renderer