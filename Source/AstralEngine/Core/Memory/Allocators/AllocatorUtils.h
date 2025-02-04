/**
* @file AllocatorUtils.h
* @author Andrew Fagan
* @date 1/28/25
*/

#pragma once

#include <cstdlib>

#include "Core/CoreMacroDefinitions.h"
#include "Debug/Macros/Asserts.h"
#include "Debug/Macros/Error.h"
#include "Debug/Macros/Loggers.h"


#if defined(__has_feature)
    #if __has_feature(address_sanitizer)
    #define ASTRAL_ASAN_AVAILABLE
    #endif
#elif defined(__SANITIZE_ADDRESS__)
#define ASTRAL_ASAN_AVAILABLE
#endif

#ifdef ASTRAL_ASAN_AVAILABLE
#include <sanitizer/asan_interface.h>
#endif


namespace Core {

    /**@enum ASANRegionPermission
     * @brief Contains the access permissions that can be assigned to a region. */
    enum class ASANRegionPermission : uint8 {
        AccessRestricted,
        AccessGranted
    };
    /** This is outside the AllocatorUtil namespace to avoid repeating the
     * AllocatorUtils specifier when being used by functions in those namespaces
     * Ex.
     * Instead of:
     *   > AllocatorUtils::SetMemoryRegionAccess(alignedAddress, size, AllocatorUtils::ASANRegionPermission::AccessGranted);
     * It will be:
     *   > AllocatorUtils::SetMemoryRegionAccess(alignedAddress, size, ASANRegionPermission::AccessGranted); */
}

namespace Core::AllocatorUtils {

    /** @brief Uses ASAN to set or restrict access to a memory region. This will only set region access when in debug builds.
     *  @param startAddress The address to the start of the memory region.
     *  @param size The size of the memory region
     *  @param accessType The access permissions for the region that will be set.
     *  @note - This is a no-op if ASAN is not available.
     *        - This is a no-op if the build type is not debug. */
    inline void SetMemoryRegionAccess(void* startAddress, size_t size, ASANRegionPermission accessType)
    {
#if defined(ASTRAL_DEBUG_BUILD) && defined(ASTRAL_ASAN_AVAILABLE)
      switch (accessType)
      {
          case ASANRegionPermission::AccessRestricted:
              ASAN_POISON_MEMORY_REGION(startAddress, size);
              break;
          case ASANRegionPermission::AccessGranted:
              ASAN_UNPOISON_MEMORY_REGION(startAddress, size);
              break;
          default: ERROR("Undefined ASANRegionPermission value was passed to SetMemoryRegionAccess!")
      }
#endif
    }


    /**@brief Checks if a given alignment is a power of two.
     * @param alignment The alignment being checked
     * @return True if the alignment is a power of two, and false otherwise.
     * @warning Alignments should not be zero. Passing in zero as an argument
     *          with return the wrong answer. Do not pass in zero.
     * @thread_safety This function is not thread safe. */
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
     * @thread_safety This function is not thread safe. */
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
     * @thread_safety This function is not thread safe. */
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
    [[nodiscard]] void* AlignedAlloc(size_t size, uint8 alignment);


    /** @brief Frees a memory block allocated by AllocatorUtils::AlignedAlloc.
     *  @param pointer The pointer to the memory block being freed
     *  @warning This free method can only be used to free pointers allocated by
     *           AllocatorUtils::AlignedAlloc and ONLY that method.
     *  @thread_safety This function is not thread safe.
     *  @note This function will automatically change the ASAN region access permission to ASANRegionPermission::AccessRestricted when applicable. */
    void FreeAlignedAlloc(void* pointer);


    /** @brief Allocates a max aligned memory block that is at least the size of originalBlockSize. (Alignment equal to max_align_t)
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
     *  @note This function ensures the corresponding free function for std::aligned_alloc is called.
     *  @thread_safety This function is not thread safe. */
    inline void FreeMaxAlignedBlock(void* blockPtr)
    {
        FreeAlignedAlloc(blockPtr);
    }


    /** @brief Resizes an allocator's memory block to a bigger size. This frees the old pointer only if resize allocation succeeds.
     *  @param memoryBlockPtr Pointer to the allocator's memory block.
     *  @param memoryBlockSize The current size of the allocator's memory block
     *  @param outNewMemoryBufferPointer A pointer to the new allocated block that is resized by the given. Will be nullptr if resize allocation failed.
     *  @param newMemoryBufferSize The amount the original memory block size is multiplied by to get the new memory block's size.
     *  @return True if the resize operation succeeded and false if the resize allocation failed
     *  @warning This does not copy any data over. It is intended to be used when the allocator is empty.
     *  @thread_safety This function is not thread safe. */
    [[nodiscard]] bool ResizeMemoryBlock(void* memoryBlockPtr, size_t memoryBlockSize, void*& outNewMemoryBufferPointer, size_t& newMemoryBufferSize);

}

