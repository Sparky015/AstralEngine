//
// Created by Andrew Fagan on 11/29/24.
//

#pragma once

#include "BufferLayout.h"

namespace Renderer {

    class VertexBuffer
    {
    public:
        virtual ~VertexBuffer() = default;
        virtual void Bind() = 0;
        virtual void Unbind() = 0;
        virtual void SetLayout(BufferLayout& bufferLayout) = 0;
        virtual BufferLayout& GetLayout() = 0;

        static VertexBuffer* CreateVertexBuffer(float* vertices, unsigned int size, const BufferLayout& bufferLayout);
    };

} // Renderer
