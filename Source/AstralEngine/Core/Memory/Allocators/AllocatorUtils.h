/**
* @file AllocatorUtils.h
* @author Andrew Fagan
* @date 1/28/25
*/

#pragma once

#include "Core/CoreMacroDefinitions.h"
#include "Debug/Macros/Asserts.h"
#include "Debug/Macros/Loggers.h"

#include <cstdlib>


#ifdef ASTRAL_DEBUG_BUILD
#define ALLOCATOR_UTILS_SET_MEMORY_REGION(startAddress, length, setType) SetMemoryRegion(startAddress, length, setType)
#else
#define ALLOCATOR_UTILS_SET_MEMORY_REGION(startAddress, length, setType)
#endif

namespace Core::AllocatorUtils {

    enum AllocatorMemorySetType : uint8
    {
        AlignedOffsetFence = 0xBD,
        AllocatedMemory = 0xCD,
        FreedMemory = 0xDD,
    };

    inline void SetMemoryRegion(void* startAddress, size_t length, AllocatorMemorySetType setType)
    {
        std::memset(startAddress, setType, length);
        LOG("Function called! -------------");
    }

    /**@brief Checks if a given alignment is a power of two.
     * @param alignment The alignment being checked
     * @return True if the alignment is a power of two, and false otherwise.
     * @warning Alignments should not be zero. Passing in zero as an argument
     *          with return the wrong answer. Do not pass in zero.
     *  @thread_safety This function is not thread safe. */
    [[nodiscard]] constexpr bool IsAlignmentPowerOfTwo(const size_t alignment) noexcept
    {
        [[unlikely]] if (alignment == 0) { return false; }
        return (alignment & (alignment - 1)) == 0;
    }

    /**@brief Checks if allocating a given number of bytes will overflow the given end address
     * @param currentAddress The current address marker of an allocator
     * @param numberOfBytes The number of bytes that will be allocated
     * @param endAddress The end address of the allocator memory block
     * @return True if allocating the given number of bytes will overflow the allocator memory block, and false otherwise.
     *  @thread_safety This function is not thread safe. */
    [[nodiscard]] constexpr bool DoesCauseOverflow(void* currentAddress, const size_t numberOfBytes, void* endAddress) noexcept
    {
        return (static_cast<unsigned char*>(currentAddress) + numberOfBytes) > static_cast<unsigned char*>(endAddress);
    }

    /**@brief Rounds the block size to the next multiple of the alignment. This allows for blocks to be used by
     *        std::aligned_alloc.
     * @param originalBlockSize The original block size of the allocator.
     *                          Expected to be not zero. Can't be negative because of size_t.
     * @param alignment The alignment that the memory block will be aligned to
     * @return A block size that is the next multiple of the alignment. Returns 0 if given alignment is 0.
     *  @thread_safety This function is not thread safe. */
    constexpr size_t RoundToNextAlignmentMultiple(size_t originalBlockSize, size_t alignment)
    {
        [[unlikely]] if (alignment == 0) { return 0; }
        return ((originalBlockSize + alignment - 1) / alignment) * alignment;
    }

    /** @brief Allocates a memory block which has the given alignment
     *  @param size The size of the memory block to allocate
     *  @param alignment The alignment of the allocated memory block. Must be a power of two.
     *  @return A pointer to the allocated memory block. Returns nullptr if size is zero.
     *  @warning The max alignment support through this function is 255.
     *  @thread_safety This function is not thread safe. */
    inline void* AlignedAlloc(size_t size, uint8 alignment)
    {
        [[unlikely]] if (size == 0) { return nullptr; }

        ASSERT(IsAlignmentPowerOfTwo(alignment), "Alignment must be a power of two.")

        // Giving ourselves enough space to perform an alignment and for an allocator header to track the offset
        size_t RoundedBlockSize = size + alignment;
        size_t space = RoundedBlockSize;

        void* allocatedAddress = std::malloc(RoundedBlockSize);
        [[unlikely]] if (allocatedAddress == nullptr) { return nullptr; }

        void* alignedAddress = allocatedAddress;

        // Aligns the address and will return nullptr if there is not enough space
        alignedAddress = std::align(alignment, size, alignedAddress, space);

        [[unlikely]] if (alignedAddress == nullptr)
        {
            std::free(allocatedAddress);
            return nullptr;
        }

        ASSERT(space >= size, "The aligning of the address caused the block to be smaller than the user asked for.")

        if ((unsigned char*)alignedAddress == allocatedAddress)
        {
            // Address is already aligned. Push the address by the given alignment to make room for allocation header.
            alignedAddress = (unsigned char*)alignedAddress + alignment;
        }

        // Add allocation header for alignment amount
        unsigned char* m_HeaderMarker = (unsigned char*)alignedAddress - 1;
        const uint8 alignmentOffset = (unsigned char*)alignedAddress - (unsigned char*)allocatedAddress;
        *(m_HeaderMarker) = alignmentOffset;

        ALLOCATOR_UTILS_SET_MEMORY_REGION(alignedAddress, size, AllocatorMemorySetType::AllocatedMemory);

        return alignedAddress;
    }


    /** @brief Frees a memory block allocated by AllocatorUtils::AlignedAlloc.
     *  @param pointer The pointer to the memory block being freed
     *  @warning This free method can only be used to free pointers allocated by
     *           AllocatorUtils::AlignedAlloc and ONLY that method.
     *  @thread_safety This function is not thread safe. */
    inline void FreeAlignedAlloc(void* pointer)
    {
        [[unlikely]] if (pointer == nullptr) { return; }

        // Get the natural alignment offset size from the allocation header
        unsigned char* headerMarker = static_cast<unsigned char*>(pointer) - 1;
        uint8 alignmentOffset = *headerMarker;

        std::free((unsigned char*)pointer - alignmentOffset);
    }

    /** @brief Allocates a max aligned memory block that is at least the size of originalBlockSize.
     *  @param originalBlockSize The minimum block size that should be allocated.
     *  @return A pointer to a max aligned memory block
     *  @note - The actual block size of the max aligned memory block might be slightly bigger due to adjusting the size
     *        of the block to make it a multiple of the alignment.
     *        - Use AllocatorUtils::FreeMaxAlignedBlock to free memory allocated by this function.
     *  @thread_safety This function is not thread safe. */
    inline void* AllocMaxAlignedBlock(size_t originalBlockSize)
    {
        return AlignedAlloc(originalBlockSize, alignof(max_align_t));
    }

    /** @brief Frees a max aligned memory block that was allocated by AllocatorUtils::AllocMaxAlignedBlock.
     *  @param blockPtr The pointer to the block being freed.
     *  @note This function ensures the corresponding free function for std::aligned_alloc is called. */
    inline void FreeMaxAlignedBlock(void* blockPtr)
    {
        FreeAlignedAlloc(blockPtr);
    }

    /** @brief Resizes an allocator's memory block to a bigger size.
     *  @param memoryBlockPtr Pointer to the allocator's memory block.
     *  @param memoryBlockSize The current size of the allocator's memory block
     *  @param resizeMultiplier The amount the original memory block size is multiplied by to get the new memory block's size.
     *  @return A pointer to the new allocated block that is resized by the given. Returns nullptr if resize allocation failed.
     *  @warning This does not copy any data over. It is intended to be used when the allocator is empty.
     *  @thread_safety This function is not thread safe. */
    inline void ResizeMemoryBlock(void* memoryBlockPtr, size_t memoryBlockSize, void*& outNewMemoryBufferPointer, size_t& newMemoryBufferSize)
    {
        static constexpr float resizeMultiplier = 2.0f;
        newMemoryBufferSize = memoryBlockSize * resizeMultiplier;
        outNewMemoryBufferPointer = AllocMaxAlignedBlock(newMemoryBufferSize);
        if (!outNewMemoryBufferPointer) { return; }
        FreeMaxAlignedBlock(memoryBlockPtr);

        WARN("Resizing Allocator Memory Block! (" << memoryBlockSize << " bytes -> " << newMemoryBufferSize << " bytes)");
    }

}

