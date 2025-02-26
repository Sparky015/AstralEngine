/**
* @file RingAllocator.h
* @author Andrew Fagan
* @date 2/4/2025
*/

#pragma once


#include "AllocatorUtils.h"
#include "Core/CoreMacroDefinitions.h"
#include "Debug/Macros/Asserts.h"
#include <cstddef>
#include <cstring>
#include <memory>
#include <new>


namespace Core {

    /**@brief This is an allocator that allows for a continuous stream of allocations. It is basically a buffer that
     *        can be allocated to store temporary data.
     * @thread_safety This class is NOT thread safe. */
    class RingAllocator {
    public:

        explicit RingAllocator(size_t memoryBlockSize);
        ~RingAllocator();

        /**@brief Allocates a memory block of the given size with the given required alignment.
         * @param size Size of the requested allocated block
         * @param alignment The alignment requirement for the allocation
         * @param outAllocatedPointer An output parameter that is populated with a pointer to allocated block.
         * @return A pointer to the allocated block if successful and nullptr if the allocation failed.*/
        [[nodiscard]] void* Allocate(size_t size, uint16 alignment);

        /**@brief Resets ALL memory that the allocator owns. Everything gets deallocated. */
        void Reset();

        /**@brief Gets the memory capacity of the allocator.
         * @return The max number of bytes the allocator can allocate. */
        [[nodiscard]] size_t GetCapacity() const { return m_EndBlockAddress - m_StartBlockAddress; }

        /**@brief Checks if an allocation of the given size can be allocated with the available space in the allocator
         * @param size The size of the allocation being checked
         * @return True if an allocation of the given size can be allocated and false if
         *         the allocation will fail due to not enough space in the allocator. */
        [[nodiscard]] bool CanAllocate(size_t size) const { return size <= GetCapacity(); }

        /**@brief Checks if an allocation will wrap to the start of the allocator's memory block.
         *        (This is due to the ring aspect of the ring allocator)
         * @param size The size of the allocation
         * @param alignment The alignment of the allocation
         * @return True if the allocation will be wrapped to the start of the allocator's memory block and
         *         false if it will not be wrapped.
         * @note Either the entire allocation will wrap around or the allocation won't wrap. */
        [[nodiscard]] bool DoesAllocationWrap(size_t size, size_t alignment) const
        {
            if (AllocatorUtils::DoesCauseOverflow(m_CurrentMarker, size, m_EndBlockAddress)) { return true; };
            void* testAddress = (void*)m_CurrentMarker;
            size_t space = m_EndBlockAddress - m_CurrentMarker;
            if (!std::align(alignment, size, testAddress, space)) { return true; }

            return false;
        }

        /**@brief Doubles the size of the internal buffer of the allocator.
         * @return True if the resize operation succeeded and false if the allocation failed.
         * @warning This will invalidate all data currently in the memory of the allocator. Only use this when you are sure
         *          that the memory previously allocated won't be read/wrote to again. */
        [[nodiscard]] bool ResizeBuffer();


        // Deleting copy constructor and operator because the copied data in use won't be able to be accessed.
        // The new allocator will copy the same address range (some of that will include addresses in use by the user).
        // Because the new allocator's memory is a freshly allocated, no one has the pointers to the new allocator's memory
        // for the data in use, so they won't be accessed. At that point, just make a regular instance.
        RingAllocator(const RingAllocator& other) = delete;
        RingAllocator& operator=(const RingAllocator& other) = delete;

        RingAllocator(RingAllocator&& other) noexcept;
        RingAllocator& operator=(RingAllocator&& other) noexcept;

        bool operator==(const RingAllocator& other) noexcept;
        bool operator!=(const RingAllocator& other) noexcept;

    private:

        /** @brief Attempts to resize the internal buffer of the allocator. This should only be used when the
         *         allocator is empty.
         *  @return True if the resize allocation succeeds and false if it fails.
         *  @remark Function will exit early and maintain current allocator capacity if the resize allocation fails.  */
        [[nodiscard]] bool ResizeInternalMemoryBlock();

        unsigned char* m_StartBlockAddress;
        unsigned char* m_EndBlockAddress;
        unsigned char* m_CurrentMarker;
    };

}
