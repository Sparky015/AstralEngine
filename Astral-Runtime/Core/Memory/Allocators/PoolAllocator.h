/**
* @file PoolAllocator.h
* @author Andrew Fagan
* @date 1/26/2025
*/

#pragma once

#include "AllocatorUtils.h"
#include "Debug/Utilities/Asserts.h"
#include <cstring>

namespace Astral {

    /**@brief Allocator that gives memory out in fixed size blocks.
     * @thread_safety This class is NOT thread safe.
     * @note Copying is not allowed with this allocator. */
    class PoolAllocator
    {
    public:

        PoolAllocator(size_t blockSize, size_t numberOfBlocks);
        ~PoolAllocator();

        /**@brief Allocates a memory block from the pool and returns a pointer to it.
         * @return A pointer to the allocated memory block */
        void* Allocate();

        /**@brief Frees an element to the pool.
         * @param elementPtr The pointer to the element being freed. */
        void Free(void* elementPtr);

        /**@brief Checks if there are is at least one memory block that can be allocated. */
        [[nodiscard]] constexpr bool CanAllocateMoreBlocks() const noexcept { return m_FreeListHead != nullptr; }

        /**@brief Gets the number of blocks this pool allocator can allocate. */
        [[nodiscard]] constexpr size_t GetNumberOfBlocks() const noexcept { return m_NumberOfBlocks; }

        /**@brief Gets the size of the memory block this pool allocator allocates. */
        [[nodiscard]] constexpr size_t GetIndividualBlockSize() const noexcept { return m_BlockSize; }

        /**@brief Gets the number of bytes this allocator can allocate out. */
        [[nodiscard]] constexpr size_t GetTotalSize() const noexcept { return m_NumberOfBlocks * m_BlockSize; }


        // Deleting copy constructor and operator because copied blocks that were in use won't be able to be freed.
        // The new PoolAllocator will copy same blocks (some of which will be free and some won't be free).
        // Because the new PoolAllocator's memory is a freshly allocated, no one has the pointers to the new PoolAllocator's memory in use.
        // so they can't be freed.
        PoolAllocator(const PoolAllocator& other) = delete;
        PoolAllocator& operator=(const PoolAllocator& other) = delete;

        PoolAllocator(PoolAllocator&& other) noexcept;
        PoolAllocator& operator=(PoolAllocator&& other) noexcept;

        bool operator==(const PoolAllocator& other) const noexcept;
        bool operator!=(const PoolAllocator& other) const noexcept;

    private:

        /**@brief Checks if a pointer is free. Intended for debug use only through ASSERT macros.
         * @param blockPointer The pointer being checked.
         * @return True if the pointer is free and ready to be allocated, false otherwise. */
        bool IsPointerFree(const void* blockPointer) const;

        size_t m_NumberOfBlocks;
        size_t m_BlockSize;
        unsigned char* m_MemoryBlock;
        void* m_FreeListHead; // Points to the first free element address
    };

}