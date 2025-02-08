/**
* @file GlobalAllocationStorage.h
* @author Andrew Fagan
* @date 1/7/2025
*/
#pragma once

#include "Core/Memory/Allocators/NoTrackingAllocator.h"
#include <unordered_map>

namespace Core {

    /**@class GlobalAllocationStorage
     * @brief Stores the size of memory blocks allocated to pointers */
    class GlobalAllocationStorage
    {
    public:
        GlobalAllocationStorage() = default;
        ~GlobalAllocationStorage() = default;

        /**@brief Stores a mapping of the pointer to the allocation size of the pointer */
        void AddPointer(const void* pointer, size_t size);

        /**@brief Removes the pointer's entry in the allocation size mapping
         * @param pointer The pointer that is being freed. Removes entry of pointer only if it was already being tracked.
         * Does nothing when given nullptr. */
        void FreePointer(const void* pointer);

        /**@brief Gets the allocated memory block size given to a pointer
         * @param pointer The pointer to the allocated memory block.
         * @return The size of the allocated memory block given to pointer.
         * Returns 0 if
         * - pointer is nullptr.
         * - pointer was not previously registered with AddPointer method. */
        size_t GetPointerSize(const void* pointer);

    private:

        std::unordered_map<const void*, size_t, std::hash<const void*>, std::equal_to<const void*>, NoTrackingAllocator<std::pair<const void* const, size_t>>> m_Storage;
    };

}