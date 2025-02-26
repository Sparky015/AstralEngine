/**
* @file LinearAllocator.h
* @author Andrew Fagan
* @date 1/29/25
*/

#pragma once

#include "AllocatorUtils.h"
#include "Core/CoreMacroDefinitions.h"
#include "Debug/Utils/Public/Asserts.h"
#include <cstddef>
#include <cstring>
#include <memory>
#include <new>


namespace Core {

    /**@brief A heap-based linear allocator. Deallocate method does nothing. Reset method deallocates the whole memory block.
     * @warning You have to use the Reset method to Deallocate memory. It deallocates all memory being used.
     *          It's all or nothing.
     * @thread_safety This class is NOT thread safe. */
    class LinearAllocator {
    public:

        explicit LinearAllocator(size_t memoryBlockSize);
        ~LinearAllocator();

        /**@brief Allocates a memory block of the given size with the given required alignment.
         * @param size Size of the requested allocated block
         * @param alignment The alignment requirement for the allocation
         * @param outAllocatedPointer An output parameter that is populated with a pointer to allocated block.
         * @return A pointer to the allocated block if successful and nullptr if the allocation failed.*/
        [[nodiscard]] void* Allocate(size_t size, uint16 alignment);

        /**@brief Resets ALL memory that the allocator owns. Everything gets deallocated. */
        void Reset();

        /**@brief Gets the amount of memory currently allocated out by the allocator.
         * @return The number of bytes currently allocated. */
        [[nodiscard]] size_t GetUsedBlockSize() const { return m_CurrentMarker - m_StartBlockAddress; }

        /**@brief Gets the memory capacity of the allocator.
         * @return The max number of bytes the allocator can allocate. */
        [[nodiscard]] size_t GetCapacity() const { return m_EndBlockAddress - m_StartBlockAddress; }

        /**@brief Doubles the size of the internal buffer of the allocator.
         * @return True if the resize operation succeeded and false if the allocation failed or if the allocator was not empty.
         * @note Only resizes when the allocator is empty. If it is not empty then this function does nothing. */
        [[nodiscard]] bool ResizeBuffer();


        // Deleting copy constructor and operator because the copied data in use won't be able to be accessed.
        // The new allocator will copy the same address range (some of that will include addresses in use by the user).
        // Because the new allocator's memory is a freshly allocated, no one has the pointers to the new allocator's memory
        // for the data in use, so they won't be accessed. At that point, just make a regular instance.
        LinearAllocator(const LinearAllocator& other) = delete;
        LinearAllocator& operator=(const LinearAllocator& other) = delete;

        LinearAllocator(LinearAllocator&& other) noexcept;
        LinearAllocator& operator=(LinearAllocator&& other) noexcept;

        bool operator==(const LinearAllocator& other) noexcept;
        bool operator!=(const LinearAllocator& other) noexcept;

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
