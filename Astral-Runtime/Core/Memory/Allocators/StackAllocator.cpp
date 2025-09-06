/**
* @file StackAllocator.cpp
* @author Andrew Fagan
* @date 2/3/25
*/

#include "StackAllocator.h"

namespace Astral {

    StackAllocator::StackAllocator(size_t memoryBlockSize) :
            m_StartBlockAddress((unsigned char*)AllocatorUtils::AllocMaxAlignedBlock(memoryBlockSize)),
            m_EndBlockAddress(m_StartBlockAddress + memoryBlockSize),
            m_CurrentMarker(m_StartBlockAddress)
    {
        ASSERT(m_StartBlockAddress, "Allocation failed!");
        ASSERT(memoryBlockSize > 0, "The memory block size must be greater than 0");
        AllocatorUtils::SetMemoryRegionAccess(m_StartBlockAddress, memoryBlockSize,
                                              ASANRegionPermission::AccessRestricted);
    }


    StackAllocator::~StackAllocator()
    {
        AllocatorUtils::FreeMaxAlignedBlock(m_StartBlockAddress);
    }


    void StackAllocator::RollbackToMarker(StackAllocator::Marker const marker)
    {
        ASSERT(marker >= m_StartBlockAddress && marker <= m_EndBlockAddress, "Passed marker does not fall within this allocators memory block.")
        ASSERT(marker <= m_CurrentMarker, "Can not rollback to marker that is already past the top of the stack.")
        m_CurrentMarker = marker;
        AllocatorUtils::SetMemoryRegionAccess(m_CurrentMarker, GetCapacity() - GetUsedBlockSize(), ASANRegionPermission::AccessRestricted);
    }


    void* StackAllocator::Allocate(size_t size, uint16 alignment)
    {
        ASSERT(AllocatorUtils::IsAlignmentPowerOfTwo(alignment), "Given alignment is not a power of two!")
        if (AllocatorUtils::DoesCauseOverflow(m_CurrentMarker, size, m_EndBlockAddress)) { return nullptr; }
        std::size_t space = m_EndBlockAddress - m_CurrentMarker;
        void* alignedAddress = m_CurrentMarker;

        // Aligns the address and will return nullptr if there is not enough space
        if (!std::align(alignment, size, alignedAddress, space)) { return nullptr; }

        if (static_cast<unsigned char*>(alignedAddress) == m_CurrentMarker)
        {
            // Address is already aligned. Push the address by the alignment of T to make room for allocation header.
            alignedAddress = static_cast<unsigned char*>(alignedAddress) + alignment;
            if (AllocatorUtils::DoesCauseOverflow(alignedAddress, size, m_EndBlockAddress)) { return nullptr; }
        }



        // Add allocation header for alignment amount
        unsigned char* m_HeaderMarker = static_cast<unsigned char*>(alignedAddress) - 1;
        AllocatorUtils::SetMemoryRegionAccess(m_HeaderMarker, 1, ASANRegionPermission::AccessGranted);
        const uint8 alignmentOffset = static_cast<unsigned char*>(alignedAddress) - m_CurrentMarker;
        *(m_HeaderMarker) = alignmentOffset;

        AllocatorUtils::SetMemoryRegionAccess(m_CurrentMarker, (unsigned char*)alignedAddress - m_CurrentMarker,
                                              ASANRegionPermission::AccessRestricted); // For allocation header
        AllocatorUtils::SetMemoryRegionAccess(alignedAddress, size, ASANRegionPermission::AccessGranted);

        // Update current marker
        m_CurrentMarker = static_cast<unsigned char*>(alignedAddress) + size;

        // Memory Layout:
        // [Allocate'd block start]
        // [...padding for alignment...]
        // [1 byte header (alignment offset)]
        // [user accessible memory]
        // [Allocate'd block end]

        return alignedAddress;
    }


    void StackAllocator::Deallocate(void* ptr, size_t sizeOfAllocatedBlock)
    {
        [[unlikely]] if (ptr == nullptr || sizeOfAllocatedBlock == 0) { return; }

        // Checking if this pointer is the last allocated pointer
        ASSERT(m_CurrentMarker - sizeOfAllocatedBlock == static_cast<unsigned char*>(ptr), "Deallocations must follow a last in first out order!")

        // Get the natural alignment offset size from the allocation header
        uint8 alignmentOffset = 0;
        if (static_cast<unsigned char*>(ptr) != m_StartBlockAddress)
        {
            AllocatorUtils::SetMemoryRegionAccess((unsigned char*)ptr - 1, 1,
                                                  ASANRegionPermission::AccessGranted); // For allocation header

            unsigned char* headerMarker = static_cast<unsigned char*>(ptr) - 1;
            alignmentOffset = *headerMarker;
        }

        // Roll back the marker by the size of the allocation and the natural alignment offset
        m_CurrentMarker -= sizeOfAllocatedBlock + alignmentOffset;
        AllocatorUtils::SetMemoryRegionAccess(m_CurrentMarker, sizeOfAllocatedBlock + alignmentOffset,
                                              ASANRegionPermission::AccessRestricted);

    }


    void StackAllocator::Reset()
    {
        m_CurrentMarker = m_StartBlockAddress;
    }


    bool StackAllocator::ResizeBuffer()
    {
        if (GetUsedBlockSize() != 0) { return false; }
        return ResizeInternalMemoryBlock();
    }


    StackAllocator::StackAllocator(StackAllocator&& other) noexcept :
            m_StartBlockAddress(other.m_StartBlockAddress),
            m_EndBlockAddress(other.m_EndBlockAddress),
            m_CurrentMarker(other.m_CurrentMarker)
    {
        other.m_StartBlockAddress = nullptr;
        other.m_EndBlockAddress = nullptr;
        other.m_CurrentMarker = nullptr;
    }


    StackAllocator& StackAllocator::operator=(StackAllocator&& other) noexcept
    {
        if (this != &other)
        {
            m_StartBlockAddress = other.m_StartBlockAddress;
            m_EndBlockAddress = other.m_EndBlockAddress;
            m_CurrentMarker = other.m_CurrentMarker;
            other.m_StartBlockAddress = nullptr;
            other.m_EndBlockAddress = nullptr;
            other.m_CurrentMarker = nullptr;
        }
        return *this;
    }


    bool StackAllocator::operator==(const StackAllocator& other) noexcept
    {
        return (m_CurrentMarker == other.m_CurrentMarker &&
                m_EndBlockAddress == other.m_EndBlockAddress &&
                m_StartBlockAddress == other.m_StartBlockAddress);
    }


    bool StackAllocator::operator!=(const StackAllocator& other) noexcept
    {
        return !(*this == other);
    }


    bool StackAllocator::ResizeInternalMemoryBlock()
    {
        size_t currentUsedSize = GetUsedBlockSize();
        void* newMemoryBlock = nullptr;
        size_t newMemoryBufferSize = 0;

        // Function frees the old block and manages the asan region permissions
        if (!AllocatorUtils::ResizeMemoryBlock(m_StartBlockAddress, GetCapacity(), newMemoryBlock, newMemoryBufferSize))
        {
            WARN("Allocator memory block resize failed!");
            return false;
        }

        m_StartBlockAddress = (unsigned char*)newMemoryBlock;
        m_CurrentMarker = m_StartBlockAddress + currentUsedSize;
        m_EndBlockAddress = m_StartBlockAddress + newMemoryBufferSize;

        return true;
    }

}