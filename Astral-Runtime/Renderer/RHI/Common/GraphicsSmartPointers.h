/**
* @file GraphicsSmartPointers.h
* @author Andrew Fagan
* @date 5/15/2025
*/

#pragma once

#include <memory>

#include "Debug/MemoryTracking/GenericRegionAllocator.h"

namespace Astral {

    template <typename T>
    using GraphicsRef = std::shared_ptr<T>;

    template <typename T, class... Args>
    inline GraphicsRef<T> CreateGraphicsRef(Args&&... args)
    {
        return std::allocate_shared<T>(GenericRegionAllocator<T>{MemoryRegion::RENDERER}, std::forward<Args>(args)...);
    }

    template <typename T>
    using GraphicsWeakRef = std::weak_ptr<T>;

    template <typename T>
    inline GraphicsWeakRef<T> CreateGraphicsWeakRef(GraphicsRef<T> ref) { return std::weak_ptr<T>(ref); }

    template <typename T>
    using GraphicsOwnedPtr = std::unique_ptr<T>;

    template <typename T, class... Args>
    inline GraphicsOwnedPtr<T> CreateGraphicsOwnedPtr(Args&&... args)
    {
        GenericRegionAllocator<T> allocator{MemoryRegion::RENDERER};
        T* pointer = allocator.allocate(1);
        allocator.construct(pointer, std::forward<Args>(args)...);

        // No need to provide custom deleter as unique_ptr will default to the overridden ::delete which tracks frees
        return std::unique_ptr<T>(pointer);
    }

}
