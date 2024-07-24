//
// Created by Andrew Fagan on 7/19/24.
//

#pragma once


#include "Ayla/Renderer/ShaderProgram.h"


/*
* Holds all the data going into a vertex buffer.
* @note Expects to hold positions, indices, colors
*/
struct VertexBufferData
{
    float positions[8] = {
            -0.5f, -0.5,
            0.5f, -0.5f,
            -0.5f, 0.5f,
            0.5f, 0.5f
    };

    unsigned int indices[6] = {
            0, 1, 2,
            1, 3, 2
    };

    float colors[6][4] = {{1.0f, .4f, .5f, 1.0f},
                          {1.0f, .3f, .9f, 1.0f},
                          {1.0f, .1f, .2f, 1.0f},
                          {0.0f, .8f, .5f, 1.0f},
                          {0.2f, .6f, .2f, 1.0f},
                          {0.5f, 1.0f, .8f, 1.0f}};
};





namespace Ayla::Renderer {

    class IRenderer {
    public:

        /** Creates a shader program. */
        explicit IRenderer(const Shader& vertexShader, const Shader& fragmentShader);
        virtual ~IRenderer();

        /** Initializes the buffers and copies data into them. */
        virtual void InitBuffers() = 0;

        /** Makes draw calls using the VAOs. */
        virtual void Render() const = 0;

        virtual void GenerateBuffer(int numberOfBuffers, unsigned int* outBuffer) const = 0;
        virtual void BindBuffer(int typeOfBuffer, unsigned int bufferID) const = 0;
        virtual void CopyDataToBuffer(int typeOfBuffer, int64 sizeOfBuffer, const void* data, unsigned int usage) = 0;

        virtual void SetVertexAttribute(int attributeIndex, int size, unsigned int dataType, bool doNormalize, int stride, const void* pointerOffset) = 0;
        virtual void EnableVertexAttribute(int attributeIndex) = 0;
        virtual void DisableVertexAttribute(int attributeIndex) = 0;

    protected:
        ShaderProgram m_ShaderProgram;
        unsigned int m_VertexBufferID{};
        unsigned int m_IndexBufferObjectID{};
        int m_UniformLocation{};

        VertexBufferData m_VertexBufferData;
    };



} // namespace Ayla::Renderer