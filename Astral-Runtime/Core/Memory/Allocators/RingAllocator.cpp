/**
* @file RingAllocator.cpp
* @author Andrew Fagan
* @date 2/4/2025
*/

#include "RingAllocator.h"

namespace Astral {

    RingAllocator::RingAllocator(size_t memoryBlockSize) :
            m_StartBlockAddress((unsigned char*)AllocatorUtils::AllocMaxAlignedBlock(memoryBlockSize)),
            m_EndBlockAddress(m_StartBlockAddress + memoryBlockSize),
            m_CurrentMarker(m_StartBlockAddress)
    {
        // Set the ASAN region access initially, but the memory block access will gradually all have "access granted" permissions
        // at all times as the allocations start fill the allocator block and wrap around. (Except the alignment padding)
        AllocatorUtils::SetMemoryRegionAccess(m_StartBlockAddress, memoryBlockSize,
                                              ASANRegionPermission::AccessRestricted);
    }


    RingAllocator::~RingAllocator()
    {
        AllocatorUtils::FreeMaxAlignedBlock(m_StartBlockAddress);
    }


    void* RingAllocator::Allocate(size_t size, uint16 alignment)
    {
        ASSERT(AllocatorUtils::IsAlignmentPowerOfTwo(alignment), "Given alignment is not a power of two!")
        [[unlikely]] if (size > GetCapacity() || alignment > GetCapacity())
        {
            ASSERT(size <= GetCapacity(), "The allocated size is greater than the capacity of this allocator!")
            ASSERT(alignment < GetCapacity(), "Alignment can not be greater than the capacity.");
            return nullptr;
        }

        [[unlikely]] if (AllocatorUtils::DoesCauseOverflow(m_CurrentMarker, size, m_EndBlockAddress))
        {
            m_CurrentMarker = m_StartBlockAddress;
        }

        std::size_t space = m_EndBlockAddress - m_CurrentMarker;
        void* alignedAddress = m_CurrentMarker;

        // Aligns the address and will return nullptr if there is not enough space
        if (!std::align(alignment, size, alignedAddress, space))
        {
            m_CurrentMarker = m_StartBlockAddress;
            alignedAddress = m_StartBlockAddress;
            space = m_EndBlockAddress - m_StartBlockAddress;
            if (!std::align(alignment, size, alignedAddress, space))
            {
                ASSERT(false, "Not enough space for allocation!")
                return nullptr;
            };
        }

        AllocatorUtils::SetMemoryRegionAccess(m_CurrentMarker, (unsigned char*) alignedAddress - m_CurrentMarker,
                                              ASANRegionPermission::AccessRestricted); // For alignment padding
        AllocatorUtils::SetMemoryRegionAccess(alignedAddress, size, ASANRegionPermission::AccessGranted);

        // Update current marker
        m_CurrentMarker = static_cast<unsigned char*>(alignedAddress) + size;

        // Memory Layout:
        // [Allocate'd block start]
        // [...padding for alignment...]
        // [user accessible memory]
        // [Allocate'd block end]

        return alignedAddress;
    }


    void RingAllocator::Reset()
    {
        AllocatorUtils::SetMemoryRegionAccess(m_StartBlockAddress, GetCapacity(), ASANRegionPermission::AccessRestricted);
        m_CurrentMarker = m_StartBlockAddress;
    }


    bool RingAllocator::ResizeBuffer()
    {
        return ResizeInternalMemoryBlock();
    }


    RingAllocator::RingAllocator(RingAllocator&& other) noexcept :
            m_StartBlockAddress(other.m_StartBlockAddress),
            m_EndBlockAddress(other.m_EndBlockAddress),
            m_CurrentMarker(other.m_CurrentMarker)
    {
        other.m_StartBlockAddress = nullptr;
        other.m_EndBlockAddress = nullptr;
        other.m_CurrentMarker = nullptr;
    }


    RingAllocator& RingAllocator::operator=(RingAllocator&& other) noexcept
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


    bool RingAllocator::operator==(const RingAllocator& other) noexcept
    {
        return (m_CurrentMarker == other.m_CurrentMarker &&
                m_EndBlockAddress == other.m_EndBlockAddress &&
                m_StartBlockAddress == other.m_StartBlockAddress);
    }


    bool RingAllocator::operator!=(const RingAllocator& other) noexcept
    {
        return !(*this == other);
    }


    bool RingAllocator::ResizeInternalMemoryBlock()
    {
        void* newMemoryBlock = nullptr;
        size_t newMemoryBufferSize = 0;

        // Function frees the old block and manages the asan region permissions
        if (!AllocatorUtils::ResizeMemoryBlock(m_StartBlockAddress, GetCapacity(), newMemoryBlock, newMemoryBufferSize))
        {
            AE_WARN("Allocator memory block resize failed!");
            return false;
        }

        m_StartBlockAddress = (unsigned char*)newMemoryBlock;
        m_CurrentMarker = m_StartBlockAddress;
        m_EndBlockAddress = m_StartBlockAddress + newMemoryBufferSize;

        AllocatorUtils::SetMemoryRegionAccess(m_StartBlockAddress, GetCapacity(), ASANRegionPermission::AccessRestricted);
        return true;
    }

}