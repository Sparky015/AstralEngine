/**
* @file PoolAllocator.h
* @author Andrew Fagan
* @date 1/26/2025
*/

#pragma once

#include "AllocatorUtils.h"
#include "Debug/Macros/Asserts.h"
#include <cstring>

namespace Core {

    /**@brief Allocator that gives memory out in fixed size blocks.
     * @thread_safety This class is NOT thread safe. */
    class PoolAllocator
    {
    public:

        PoolAllocator(size_t blockSize, size_t numberOfBlocks) :
                m_NumberOfBlocks(numberOfBlocks),
                m_BlockSize(blockSize),
                m_MemoryBlock{(unsigned char*)AllocatorUtils::AllocMaxAlignedBlock(GetTotalSize())}
        {

            m_FreeListHead = &m_MemoryBlock[0];
            for (size_t i = 0; i < numberOfBlocks - 1; i++)
            {
                // The next free element's address is stored in the memory space of the previous free element
                *reinterpret_cast<void**>(&m_MemoryBlock[blockSize * i]) = &m_MemoryBlock[blockSize * (i + 1)];
            }
            *reinterpret_cast<void**>(&m_MemoryBlock[blockSize * (numberOfBlocks - 1)]) = nullptr;
        }

        ~PoolAllocator()
        {
            AllocatorUtils::FreeMaxAlignedBlock(m_MemoryBlock);
            AllocatorUtils::SetMemoryRegionBoundary(m_MemoryBlock, GetTotalSize(), AllocatorUtils::FreedMemory);
        }

        /**@brief Allocates a memory block from the pool and returns a pointer to it.
         * @return A pointer to the allocated memory block */
        void* Allocate()
        {
            [[likely]] if (m_FreeListHead)
            {
                // Popping pointer of free list
                void* firstElementAddress = m_FreeListHead;  // m_HeaderPointer points to the first element address
                void* secondElementAddress = *static_cast<void**>(m_FreeListHead);
                m_FreeListHead = secondElementAddress; // Can be nullptr

                // Now returning the free pointer
                return firstElementAddress;
            }

            return nullptr;
        }

        /**@brief Frees an element to the pool.
         * @param elementPtr The pointer to the element being freed. */
        void Free(void* elementPtr)
        {
            [[unlikely]] if (!elementPtr) return;
            ASSERT(!IsPointerFree(elementPtr), "Double free of memory block pointer has occurred!")
            ASSERT(elementPtr >= &m_MemoryBlock[0] && elementPtr <= &m_MemoryBlock[GetTotalSize() - 1], "Pointer does not fall within this pool's memory block.")

            // Pushing pointer onto the free list
            void* nextPointer = m_FreeListHead; // Can be nullptr
            m_FreeListHead = elementPtr;
            *static_cast<void**>(elementPtr) = nextPointer;
        }

        /**@brief Checks if there are is at least one memory block that can be allocated. */
        [[nodiscard]] constexpr bool CanAllocateMoreBlocks() const noexcept { return m_FreeListHead != nullptr; }

        /**@brief Gets the number of blocks this pool allocator can allocate. */
        [[nodiscard]] constexpr size_t GetNumberOfBlocks() const noexcept { return m_NumberOfBlocks; }

        /**@brief Gets the size of the memory block this pool allocator allocates. */
        [[nodiscard]] constexpr size_t GetSizeOfBlocks() const noexcept { return m_BlockSize; }

        /**@brief Gets the number of bytes this allocator can allocate out. */
        [[nodiscard]] constexpr size_t GetTotalSize() const noexcept { return m_NumberOfBlocks * m_BlockSize; }

        PoolAllocator(PoolAllocator&& other) noexcept :
            m_NumberOfBlocks(other.m_NumberOfBlocks),
            m_BlockSize(other.m_BlockSize),
            m_MemoryBlock(other.m_MemoryBlock),
            m_FreeListHead(other.m_FreeListHead)
        {
            other.m_MemoryBlock = nullptr;
            other.m_FreeListHead = nullptr;
        }

        PoolAllocator& operator=(PoolAllocator&& other) noexcept
        {
            if (this != &other)
            {
                AllocatorUtils::FreeMaxAlignedBlock(m_MemoryBlock);
                m_MemoryBlock = other.m_MemoryBlock;
                m_FreeListHead = other.m_FreeListHead;
                other.m_MemoryBlock = nullptr;
                other.m_FreeListHead = nullptr;
            }
            return *this;
        }

        PoolAllocator(const PoolAllocator& other) :
            m_NumberOfBlocks(other.m_NumberOfBlocks),
            m_BlockSize(other.m_BlockSize),
            m_MemoryBlock{(unsigned char*)AllocatorUtils::AllocMaxAlignedBlock(GetTotalSize())},
            m_FreeListHead(other.m_FreeListHead)
        {
            std::memcpy(m_MemoryBlock, other.m_MemoryBlock, m_NumberOfBlocks * m_BlockSize);
        }

        PoolAllocator& operator=(const PoolAllocator& other)
        {
            if (this != &other)
            {
                AllocatorUtils::FreeMaxAlignedBlock(m_MemoryBlock);
                m_MemoryBlock = (unsigned char*)AllocatorUtils::AllocMaxAlignedBlock(GetTotalSize());
                m_FreeListHead = other.m_FreeListHead;
            }
            return *this;
        }


        bool operator==(const PoolAllocator& other) noexcept
        {
            return (m_MemoryBlock == other.m_MemoryBlock &&
                    m_FreeListHead == other.m_FreeListHead);
        }

        bool operator!=(const PoolAllocator& other) noexcept
        {
            return !(*this == other);
        }

    private:

        /**@brief Checks if a pointer is free. Intended for debug use only through ASSERT macros.
         * @param blockPointer The pointer being checked.
         * @return True if the pointer is free and ready to be allocated, false otherwise. */
        bool IsPointerFree(const void* blockPointer) const
        {
            void* nextPointer = m_FreeListHead;
            if (m_FreeListHead == blockPointer) { return true; }

            while (nextPointer != nullptr)
            {
                if (*static_cast<void**>(nextPointer) == blockPointer) { return true; } // Found pointer in free list
                nextPointer = *static_cast<void**>(nextPointer); // Can be nullptr
            }
            return false;
        }

        const size_t m_NumberOfBlocks;
        const size_t m_BlockSize;
        unsigned char* m_MemoryBlock;
        void* m_FreeListHead; // Points to the first free element address
    };

}