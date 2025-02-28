/**
* @file StackBasedLinearAllocator_impl.h
* @author Andrew Fagan
* @date 2/3/2025
*/

#pragma once

#include "AllocatorUtils.h"

namespace Core {

    template<size_t MemoryBlockSize>
    StackBasedLinearAllocator<MemoryBlockSize>::StackBasedLinearAllocator() :
            m_MemoryBlock(),
            m_StartBlockAddress(&m_MemoryBlock[0]),
            m_EndBlockAddress(m_StartBlockAddress + MemoryBlockSize),
            m_CurrentMarker(m_StartBlockAddress)
    {
        AllocatorUtils::SetMemoryRegionAccess(m_StartBlockAddress, MemoryBlockSize, ASANRegionPermission::AccessRestricted);
    }


    template<size_t MemoryBlockSize>
    StackBasedLinearAllocator<MemoryBlockSize>::~StackBasedLinearAllocator()
    {
        Reset();
    }


    template<size_t MemoryBlockSize>
    void* StackBasedLinearAllocator<MemoryBlockSize>::Allocate(size_t size, uint16 alignment)
    {
        ASSERT(AllocatorUtils::IsAlignmentPowerOfTwo(alignment), "Given alignment is not a power of two!")
        if (AllocatorUtils::DoesCauseOverflow(m_CurrentMarker, size, m_EndBlockAddress)) { return nullptr; }

        std::size_t space = m_EndBlockAddress - m_CurrentMarker;
        void* alignedAddress = m_CurrentMarker;

        // Aligns the address and will return nullptr if there is not enough space
        if (!std::align(alignment, size, alignedAddress, space)) { return nullptr; }

        AllocatorUtils::SetMemoryRegionAccess(m_CurrentMarker, (unsigned char*) alignedAddress - m_CurrentMarker,
                                              ASANRegionPermission::AccessRestricted);
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



    template<size_t MemoryBlockSize>
    void StackBasedLinearAllocator<MemoryBlockSize>::Reset()
    {
        AllocatorUtils::SetMemoryRegionAccess(m_StartBlockAddress, MemoryBlockSize, ASANRegionPermission::AccessRestricted);
        m_CurrentMarker = m_StartBlockAddress;
    }


    template<size_t MemoryBlockSize>
    StackBasedLinearAllocator<MemoryBlockSize>::StackBasedLinearAllocator(StackBasedLinearAllocator&& other) noexcept :
            m_MemoryBlock(),
            m_StartBlockAddress(&m_MemoryBlock[0]),
            m_EndBlockAddress(m_StartBlockAddress + MemoryBlockSize),
            m_CurrentMarker(m_StartBlockAddress + other.GetUsedBlockSize())
    {
        // Set permissions for the memcpy. Assures that both are not poisoned regions.
        AllocatorUtils::SetMemoryRegionAccess(other.m_StartBlockAddress, other.GetCapacity(), ASANRegionPermission::AccessGranted);
        AllocatorUtils::SetMemoryRegionAccess(m_StartBlockAddress, other.GetCapacity(), ASANRegionPermission::AccessGranted);

        std::memcpy(m_StartBlockAddress, other.m_StartBlockAddress, MemoryBlockSize);
        other.m_StartBlockAddress = nullptr;
        other.m_EndBlockAddress = nullptr;
        other.m_CurrentMarker = nullptr;

        AllocatorUtils::SetMemoryRegionAccess(other.m_StartBlockAddress, other.GetUsedBlockSize(),
                                              ASANRegionPermission::AccessGranted);
        AllocatorUtils::SetMemoryRegionAccess(other.m_CurrentMarker, other.GetCapacity() - other.GetUsedBlockSize(),
                                              ASANRegionPermission::AccessRestricted);
        AllocatorUtils::SetMemoryRegionAccess(m_StartBlockAddress, GetUsedBlockSize(),
                                              ASANRegionPermission::AccessGranted);
        AllocatorUtils::SetMemoryRegionAccess(m_CurrentMarker, GetCapacity() - GetUsedBlockSize(),
                                              ASANRegionPermission::AccessRestricted);
    }


    template<size_t MemoryBlockSize>
    StackBasedLinearAllocator<MemoryBlockSize>& StackBasedLinearAllocator<MemoryBlockSize>::operator=(StackBasedLinearAllocator&& other) noexcept
    {
        if (this != &other)
        {
            // Set permissions for the memcpy. Assures that both are not poisoned regions.
            AllocatorUtils::SetMemoryRegionAccess(other.m_StartBlockAddress, other.GetCapacity(), ASANRegionPermission::AccessGranted);
            AllocatorUtils::SetMemoryRegionAccess(m_StartBlockAddress, other.GetCapacity(), ASANRegionPermission::AccessGranted);

            std::memcpy(m_StartBlockAddress, other.m_StartBlockAddress, MemoryBlockSize);
            m_StartBlockAddress = &m_MemoryBlock[0];
            m_EndBlockAddress = m_StartBlockAddress + MemoryBlockSize;
            m_CurrentMarker = m_StartBlockAddress + other.GetUsedBlockSize();
            other.m_StartBlockAddress = nullptr;
            other.m_EndBlockAddress = nullptr;
            other.m_CurrentMarker = nullptr;

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


    template<size_t MemoryBlockSize>
    bool StackBasedLinearAllocator<MemoryBlockSize>::operator==(const StackBasedLinearAllocator<MemoryBlockSize>& other) noexcept
    {
        return (m_CurrentMarker == other.m_CurrentMarker &&
                m_EndBlockAddress == other.m_EndBlockAddress &&
                m_StartBlockAddress == other.m_StartBlockAddress);
    }


    template<size_t MemoryBlockSize>
    bool StackBasedLinearAllocator<MemoryBlockSize>::operator!=(const StackBasedLinearAllocator<MemoryBlockSize>& other) noexcept
    {
        return !(*this == other);
    }

}
