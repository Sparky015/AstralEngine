/**
* @file StackAllocator.h
* @author Andrew Fagan
* @date 1/15/2025
*/


#pragma once

#include "IAllocator.h"
#include "AllocatorUtils.h"
#include "Core/FixedIntegerTypes.h"

namespace Astral {

    /**
     * @brief Stack-like allocator that allocates memory in a last in first out order. This means that the user can
     *        deallocate only the most recent unfreed memory allocation.
     * @thread_safety This class is not thread safe.
     */
    class StackAllocator : public IAllocator
    {
    public:
        explicit StackAllocator(size_t memoryBlockSize);
        ~StackAllocator();

        using Marker = unsigned char*;

        /**
         * @brief Gets a marker to the top of the memory block
         * @return A marker to the top of the memory block
         */
        [[nodiscard]] Marker GetMarker() const { return m_CurrentMarker; }

        /**
         * @brief Rolls the stack back to the passed marker. Deallocates memory that was allocated after the marker.
         * @param marker The marker to roll back to
         */
        void RollbackToMarker(const Marker marker);

        /**
         * @brief Allocates a memory block of the given size with the given required alignment.
         * @param size Size of the requested allocated block
         * @param alignment The alignment requirement for the allocation
         * @return A pointer to the allocated block or nullptr if the allocation failed.
         */
        void* Allocate(size_t size, uint16 alignment);

        /**
         * @brief Deallocates the memory block at the pointer
         * @warning You can only Deallocate the previous allocation. This allocator follows a last in first out approach
         */
        void Deallocate(void* ptr, size_t sizeOfAllocatedBlock);

        /**
         * @brief Resets all memory that the allocator owns. Every previous allocation is deallocated.
         */
        void Reset() override;

        /**
         * @brief Gets the amount of memory currently allocated out by the allocator.
         * @return The number of bytes currently allocated.
         */
        [[nodiscard]] size_t GetUsedBlockSize() const override;

        /**
         * @brief Gets the memory capacity of the allocator.
         * @return The max number of bytes the allocator can allocate
         */
        [[nodiscard]] size_t GetCapacity() const override;

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
         * @brief Doubles the size of the internal buffer of the allocator.
         * @return True if the resize operation was successful and false if the operation failed.
         * @note Only resizes when the allocator is empty. If it is not empty then this function does nothing.
         */
        [[nodiscard]] bool ResizeBuffer();


        // Deleting copy constructor and operator because the copied data in use won't be able to be freed.
        // The new allocator will copy the same address range (some of that will include addresses in use by the user).
        // Because the new allocator's memory is a freshly allocated, no one has the pointers to the new allocator's memory
        // for the data in use, so they can't be freed.
        StackAllocator(const StackAllocator& other) = delete;
        StackAllocator& operator=(const StackAllocator& other) = delete;

        StackAllocator(StackAllocator&& other) noexcept;
        StackAllocator& operator=(StackAllocator&& other) noexcept;

        bool operator==(const StackAllocator& other) noexcept;
        bool operator!=(const StackAllocator& other) noexcept;

    private:

        /**
         * @brief Attempts to resize the internal buffer of the allocator.
         * @return True if the resize operation was successful and false if the operation failed.
         */
        [[nodiscard]] inline bool ResizeInternalMemoryBlock();

        unsigned char* m_StartBlockAddress;
        unsigned char* m_EndBlockAddress;
        unsigned char* m_CurrentMarker;
    };

}


