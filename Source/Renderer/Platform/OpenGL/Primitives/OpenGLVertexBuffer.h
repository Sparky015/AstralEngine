//
// Created by Andrew Fagan on 7/19/24.
//

#pragma once

#include "Renderer/Primitives/VertexBuffer.h"

namespace Renderer {

    class OpenGLVertexBuffer : public VertexBuffer
    {
    public:
        explicit OpenGLVertexBuffer(float* vertices, unsigned int size);
        ~OpenGLVertexBuffer() override;

        void Bind() override;
        void Unbind() override;

    private:
        unsigned int m_RendererID;
    };

}