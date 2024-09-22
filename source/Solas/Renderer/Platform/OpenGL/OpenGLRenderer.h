//
// Created by Andrew Fagan on 7/19/24.
//

#pragma once

#include "Solas/Renderer/Renderer.h"




namespace Ayla::Renderer {


    class OpenGLRenderer : public IRenderer {
    public:
        explicit OpenGLRenderer(const Shader& vertexShader, const Shader& fragmentShader);
        virtual ~OpenGLRenderer();


        /** Initializes the buffers and copies data into them. */
        virtual void InitBuffers() override;

        /** Makes draw calls using the VAOs. */
        virtual void Render() const override;

        virtual void GenerateBuffer(int numberOfBuffers, unsigned int* outBuffer) const override;
        virtual void BindBuffer(int typeOfBuffer, unsigned int bufferID) const override;
        virtual void CopyDataToBuffer(int typeOfBuffer, int64 sizeOfBuffer, const void* data, unsigned int usage) override;

        virtual void SetVertexAttribute(int attributeIndex, int size, unsigned int dataType, bool doNormalize, int stride, const void* pointerOffset) override;
        virtual void EnableVertexAttribute(int attributeIndex) override;
        virtual void DisableVertexAttribute(int attributeIndex) override;

    private:

    };



} // namespace Ayla::Renderer