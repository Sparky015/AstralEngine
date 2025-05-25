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
        explicit OpenGLVertexBuffer(float* vertices, uint32 size, BufferLayout bufferLayout);
        ~OpenGLVertexBuffer() override;

        void Bind(CommandBufferHandle commandBufferHandle) override;
        void Unbind() override;
        void SetLayout(BufferLayout& bufferLayout) override;
        //BufferLayout& GetLayout() override;

        void* GetNativeHande() override { return &m_RendererID; }

    private:
        BufferLayout m_BufferLayout;
        uint32 m_RendererID;
    };

}