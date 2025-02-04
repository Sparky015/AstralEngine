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


    void LinearAllocator::Reset()
    {
        TRACK_DEALLOCATION(m_CurrentMarker - m_StartBlockAddress);
        AllocatorUtils::SetMemoryRegionAccess(m_StartBlockAddress, GetCapacity(), ASANRegionPermission::AccessRestricted);
        m_CurrentMarker = m_StartBlockAddress;
    }


    bool LinearAllocator::ResizeBuffer()
    {
        if (GetUsedBlockSize() != 0) { return false; }
        return ResizeInternalMemoryBlock();
    }


    LinearAllocator::LinearAllocator(const LinearAllocator& other) :
            m_StartBlockAddress((unsigned char*)AllocatorUtils::AllocMaxAlignedBlock(other.GetCapacity())),
            m_EndBlockAddress(m_StartBlockAddress + other.GetCapacity()),
            m_CurrentMarker(m_StartBlockAddress + other.GetUsedBlockSize())
    {
        ASSERT(m_StartBlockAddress, "Allocation failed!");

        // Set permissions for the memcpy. Assures that both are not poisoned regions.
        AllocatorUtils::SetMemoryRegionAccess(other.m_StartBlockAddress, other.GetCapacity(), ASANRegionPermission::AccessGranted);

        std::memcpy(m_StartBlockAddress, other.m_StartBlockAddress, other.GetCapacity());

        AllocatorUtils::SetMemoryRegionAccess(other.m_StartBlockAddress, other.GetUsedBlockSize(),
                                              ASANRegionPermission::AccessGranted);
        AllocatorUtils::SetMemoryRegionAccess(other.m_CurrentMarker, other.GetCapacity() - other.GetUsedBlockSize(),
                                              ASANRegionPermission::AccessRestricted);
        AllocatorUtils::SetMemoryRegionAccess(m_StartBlockAddress, GetUsedBlockSize(),
                                              ASANRegionPermission::AccessGranted);
        AllocatorUtils::SetMemoryRegionAccess(m_CurrentMarker, GetCapacity() - GetUsedBlockSize(),
                                              ASANRegionPermission::AccessRestricted);
    }


    LinearAllocator& LinearAllocator::operator=(const LinearAllocator& other)
    {
        if (this != &other)
        {
            // Set permissions for the memcpy. Assures that both are not poisoned regions.
            AllocatorUtils::SetMemoryRegionAccess(other.m_StartBlockAddress, other.GetCapacity(), ASANRegionPermission::AccessGranted);

            AllocatorUtils::FreeAlignedAlloc(m_StartBlockAddress);
            m_StartBlockAddress = (unsigned char*)AllocatorUtils::AllocMaxAlignedBlock(other.GetCapacity());
            ASSERT(m_StartBlockAddress, "Allocation failed!");

            std::memcpy(m_StartBlockAddress, other.m_StartBlockAddress, other.GetCapacity());
            m_EndBlockAddress = m_StartBlockAddress + other.GetCapacity();
            m_CurrentMarker = m_StartBlockAddress + other.GetUsedBlockSize();

            AllocatorUtils::SetMemoryRegionAccess(other.m_StartBlockAddress, other.GetUsedBlockSize(),
                                                  ASANRegionPermission::AccessGranted);
            AllocatorUtils::SetMemoryRegionAccess(other.m_CurrentMarker, other.GetCapacity() - other.GetUsedBlockSize(),
                                                  ASANRegionPermission::AccessRestricted);
            AllocatorUtils::SetMemoryRegionAccess(m_StartBlockAddress, GetUsedBlockSize(),
                                                  ASANRegionPermission::AccessGranted);
            AllocatorUtils::SetMemoryRegionAccess(m_CurrentMarker, GetCapacity() - GetUsedBlockSize(),
                                                  ASANRegionPermission::AccessRestricted);
        }
        return *this;
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