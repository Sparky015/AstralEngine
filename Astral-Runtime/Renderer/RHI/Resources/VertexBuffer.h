//
// Created by Andrew Fagan on 11/29/24.
//

#pragma once

#include "BufferLayout.h"
#include "Renderer/RHI/Common/GraphicsSmartPointers.h"

namespace Astral {

    class VertexBuffer
    {
    public:
        virtual ~VertexBuffer() = default;
        virtual void Bind() {};
        virtual void Unbind() {};
        virtual void SetLayout(BufferLayout& bufferLayout) {};
        //virtual BufferLayout& GetLayout() = 0

        virtual void* GetVerticeData() { return nullptr; }
        virtual uint32 GetSize() { return 0; }

        virtual void* GetNativeHande() = 0;

        static VertexBuffer* CreateVertexBuffer(float* vertices, unsigned int size, const BufferLayout& bufferLayout);
    };

    using VertexBufferHandle = GraphicsRef<VertexBuffer>;

} // Renderer
