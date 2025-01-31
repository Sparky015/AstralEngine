/**
* @file AllocatorUtils.h
* @author Andrew Fagan
* @date 1/28/25
*/

#pragma once

#include <cstdlib>
#include "Debug/Macros/Loggers.h"

#ifdef ASTRAL_PLATFORM_WINDOWS
#define ALLOCATOR_UTILS_ALIGNED_ALLOC(alignment, size) _aligned_malloc(size, alignment)
#define ALLOCATOR_UTILS_ALIGNED_FREE(pointer) _aligned_free(pointer)
#else
#define ALLOCATOR_UTILS_ALIGNED_ALLOC(alignment, size) std::aligned_alloc(alignment, size)
#define ALLOCATOR_UTILS_ALIGNED_FREE(pointer) std::free(pointer)
#endif

namespace Core::AllocatorUtils {

    /**@brief Checks if a given alignment is a power of two.
     * @param alignment The alignment being checked
     * @return True if the alignment is a power of two, and false otherwise. */
    [[nodiscard]] constexpr bool IsAlignmentPowerOfTwo(const size_t alignment) noexcept
    {
        return (alignment & (alignment - 1)) == 0;
    }

    /**@brief Checks if allocating a given number of bytes will overflow the given end address
     * @param currentAddress The current address marker of an allocator
     * @param numberOfBytes The number of bytes that will be allocated
     * @param endAddress The end address of the allocator memory block
     * @return True if allocating the given number of bytes will overflow the allocator memory block, and false otherwise. */
    [[nodiscard]] constexpr bool DoesCauseOverflow(void* currentAddress, const size_t numberOfBytes, void* endAddress) noexcept
    {
        return (static_cast<unsigned char*>(currentAddress) + numberOfBytes) > static_cast<unsigned char*>(endAddress);
    }

    /**@brief Rounds the block size to the next multiple of the alignment. This allows for blocks to be used by
     *        std::aligned_alloc.
     * @param originalBlockSize The original block size of the allocator.
     *                          Expected to be not zero. Can't be negative because of size_t.
     * @param alignment The alignment that the memory block will be aligned to
     * @return A block size that is the next multiple of the alignment. */
    constexpr size_t RoundToNextAlignmentMultiple(size_t originalBlockSize, size_t alignment)
    {
        return ((originalBlockSize + alignment - 1) / alignment) * alignment;
    }

    /** @brief Allocates a max aligned memory block that is at least the size of originalBlockSize.
     *  @param originalBlockSize The minimum block size that should be allocated.
     *  @return A pointer to a max aligned memory block
     *  @note - The actual block size of the max aligned memory block might be slightly bigger due to adjusting the size
     *        of the block to make it a multiple of the alignment.
     *        - Use AllocatorUtils::FreeMaxAlignedBlock to free memory allocated by this function. */
    inline void* AllocMaxAlignedBlock(size_t originalBlockSize)
    {
        const size_t alignedBlockSize = RoundToNextAlignmentMultiple(originalBlockSize, alignof(max_align_t));
        return ALLOCATOR_UTILS_ALIGNED_ALLOC(alignof(std::max_align_t), alignedBlockSize);
    }

    /** @brief Frees a max aligned memory block that was allocated by AllocatorUtils::AllocMaxAlignedBlock.
     *  @param blockPtr The pointer to the block being freed.
     *  @note This function ensures the corresponding free function for std::aligned_alloc is called. */
    inline void FreeMaxAlignedBlock(void* blockPtr)
    {
        ALLOCATOR_UTILS_ALIGNED_FREE(blockPtr);
    }

    /** @brief Resizes an allocator's memory block to a bigger size.
     *  @param memoryBlockPtr Pointer to the allocator's memory block.
     *  @param memoryBlockSize The current size of the allocator's memory block
     *  @param resizeMultiplier The amount the original memory block size is multiplied by to get the new memory block's size.
     *  @return A pointer to the new allocated block that is resized by the given. */
    inline void* ResizeMemoryBlock(void* memoryBlockPtr, size_t memoryBlockSize, float resizeMultiplier = 2.0f)
    {
        const size_t newBlockSize = memoryBlockSize * resizeMultiplier;
        void* newBlockPtr = AllocMaxAlignedBlock(newBlockSize);
        std::memcpy(newBlockPtr, memoryBlockPtr, memoryBlockSize);
        FreeMaxAlignedBlock(memoryBlockPtr);

        WARN("Resizing Allocator Memory Block! (" << memoryBlockSize << " bytes -> " << newBlockSize << " bytes)");
        return newBlockPtr;
    }

}

#undef ALLOCATOR_UTILS_ALIGNED_ALLOC
#undef ALLOCATOR_UTILS_ALIGNED_FREE
