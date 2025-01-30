/**
* @file STLStackBasedLinearAllocator.h
* @author Andrew Fagan
* @date 1/8/2025
*/

#pragma once

#include "Core/Memory/Tracking/AllocationTracker.h"
#include <cstddef>
#include <cstring>
#include <memory>
#include <new>

#include "Core/Memory/Allocators/AllocatorUtils.h"


namespace Core {

    /**@brief STL compliant version of StackBasedLinearAllocator. It is a stack-based linear allocator. Max allocation size
     *        is 5.28 KB due to being on the stack. Deallocate method does nothing. Reset method deallocates
     *        the whole memory block.
     * @warning You have to use the Reset method to Deallocate memory. It deallocates all memory being used.
     *          It's all or nothing.
     * @thread_safety This class is NOT thread safe. */
    template <typename T, size_t memoryBlockSize>
    class STLStackBasedLinearAllocator
    {
    public:
        static constexpr size_t MAX_STACK_ALLOCATION_SIZE = 5280; // 5.28 KB
        static_assert(memoryBlockSize <= MAX_STACK_ALLOCATION_SIZE, "Memory block size for stack is too big!");

        using value_type = T;
        using pointer = T*;
        using size_type = std::size_t;
        using difference_type = std::ptrdiff_t;
        using propagate_on_container_move_assignment = std::true_type;
        using propagate_on_container_copy_assignment = std::true_type;
        using is_always_equal = std::false_type; // This needs to be false for stateful allocators!


        STLStackBasedLinearAllocator() = default;

        STLStackBasedLinearAllocator(size_type) noexcept : STLStackBasedLinearAllocator() {}

        ~STLStackBasedLinearAllocator()
        {
            reset();
        }


        /**@brief Allocates memory for n instances of the type of allocator. Hint is completely ignored. */
        pointer allocate(size_type numberOfElements, const void* hint = nullptr)
        {
            const size_t allocatedBytes = numberOfElements * sizeof(T);
            if (AllocatorUtils::DoesCauseOverflow(m_CurrentMarker, allocatedBytes, m_EndBlockAddress))
            {
                throw std::bad_alloc();
            }


            std::size_t space = m_EndBlockAddress - m_CurrentMarker;
            void* alignedAddress = m_CurrentMarker;
            if (std::align(alignof(T), allocatedBytes, alignedAddress, space))
            {
                void* returnPointer = alignedAddress;
                m_CurrentMarker = static_cast<unsigned char*>(alignedAddress) + allocatedBytes;
                TRACK_ALLOCATION(allocatedBytes);
                return static_cast<pointer>(returnPointer);
            }

            throw std::bad_alloc();
        }


        /**@brief This does nothing. Use Reset method to Deallocate memory. */
        void deallocate(pointer ptr, size_type numberOfElements)
        {
            // Does nothing. Only resets memory on call to reset()
        }

        /**@brief Resets ALL memory that the allocator owns. Everything gets deallocated. */
        void reset()
        {
            TRACK_DEALLOCATION(m_CurrentMarker - m_StartBlockAddress);
            m_CurrentMarker = m_StartBlockAddress;
        }

        /**@brief Gets the amount of memory currently allocated out by the allocator. */
        size_t getUsedBlockSize()
        {
            return m_CurrentMarker - m_StartBlockAddress;
        }

        // Rebind struct
        template <typename U>
        struct rebind
        {
            using other = STLStackBasedLinearAllocator<U, memoryBlockSize>;
        };

        STLStackBasedLinearAllocator(const STLStackBasedLinearAllocator& other)
        {

        }

        STLStackBasedLinearAllocator& operator=(const STLStackBasedLinearAllocator& other)
        {
            if (this != &other)
            {

            }
            return *this;
        }

        STLStackBasedLinearAllocator(STLStackBasedLinearAllocator&& other) noexcept
        {

        }

        STLStackBasedLinearAllocator& operator=(STLStackBasedLinearAllocator&& other) noexcept
        {
            if (this != &other)
            {

            }
            return *this;
        }


        template <typename U>
        bool operator==(const STLStackBasedLinearAllocator<U, memoryBlockSize>& other) noexcept
        {
            return (m_CurrentMarker == other.m_CurrentMarker &&
                    m_EndBlockAddress == other.m_EndBlockAddress &&
                    m_StartBlockAddress == other.m_StartBlockAddress);
        }

        template <typename U>
        bool operator!=(const STLStackBasedLinearAllocator<U, memoryBlockSize>& other) noexcept
        {
            return !(*this == other);
        }

    private:

        alignas(std::max_align_t) unsigned char m_MemoryBlock[memoryBlockSize] = {};
        unsigned char* m_StartBlockAddress = m_MemoryBlock;
        unsigned char* m_EndBlockAddress = m_StartBlockAddress + memoryBlockSize;
        unsigned char* m_CurrentMarker = m_StartBlockAddress;

    };



}
