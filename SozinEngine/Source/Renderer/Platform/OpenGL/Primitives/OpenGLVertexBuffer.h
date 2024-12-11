//
// Created by Andrew Fagan on 7/19/24.
//

#pragma once

#include "Renderer/Primitives/VertexBuffer.h"

namespace Graphics {

    class OpenGLVertexBuffer : public VertexBuffer
    {
    public:
        explicit OpenGLVertexBuffer(float* vertices, unsigned int size, BufferLayout bufferLayout);
        ~OpenGLVertexBuffer() override;

        void Bind() override;
        void Unbind() override;
        void SetLayout(BufferLayout& bufferLayout) override;
        BufferLayout& GetLayout() override;

    private:
        BufferLayout m_BufferLayout;
        unsigned int m_RendererID;
    };

}