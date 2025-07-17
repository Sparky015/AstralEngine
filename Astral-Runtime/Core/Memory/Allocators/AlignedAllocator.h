/**
* @file AlignedAllocator.h
* @author Andrew Fagan
* @date 2/7/25
*/

#pragma once
#include "Core/FixedIntegerTypes.h"

namespace Astral {

    /** @brief Allocates a memory block which has the given alignment
     *  @param size The size of the memory block to allocate
     *  @param alignment The alignment of the allocated memory block. Must be a power of two.
     *  @return A pointer to the allocated memory block. Returns nullptr if size is zero.
     *  @warning The max alignment support through this function is 255.
     *  @thread_safety This function is not thread safe. */
    [[nodiscard]] void* AlignedAlloc(size_t size, uint8 alignment);


    /** @brief Frees a memory block allocated by AllocatorUtils::AlignedAlloc.
     *  @param pointer The pointer to the memory block being freed
     *  @warning This free method can only be used to free pointers allocated by
     *           AllocatorUtils::AlignedAlloc and ONLY that method.
     *  @thread_safety This function is not thread safe.
     *  @note This function will automatically change the ASAN region access permission to ASANRegionPermission::AccessRestricted when applicable. */
    void FreeAlignedAlloc(void* pointer);

}
