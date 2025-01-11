/**
* @file LinearAllocator.h
* @author Andrew Fagan
* @date 1/8/2025
*/

#pragma once

#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <memory>
#include <new>
#include <type_traits>


namespace Core {

    template <typename T, size_t memoryBlockSize>
    class StackLinearAllocator {
    public:
        static constexpr size_t MAX_STACK_ALLOCATION_SIZE = 5280; // 5.28 KB
        static_assert(memoryBlockSize <= MAX_STACK_ALLOCATION_SIZE, "Memory block size for stack is too big!");

        using value_type = T;
        using pointer = T*;
        using const_pointer = const T*;
        using reference = T&;
        using const_reference = const T&;
        using size_type = std::size_t;
        using difference_type = std::ptrdiff_t;
        using propagate_on_container_move_assignment = std::true_type;
        using propagate_on_container_copy_assignment = std::true_type;
        using is_always_equal = std::false_type; // This NEEDS to be false for stateful allocators!!!


        pointer allocate(size_type n, const void* hint = nullptr)
        {
            const size_t allocatedBytes = n * sizeof(T);
            if (m_CurrentMarker + allocatedBytes > m_EndBlockAddress)
            {
                throw std::bad_alloc();
            }

            // Ensure proper alignment
            std::size_t space = m_EndBlockAddress - m_CurrentMarker;
            void* alignedAddress = m_CurrentMarker;
            if (std::align(alignof(T), allocatedBytes, alignedAddress, space))
            {
                void* returnPointer = alignedAddress;
                m_CurrentMarker = static_cast<unsigned char*>(alignedAddress) + allocatedBytes;
                return static_cast<pointer>(returnPointer);
            }

            throw std::bad_alloc();
        }


        void deallocate(pointer ptr, size_type n)
        {
            // Does nothing. Only resets memory on call to reset()
        }

        void reset()
        {
            memset(m_MemoryBlock, 0, memoryBlockSize);
            m_CurrentMarker = m_StartBlockAddress;
        }

        size_t getUsedBlockSize()
        {
            return m_CurrentMarker - m_StartBlockAddress;
        }

        // Rebind struct
        template <typename U>
        struct rebind
        {
            using other = StackLinearAllocator<U, memoryBlockSize>;
        };

        StackLinearAllocator() noexcept = default;
        StackLinearAllocator(size_type) noexcept : StackLinearAllocator() {}
        ~StackLinearAllocator() { reset(); }


        constexpr StackLinearAllocator(const StackLinearAllocator& other) noexcept
        {
            memcpy(m_MemoryBlock, other.m_MemoryBlock, memoryBlockSize);
            m_StartBlockAddress = m_MemoryBlock;
            m_EndBlockAddress = m_StartBlockAddress + memoryBlockSize;
            m_CurrentMarker = m_StartBlockAddress + (other.m_CurrentMarker - other.m_StartBlockAddress);
        };

        StackLinearAllocator(StackLinearAllocator&& other) noexcept
        {
            std::memcpy(m_MemoryBlock, other.m_MemoryBlock, memoryBlockSize);
            m_StartBlockAddress = m_MemoryBlock;
            m_EndBlockAddress = m_StartBlockAddress + memoryBlockSize;
            m_CurrentMarker = m_StartBlockAddress + (other.m_CurrentMarker - other.m_StartBlockAddress);
            other.m_CurrentMarker = other.m_StartBlockAddress;
        };

        StackLinearAllocator& operator=(const StackLinearAllocator& other) = default;
        StackLinearAllocator& operator=(StackLinearAllocator&& other) = default;


    private:

        alignas(std::max_align_t) unsigned char m_MemoryBlock[memoryBlockSize] = {};
        unsigned char* m_StartBlockAddress = m_MemoryBlock;
        unsigned char* m_EndBlockAddress = m_StartBlockAddress + memoryBlockSize;
        unsigned char* m_CurrentMarker = m_StartBlockAddress;
    };


    template <typename T, typename U, size_t memoryBlockSize>
    bool operator==(const StackLinearAllocator<T, memoryBlockSize>& a1, const StackLinearAllocator<U, memoryBlockSize>& a2) noexcept
    {
        return false;
    }

    template <typename T, typename U, size_t memoryBlockSize>
    bool operator!=(const StackLinearAllocator<T, memoryBlockSize>& a1, const StackLinearAllocator<U, memoryBlockSize>& a2) noexcept
    {
        return false;
    }

}