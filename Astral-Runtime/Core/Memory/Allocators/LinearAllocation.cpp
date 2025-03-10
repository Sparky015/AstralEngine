/**
* @file LinearAllocation.cpp
* @author Andrew Fagan
* @date 2/3/25
*/

#include "LinearAllocator.h"

namespace Core {

    LinearAllocator::LinearAllocator(size_t memoryBlockSize) :
            m_StartBlockAddress((unsigned char*)AllocatorUtils::AllocMaxAlignedBlock(memoryBlockSize)),
            m_EndBlockAddress(m_StartBlockAddress + memoryBlockSize),
            m_CurrentMarker(m_StartBlockAddress)
    {
        AllocatorUtils::SetMemoryRegionAccess(m_StartBlockAddress, memoryBlockSize,
                                              ASANRegionPermission::AccessRestricted);
    }


    LinearAllocator::~LinearAllocator()
    {
        AllocatorUtils::FreeMaxAlignedBlock(m_StartBlockAddress);
    }


    void* LinearAllocator::Allocate(size_t size, uint16 alignment)
    {

        ASSERT(AllocatorUtils::IsAlignmentPowerOfTwo(alignment), "Given alignment is not a power of two!")
        if (AllocatorUtils::DoesCauseOverflow(m_CurrentMarker, size, m_EndBlockAddress))
        {
            return nullptr;
        }

        std::size_t space = m_EndBlockAddress - m_CurrentMarker;
        void* alignedAddress = m_CurrentMarker;

        // Aligns the address and will return nullptr if there is not enough space
        if (!std::align(alignment, size, alignedAddress, space)) { return nullptr; }

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


    void LinearAllocator::Reset()
    {
        AllocatorUtils::SetMemoryRegionAccess(m_StartBlockAddress, GetCapacity(), ASANRegionPermission::AccessRestricted);
        m_CurrentMarker = m_StartBlockAddress;
    }


    bool LinearAllocator::ResizeBuffer()
    {
        if (GetUsedBlockSize() != 0) { return false; }
        return ResizeInternalMemoryBlock();
    }


    LinearAllocator::LinearAllocator(LinearAllocator&& other) noexcept :
            m_StartBlockAddress(other.m_StartBlockAddress),
            m_EndBlockAddress(other.m_EndBlockAddress),
            m_CurrentMarker(other.m_CurrentMarker)
    {
        other.m_StartBlockAddress = nullptr;
        other.m_EndBlockAddress = nullptr;
        other.m_CurrentMarker = nullptr;
    }


    LinearAllocator& LinearAllocator::operator=(LinearAllocator&& other) noexcept
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


    bool LinearAllocator::operator==(const LinearAllocator& other) noexcept
    {
        return (m_CurrentMarker == other.m_CurrentMarker &&
                m_EndBlockAddress == other.m_EndBlockAddress &&
                m_StartBlockAddress == other.m_StartBlockAddress);
    }


    bool LinearAllocator::operator!=(const LinearAllocator& other) noexcept
    {
        return !(*this == other);
    }


    bool LinearAllocator::ResizeInternalMemoryBlock()
    {
        [[unlikely]] if (GetUsedBlockSize() != 0) { return false; }

        void* newMemoryBlock = nullptr;
        size_t newMemoryBufferSize = 0;

        // Function frees the old block and manages the asan region permissions
        if (!AllocatorUtils::ResizeMemoryBlock(m_StartBlockAddress, GetCapacity(), newMemoryBlock, newMemoryBufferSize))
        {
            WARN("Allocator memory block resize failed!");
            return false;
        }

        m_StartBlockAddress = (unsigned char*)newMemoryBlock;
        m_CurrentMarker = m_StartBlockAddress;
        m_EndBlockAddress = m_StartBlockAddress + newMemoryBufferSize;

        AllocatorUtils::SetMemoryRegionAccess(m_StartBlockAddress, GetCapacity(), ASANRegionPermission::AccessRestricted);
        return true;
    }

}