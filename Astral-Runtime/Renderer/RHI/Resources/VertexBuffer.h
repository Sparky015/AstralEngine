//
// Created by Andrew Fagan on 11/29/24.
//

#pragma once

#include <complex.h>

#include "Buffer.h"
#include "VertexBufferLayout.h"
#include "Core/Math/Math.h"
#include "Renderer/RHI/Common/GraphicsSmartPointers.h"

namespace Astral {

    class VertexBuffer
    {
    public:

        virtual ~VertexBuffer() = default;
        virtual VertexBufferLayout& GetBufferLayout() = 0;

        virtual void* GetVerticeData() = 0;
        virtual uint32 GetSize() = 0;

        virtual void MapPointer(void** cpuPtr) = 0;
        virtual void UnmapPointer() = 0;
        virtual void CopyDataToBuffer(void* data, uint32 size) = 0;

        virtual void* GetNativeHandle() = 0;

        static GraphicsRef<VertexBuffer> CreateVertexBuffer(float* vertices, unsigned int size, VertexBufferLayout& bufferLayout);
    };

    using VertexBufferHandle = GraphicsRef<VertexBuffer>;

} // Renderer
