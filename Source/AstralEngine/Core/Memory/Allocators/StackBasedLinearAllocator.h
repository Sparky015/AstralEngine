/**
* @file StackBasedLinearAllocator.h
* @author Andrew Fagan
* @date 1/8/2025
*/

#pragma once

#include "AllocatorUtils.h"
#include "Core/CoreMacroDefinitions.h"
#include "Core/Memory/Tracking/NewDeleteOverrides.h"
#include "AstralEngine/Debug/Utils/Public/Asserts.h"
#include <cstddef>
#include <cstring>
#include <memory>
#include <new>


namespace Core {

    /**@brief A stack-based linear allocator. Max allocation size is 5.28 KB due to being on the stack.
     *        Deallocate method does nothing. Reset method deallocates the whole memory block.
     * @warning You have to use the Reset method to Deallocate memory. It deallocates all memory being used.
     *          It's all or nothing.
     * @thread_safety This class is NOT thread safe. */
    template<size_t MemoryBlockSize>
    class StackBasedLinearAllocator {
    public:
        static constexpr size_t MAX_STACK_ALLOCATION_SIZE = 5280; // 5.28 KB
        static_assert(MemoryBlockSize <= MAX_STACK_ALLOCATION_SIZE, "Memory block size for stack is too big!");

        StackBasedLinearAllocator();
        ~StackBasedLinearAllocator();


        /**@brief Allocates a memory block of the given size with the given required alignment.
         * @param size Size of the requested allocated block
         * @param alignment The alignment requirement for the allocation
         * @return A pointer to the allocated block or nullptr if the allocation failed. */
        void* Allocate(size_t size, uint16 alignment);

        /**@brief Resets ALL memory that the allocator owns. Everything gets deallocated. */
        void Reset();

        /**@brief Gets the amount of memory currently allocated out by the allocator. */
        [[nodiscard]] size_t GetUsedBlockSize() const { return m_CurrentMarker - m_StartBlockAddress; }

        /**@brief Gets the memory capacity of the allocator. */
        [[nodiscard]] size_t GetCapacity() const { return m_EndBlockAddress - m_StartBlockAddress; }


        // Deleting copy constructor and operator because the copied data in use won't be able to be accessed.
        // The new allocator will copy the same address range (some of that will include addresses in use by the user).
        // Because the new allocator's memory is a freshly allocated, no one has the pointers to the new allocator's memory
        // for the data in use, so they won't be accessed. At that point, just make a regular instance.
        StackBasedLinearAllocator(const StackBasedLinearAllocator& other) = delete;
        StackBasedLinearAllocator& operator=(const StackBasedLinearAllocator& other) = delete;

        StackBasedLinearAllocator(StackBasedLinearAllocator&& other) noexcept;
        StackBasedLinearAllocator& operator=(StackBasedLinearAllocator&& other) noexcept;

        bool operator==(const StackBasedLinearAllocator<MemoryBlockSize>& other) noexcept;
        bool operator!=(const StackBasedLinearAllocator<MemoryBlockSize>& other) noexcept;


    private:

        alignas(std::max_align_t) unsigned char m_MemoryBlock[MemoryBlockSize];
        unsigned char* m_StartBlockAddress;
        unsigned char* m_EndBlockAddress;
        unsigned char* m_CurrentMarker;
    };

}

#include "StackBasedLinearAllocator_impl.h"