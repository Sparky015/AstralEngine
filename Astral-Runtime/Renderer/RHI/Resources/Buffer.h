/**
* @file Buffer.h
* @author Andrew Fagan
* @date 5/13/25
*/

#pragma once

#include "Core/FixedIntegerTypes.h"
#include "Renderer/RHI/Common/GraphicsSmartPointers.h"

namespace Astral {

    class Buffer
    {
    public:
        virtual ~Buffer() = default;

        virtual uint32 GetAllocatedSize() = 0;
        virtual uint32 GetUsedSize() = 0;
        virtual void MapPointer(void** cpuPtr) = 0;
        virtual void UnmapPointer() = 0;
        virtual void CopyDataToBuffer(void* data, uint32 size) = 0;

        virtual void* GetNativeHandle() = 0;

        static Buffer* CreateBuffer();
    };

    using BufferHandle = GraphicsRef<Buffer>;

}
