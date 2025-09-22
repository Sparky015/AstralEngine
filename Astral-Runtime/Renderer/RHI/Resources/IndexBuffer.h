//
// Created by Andrew Fagan on 11/29/24.
//

#pragma once

#include "Core/FixedIntegerTypes.h"
#include "Renderer/RHI/Common/GraphicsSmartPointers.h"
#include "Renderer/RHI/Common/MemoryTypes.h"

namespace Astral {

    class IndexBuffer
    {
    public:
        virtual ~IndexBuffer() = default;

        [[nodiscard]] virtual uint32 GetCount() const = 0;

        virtual void MapPointer(void** cpuPtr) = 0;
        virtual void UnmapPointer() = 0;
        virtual void CopyDataToBuffer(void* data, uint32 size) = 0;

        virtual void* GetNativeHandle() = 0;

        static GraphicsRef<IndexBuffer> CreateIndexBuffer(uint32* data, uint32 count, GPUMemoryType memoryType = GPUMemoryType::DEVICE_LOCAL);
    };

    using IndexBufferHandle = GraphicsRef<IndexBuffer>;

}
