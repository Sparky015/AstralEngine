/**
* @file AllocationData.h
* @author Andrew Fagan
* @date 2/10/2025
*/

#pragma once

#include "Core/CoreMacroDefinitions.h"
#include <thread>

namespace Core {

    enum class MemoryRegion : uint8
    {
        RENDERER,
        WINDOW,
        ASSETS,
        ECS,
        DEBUG,
        CORE,
        UNKNOWN,

        MEMORY_REGION_END
    };

    enum class AllocatorType : uint8
    {
        STACK,
        RING,
        FRAME,
        LINEAR,
        POOL,
        DOUBLE_BUFFERED,

        ALIGNED_ALLOCATOR,
        NEW_OPERATOR,

        ALLOCATOR_TYPE_END
    };

    struct AllocationData
    {
        void* pointer;
        size_t size;
        MemoryRegion region;
        AllocatorType allocatorType;
        std::thread::id threadID;
    };

}