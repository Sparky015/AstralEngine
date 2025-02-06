/**
* @file FrameAllocator.cpp
* @author Andrew Fagan
* @date 2/3/25
*/

#include "FrameAllocator.h"

namespace Core {

    FrameAllocator::FrameAllocator(size_t memoryBlockSize)  :
            m_StartBlockAddress((unsigned char*)AllocatorUtils::AllocMaxAlignedBlock(memoryBlockSize)),
            m_EndBlockAddress(m_StartBlockAddress + memoryBlockSize),
            m_CurrentMarker(m_StartBlockAddress)
    {
        ASSERT(m_StartBlockAddress, "Allocation failed!");
        ASSERT(memoryBlockSize > 0, "The memory block size must be greater than 0");
        AllocatorUtils::SetMemoryRegionAccess(m_StartBlockAddress, memoryBlockSize,
                                              ASANRegionPermission::AccessRestricted);
    }


    FrameAllocator::~FrameAllocator()
    {
        AllocatorUtils::FreeMaxAlignedBlock(m_StartBlockAddress);
    }


    void FrameAllocator::RollbackToMarker(Marker const marker)
    {
        ASSERT(marker >= m_StartBlockAddress && marker <= m_EndBlockAddress, "Passed marker does not fall within this allocators memory block.")
        ASSERT(marker <= m_CurrentMarker, "Can not rollback to marker that is already past the top of the stack.")
        m_CurrentMarker = marker;
        AllocatorUtils::SetMemoryRegionAccess(m_CurrentMarker, GetCapacity() - GetUsedBlockSize(), ASANRegionPermission::AccessRestricted);
    }


    void* FrameAllocator::Allocate(size_t size, uint16 alignment)
    {
        ASSERT(AllocatorUtils::IsAlignmentPowerOfTwo(alignment), "Given alignment is not a power of two!")
        if (AllocatorUtils::DoesCauseOverflow(m_CurrentMarker, size, m_EndBlockAddress)) { return nullptr; }

        std::size_t space = m_EndBlockAddress - m_CurrentMarker;
        void* alignedAddress = m_CurrentMarker;

        // Aligns the address. Will return nullptr if there is not enough space triggering a bad_alloc exception
        if (!std::align(alignment, size, alignedAddress, space)) { return nullptr; }

        AllocatorUtils::SetMemoryRegionAccess(m_CurrentMarker, (unsigned char*) alignedAddress - m_CurrentMarker,
                                              ASANRegionPermission::AccessRestricted); // For allocation header
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


    void FrameAllocator::Reset()
    {
        AllocatorUtils::SetMemoryRegionAccess(m_StartBlockAddress, GetCapacity(), ASANRegionPermission::AccessRestricted);
        m_CurrentMarker = m_StartBlockAddress;
    }


    size_t FrameAllocator::GetUsedBlockSize() const
    {
        return m_CurrentMarker - m_StartBlockAddress;
    }


    size_t FrameAllocator::GetCapacity() const
    {
        return m_EndBlockAddress - m_StartBlockAddress;
    }


    bool FrameAllocator::ResizeBuffer()
    {
        if (GetUsedBlockSize() != 0) { return false; }
        return ResizeInternalMemoryBlock();
    }


    FrameAllocator::FrameAllocator(FrameAllocator&& other) noexcept :
            m_StartBlockAddress(other.m_StartBlockAddress),
            m_EndBlockAddress(other.m_EndBlockAddress),
            m_CurrentMarker(other.m_CurrentMarker)
    {
        other.m_StartBlockAddress = nullptr;
        other.m_EndBlockAddress = nullptr;
        other.m_CurrentMarker = nullptr;
    }


    FrameAllocator& FrameAllocator::operator=(FrameAllocator&& other) noexcept
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


    bool FrameAllocator::operator==(const FrameAllocator& other) noexcept
    {
        return (m_CurrentMarker == other.m_CurrentMarker &&
                m_StartBlockAddress == other.m_StartBlockAddress);
    }


    bool FrameAllocator::operator!=(const FrameAllocator& other) noexcept
    {
        return !(*this == other);
    }


    bool FrameAllocator::ResizeInternalMemoryBlock()
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