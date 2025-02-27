/**
* @file AlignedAllocator.cpp
* @author Andrew Fagan
* @date 2/7/25
*/

#include "AlignedAllocator.h"
#include "AllocatorUtils.h"

#include <memory>

namespace Core {

    void* AlignedAlloc(size_t size, uint8 alignment)
    {
        [[unlikely]] if (size == 0) { return nullptr; }

        ASSERT(AllocatorUtils::IsAlignmentPowerOfTwo(alignment), "Alignment must be a power of two.")

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

        AllocatorUtils::SetMemoryRegionAccess(allocatedAddress, alignmentOffset - 1, ASANRegionPermission::AccessRestricted); // for alignment padding
        AllocatorUtils::SetMemoryRegionAccess(alignedAddress, size, ASANRegionPermission::AccessGranted); // for main allocated block

        // Memory Layout:
        // [malloc'd block start]
        // [...padding for alignment...]
        // [1 byte header (alignment offset)]
        // [user accessible memory]
        // [malloc'd block end]

        return alignedAddress;
    }


    void FreeAlignedAlloc(void* pointer)
    {
        [[unlikely]] if (pointer == nullptr) { return; }

        // Unpoisoning allocation header if asan is used
        AllocatorUtils::SetMemoryRegionAccess(static_cast<unsigned char*>(pointer) - 1, 1, ASANRegionPermission::AccessGranted);

        // Get the natural alignment offset size from the allocation header
        unsigned char* headerMarker = static_cast<unsigned char*>(pointer) - 1;
        uint8 alignmentOffset = *headerMarker;

        // ASAN will automatically start monitoring for use after frees and other stuff when the block is freed by std::free.
        // No further action is needed.
        std::free((unsigned char*)pointer - alignmentOffset);
    }

}