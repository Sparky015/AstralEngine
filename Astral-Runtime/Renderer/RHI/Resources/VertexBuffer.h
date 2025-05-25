//
// Created by Andrew Fagan on 11/29/24.
//

#pragma once

#include <complex.h>

#include "BufferLayout.h"
#include "CommandBuffer.h"
#include "Core/Math/Math.h"
#include "Renderer/RHI/Common/GraphicsSmartPointers.h"

namespace Astral {

    class VertexBuffer
    {
    public:
        struct Vertex
        {
            Vec2 position;
        };


        virtual ~VertexBuffer() = default;
        virtual void Bind(CommandBufferHandle commandBufferHandle) = 0;
        virtual void Unbind() {};
        virtual void SetLayout(BufferLayout& bufferLayout) {};
        //virtual BufferLayout& GetLayout() = 0

        virtual void* GetVerticeData() { return nullptr; }
        virtual uint32 GetSize() { return 0; }

        virtual void* GetNativeHande() = 0;

        static GraphicsRef<VertexBuffer> CreateVertexBuffer(float* vertices, unsigned int size,
                                                            const BufferLayout& bufferLayout);
    };

    using VertexBufferHandle = GraphicsRef<VertexBuffer>;

} // Renderer
