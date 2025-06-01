//
// Created by Andrew Fagan on 7/19/24.
//

#pragma once

#include <complex.h>

#include "Renderer/RHI/Resources/CommandBuffer.h"
#include "Renderer/RHI/Resources/VertexBuffer.h"

namespace Astral {

    class OpenGLVertexBuffer : public VertexBuffer
    {
    public:
        explicit OpenGLVertexBuffer(float* vertices, uint32 size, VertexBufferLayout bufferLayout);
        ~OpenGLVertexBuffer() override;

        void Bind(CommandBufferHandle commandBufferHandle) override;
        void Unbind() override;
        void SetLayout(VertexBufferLayout& bufferLayout) override;
        VertexBufferLayout& GetBufferLayout() override { return m_BufferLayout; }

        void* GetNativeHande() override { return &m_RendererID; }

    private:
        VertexBufferLayout m_BufferLayout;
        uint32 m_RendererID;
    };

}