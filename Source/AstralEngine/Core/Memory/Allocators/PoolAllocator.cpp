/**
* @file PoolAllocator.cpp
* @author Andrew Fagan
* @date 2/3/25
*/

#include "PoolAllocator.h"

namespace Core {

    PoolAllocator::PoolAllocator(size_t blockSize, size_t numberOfBlocks) :
            m_NumberOfBlocks(numberOfBlocks),
            m_BlockSize(blockSize),
            m_MemoryBlock{(unsigned char*)AllocatorUtils::AllocMaxAlignedBlock(GetTotalSize())}
    {
        ASSERT(m_MemoryBlock, "Memory block allocation failed");

        m_FreeListHead = &m_MemoryBlock[0];
        for (size_t i = 0; i < numberOfBlocks - 1; i++)
        {
            // The next free element's address is stored in the memory space of the previous free element
            *reinterpret_cast<void**>(&m_MemoryBlock[blockSize * i]) = &m_MemoryBlock[blockSize * (i + 1)];
        }
        *reinterpret_cast<void**>(&m_MemoryBlock[blockSize * (numberOfBlocks - 1)]) = nullptr;

        AllocatorUtils::SetMemoryRegionAccess(m_MemoryBlock, GetTotalSize(), ASANRegionPermission::AccessRestricted);
    }


    PoolAllocator::~PoolAllocator()
    {
        AllocatorUtils::FreeMaxAlignedBlock(m_MemoryBlock);
    }


    void* PoolAllocator::Allocate()
    {
        [[likely]] if (m_FreeListHead)
        {
            // Popping pointer of free list
            void* firstElementAddress = m_FreeListHead;  // m_HeaderPointer points to the first element address
            AllocatorUtils::SetMemoryRegionAccess(firstElementAddress, GetIndividualBlockSize(),
                                                  ASANRegionPermission::AccessGranted);
            void* secondElementAddress = *static_cast<void**>(m_FreeListHead);
            m_FreeListHead = secondElementAddress; // Can be nullptr

            // Memory Layout Example:
            // m_FreeListHead - Holds pointer to Block 1 address
            // [Free] [Block 1] - Holds pointer to Block 3 address
            // [Used] [Block 2] - Holds user data
            // [Free] [Block 3] - Holds pointer to Block 4 address
            // [Free] [Block 4] - Value inside block 4 is nullptr

            // Now returning the free pointer
            return firstElementAddress;
        }

        return nullptr;
    }


    void PoolAllocator::Free(void* elementPtr)
    {
        [[unlikely]] if (!elementPtr) return;
        ASSERT(!IsPointerFree(elementPtr), "Double free of memory block pointer has occurred!")
        ASSERT(elementPtr >= &m_MemoryBlock[0] && elementPtr <= &m_MemoryBlock[GetTotalSize() - 1], "Pointer does not fall within this pool's memory block.")

        // Pushing pointer onto the free list
        void* nextPointer = m_FreeListHead; // Can be nullptr
        m_FreeListHead = elementPtr;
        *static_cast<void**>(elementPtr) = nextPointer;
        AllocatorUtils::SetMemoryRegionAccess(elementPtr, GetIndividualBlockSize(), ASANRegionPermission::AccessRestricted);
    }


    PoolAllocator::PoolAllocator(PoolAllocator&& other) noexcept :
            m_NumberOfBlocks(other.m_NumberOfBlocks),
            m_BlockSize(other.m_BlockSize),
            m_MemoryBlock(other.m_MemoryBlock),
            m_FreeListHead(other.m_FreeListHead)
    {
        other.m_MemoryBlock = nullptr;
        other.m_FreeListHead = nullptr;
    }


    PoolAllocator& PoolAllocator::operator=(PoolAllocator&& other) noexcept
    {
        if (this != &other)
        {
            m_MemoryBlock = other.m_MemoryBlock;
            m_FreeListHead = other.m_FreeListHead;
            m_BlockSize = other.m_BlockSize;
            m_NumberOfBlocks = other.m_NumberOfBlocks;
            other.m_MemoryBlock = nullptr;
            other.m_FreeListHead = nullptr;
        }
        return *this;
    }


    bool PoolAllocator::operator==(const PoolAllocator& other) const noexcept
    {
        return (m_MemoryBlock == other.m_MemoryBlock &&
                m_FreeListHead == other.m_FreeListHead);
    }


    bool PoolAllocator::operator!=(const PoolAllocator& other) const noexcept
    {
        return !(*this == other);
    }


    bool PoolAllocator::IsPointerFree(const void* blockPointer) const
    {
        void* nextPointer = m_FreeListHead;
        if (m_FreeListHead == blockPointer) { return true; }

        while (nextPointer != nullptr)
        {
            // Temporarily change the access permission for the next pointer which should naturally be restricted normally because it is free.
            AllocatorUtils::SetMemoryRegionAccess(nextPointer, sizeof(nextPointer), ASANRegionPermission::AccessGranted);
            if (*static_cast<void**>(nextPointer) == blockPointer) { return true; } // Found pointer in free list
            nextPointer = *static_cast<void**>(nextPointer); // Can be nullptr
            AllocatorUtils::SetMemoryRegionAccess(nextPointer, sizeof(nextPointer), ASANRegionPermission::AccessRestricted);
        }
        return false;
    }

}

