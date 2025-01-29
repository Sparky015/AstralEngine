/**
* @file AllocatorUtils.h
* @author Andrew Fagan
* @date 1/28/25
*/

#pragma once

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

    constexpr size_t RoundToNextAlignmentMultiple(size_t originalBlockSize, size_t alignment)
    {
        return ((originalBlockSize + alignment - 1) / alignment) * alignment;
    }


}
