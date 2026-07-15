/**
* @file PoolAllocator.h
* @author Andrew Fagan
* @date 1/26/2025
*/

#pragma once

#include "Allocator.h"
#include "AllocatorUtils.h"
#include "Core/Utilities/Asserts.h"


#include <cstring>


namespace Astral {

    /**
     * @brief Allocator that gives memory out in fixed size blocks.
     * @thread_safety This class is not thread safe.
     */
    class PoolAllocator : public IAllocator
    {
    public:

        PoolAllocator(size_t blockSize, size_t numberOfBlocks);
        ~PoolAllocator();

        /**
         * @brief Allocates a memory block from the pool and returns a pointer to it.
         * @return A pointer to the allocated memory block
         */
        void* Allocate();

        /**
         * @brief Frees an element to the pool.
         * @param elementPtr The pointer to the element being freed.
         */
        void Free(void* elementPtr);

        /**
         * @brief Resets all memory that the allocator owns. Every previous allocation is deallocated.
         */
        void Reset() override;

        /**
         * @brief Gets the amount of memory currently allocated out by the allocator.
         * @return The number of bytes currently allocated.
         */
        [[nodiscard]] size_t GetUsedBlockSize() const; // TODO

        /**
         * @brief Gets the memory capacity of the allocator.
         * @return The max number of bytes the allocator can allocate
         */
        [[nodiscard]] size_t GetCapacity() const noexcept override;

        /**
         * @brief Gets the total owned memory size of an allocator (including overhead)
         * @return The total owned memory size of an allocator (including overhead)
         */
        [[nodiscard]] size_t GetOwnedMemorySize() const override;

        /**
         * @brief Gets the allocator's type
         * @return The allocator's type
         */
        [[nodiscard]] AllocatorType GetAllocatorType() const override;

        /**
         * @brief Checks if there is at least one memory block that can be allocated.
         * @return True if there is at least one memory block that can be allocated, false otherwise
         */
        [[nodiscard]] bool CanAllocateMoreBlocks() const noexcept;

        /**
         * @brief Gets the number of blocks this pool allocator can allocate.
         * @return The number of blocks this pool allocator can allocate.
         */
        [[nodiscard]] size_t GetNumberOfBlocks() const noexcept;

        /**
         * @brief Gets the size of the memory block this pool allocator allocates.
         * @return The size of the memory block this pool allocator allocates.
         */
        [[nodiscard]] size_t GetIndividualBlockSize() const noexcept;


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
