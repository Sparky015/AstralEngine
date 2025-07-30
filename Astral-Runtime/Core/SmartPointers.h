/**
* @file SmartPointers.h
* @author Andrew Fagan
* @date 4/16/25
*/

#pragma once

#include <memory>

#include "Debug/MemoryTracking/GenericRegionAllocator.h"
#include "Asset/Ref.h"

namespace Astral {

    template <typename T>
    using ScopedPtr = std::unique_ptr<T>;

    template <typename T, class... Args>
    inline ScopedPtr<T> CreateScopedPtr(Args&&... args)
    {
        GenericRegionAllocator<T> allocator{MemoryRegion::CORE};
        T* pointer = allocator.allocate(1);
        allocator.construct(pointer, args...);

        // No need to provide custom deleter as unique_ptr will default to the overridden ::delete which tracks frees
        return std::unique_ptr<T>(pointer);
    }

}
