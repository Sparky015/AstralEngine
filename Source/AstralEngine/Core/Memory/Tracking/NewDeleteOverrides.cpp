//
// Created by Andrew Fagan on 1/7/25.
//

#include "NewDeleteOverrides.h"

#include <cstdlib> // for std::malloc and std::free
#include <iostream>
#include "Core/Memory/Allocators/NoTrackingMalloc.h"

#include "MemoryTracker.h"

void* operator new(std::size_t size)
{
    void* pointer = ASTRAL_NO_TRACKING_MALLOC(size);
    if (!pointer) throw std::bad_alloc();
    Core::MemoryTracker::Get().AddAllocation(pointer, size, Core::MemoryRegion::UNKNOWN, Core::AllocatorType::NEW_OPERATOR);
    return pointer;
}


void* operator new[](std::size_t size)
{
    void* pointer = ASTRAL_NO_TRACKING_MALLOC(size);
    if (!pointer) throw std::bad_alloc();
    Core::MemoryTracker::Get().AddAllocation(pointer, size, Core::MemoryRegion::UNKNOWN, Core::AllocatorType::NEW_OPERATOR);
    return pointer;
}


void operator delete(void* pointer) noexcept
{
    Core::MemoryTracker::Get().RemoveAllocation(pointer);
    ASTRAL_NO_TRACKING_FREE(pointer);
}


void operator delete[](void* pointer) noexcept
{
    Core::MemoryTracker::Get().RemoveAllocation(pointer);
    ASTRAL_NO_TRACKING_FREE(pointer);
}


// Override C malloc/free functions
extern "C"
{
    void* malloc(size_t size)
    {
        // Get the real malloc using dlsym/GetProcAddress
        void* ptr = ASTRAL_NO_TRACKING_MALLOC(size);
        Core::MemoryTracker::Get().AddAllocation(ptr, size, Core::MemoryRegion::UNKNOWN, Core::AllocatorType::MALLOC);
        return ptr;
    }


    void free(void* ptr)
    {
        if (ptr) {
            Core::MemoryTracker::Get().RemoveAllocation(ptr);
            ASTRAL_NO_TRACKING_FREE(ptr);
        }
    }


    void* calloc(size_t num, size_t size)
    {
        static auto real_calloc = (void*(*)(size_t, size_t))dlsym(RTLD_NEXT, "calloc");
        void* ptr = real_calloc(num, size);
        Core::MemoryTracker::Get().AddAllocation(ptr, num * size, Core::MemoryRegion::UNKNOWN, Core::AllocatorType::MALLOC);
        return ptr;
    }


    void* realloc(void* ptr, size_t new_size)
    {
        static auto real_realloc = (void*(*)(void*, size_t))dlsym(RTLD_NEXT, "realloc");
        if (ptr) {
            Core::MemoryTracker::Get().RemoveAllocation(ptr);
        }
        void* new_ptr = real_realloc(ptr, new_size);
        Core::MemoryTracker::Get().AddAllocation(new_ptr, new_size, Core::MemoryRegion::UNKNOWN, Core::AllocatorType::MALLOC);
        return new_ptr;
    }

}