//
// Created by Andrew Fagan on 7/19/24.
//

#include "OpenGLRenderer.h"

#include "Solas/Renderer/Abstractions/IndexBufferObject.h"
#include "Solas/Renderer/Abstractions/VertexArrayObject.h"
#include "Solas/Renderer/Abstractions/VertexBuffer.h"

#include "glad/glad.h"





namespace Solas::Renderer {

    OpenGLRenderer::OpenGLRenderer(const Shader& vertexShader, const Shader& fragmentShader) : IRenderer(vertexShader, fragmentShader)
    {
        TRACE("Initializing OpenGL Renderer!");

    }


    OpenGLRenderer::~OpenGLRenderer()
    {
        TRACE("Destroying OpenGL Renderer!");
    }


    void OpenGLRenderer::InitBuffers()
    {
        // TODO: Can't use static stuff here because you can't call gl functions after the window (includes open gl context) shuts down
         static VertexArrayObject vertexArrayObject = VertexArrayObject();


         static VertexBuffer vertexBuffer = VertexBuffer(m_VertexBufferData.positions, sizeof(m_VertexBufferData.positions));

        SetVertexAttribute(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
        EnableVertexAttribute(0);

        SetVertexAttribute(1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float),(void*)8);
        EnableVertexAttribute(1);


         static IndexBufferObject indexBufferObject = IndexBufferObject(m_VertexBufferData.indices,sizeof(m_VertexBufferData.indices));


        glUseProgram(m_ShaderProgram.GetID());

        m_UniformLocation = glGetUniformLocation(m_ShaderProgram.GetID(), "u_Color");
        AY_ASSERT(m_UniformLocation != -1, "Uniform not found!");
        glUniform4f(m_UniformLocation, 0.0f, 1.0f, 0.0f, 1.0f);

    }


    void OpenGLRenderer::Render() const
    {
        glClearColor(1, 0, 1, 1);
        glClear(GL_COLOR_BUFFER_BIT);


        glUniform4f(m_UniformLocation, 1, 0, 0, .5);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    }



    void OpenGLRenderer::GenerateBuffer(int numberOfBuffers, unsigned int* outBuffer) const
    {
        glGenBuffers(numberOfBuffers, outBuffer);
    }


    void OpenGLRenderer::BindBuffer(int typeOfBuffer, unsigned int bufferID) const
    {
        glBindBuffer(typeOfBuffer, bufferID);
    }


    void OpenGLRenderer::CopyDataToBuffer(int typeOfBuffer, int64_t sizeOfBuffer, const void* data, unsigned int usage)
    {
        glBufferData(typeOfBuffer, sizeOfBuffer, data, usage);
    }

    void OpenGLRenderer::SetVertexAttribute(int attributeIndex, int size, unsigned int dataType, bool doNormalize, int stride,
                                      const void* pointerOffset)
    {
        glVertexAttribPointer(attributeIndex, size, dataType, doNormalize, stride, pointerOffset);
    }


    void OpenGLRenderer::EnableVertexAttribute(int attributeIndex)
    {
        glEnableVertexAttribArray(attributeIndex);
    }


    void OpenGLRenderer::DisableVertexAttribute(int attributeIndex)
    {
        glDisableVertexAttribArray(attributeIndex);
    }

} // namespace Solas::Renderer