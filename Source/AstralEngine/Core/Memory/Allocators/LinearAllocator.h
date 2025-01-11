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
        static constexpr size_t MAX_STACK_ALLOCATION_SIZE = 5280; // 5.2 KB
        static_assert(memoryBlockSize < MAX_STACK_ALLOCATION_SIZE, "Memory block size for stack is too big!");

        using value_type = T;
        using pointer = T*;
        using const_pointer = const T*;
        using reference = T&;
        using const_reference = const T&;
        using size_type = std::size_t;
        using difference_type = std::ptrdiff_t;
        using propagate_on_container_move_assignment = std::true_type;
        using propagate_on_container_copy_assignment = std::true_type;
        using propagate_on_container_swap = std::true_type;
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

        void swap(StackLinearAllocator& other) noexcept
        {
            // Swap all internal state
            std::swap(m_MemoryBlock, other.m_MemoryBlock);
            std::swap(m_StartBlockAddress, other.m_StartBlockAddress);
            std::swap(m_EndBlockAddress, other.m_EndBlockAddress);
            std::swap(m_CurrentMarker, other.m_CurrentMarker);
        }

        void deallocate(pointer ptr, size_type n)
        {
            // Does nothing. Only resets memory on call to reset()
        }

        void reset()
        {
            std::cout << "Reset called, memory use before: " << getUseMemorySize() << "\n";  // Debug
            memset(m_MemoryBlock, 0, memoryBlockSize);
            m_CurrentMarker = m_StartBlockAddress;
            std::cout << "Reset complete, memory use after: " << getUseMemorySize() << "\n";  // Debug
        };

        size_t getUseMemorySize()
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
        StackLinearAllocator(size_type) noexcept : StackLinearAllocator()
        {

        }
        ~StackLinearAllocator()
        {
            reset();
        };

        template <typename U>
        StackLinearAllocator(const StackLinearAllocator<U, memoryBlockSize>& other) noexcept
        {
            memcpy(m_MemoryBlock, other.m_MemoryBlock, memoryBlockSize);
            m_StartBlockAddress = m_MemoryBlock;
            m_EndBlockAddress = m_StartBlockAddress + memoryBlockSize;
            m_CurrentMarker = m_StartBlockAddress + (other.m_CurrentMarker - other.m_StartBlockAddress);
        }

        constexpr StackLinearAllocator(const StackLinearAllocator<T, memoryBlockSize>& other) noexcept
        {
            memcpy(m_MemoryBlock, other.m_MemoryBlock, memoryBlockSize);
            m_StartBlockAddress = m_MemoryBlock;
            m_EndBlockAddress = m_StartBlockAddress + memoryBlockSize;
            m_CurrentMarker = m_StartBlockAddress + (other.m_CurrentMarker - other.m_StartBlockAddress);
        };

        StackLinearAllocator(StackLinearAllocator<T, memoryBlockSize>&& other) noexcept
        {
            std::memcpy(m_MemoryBlock, other.m_MemoryBlock, memoryBlockSize);
            m_StartBlockAddress = m_MemoryBlock;
            m_EndBlockAddress = m_StartBlockAddress + memoryBlockSize;
            m_CurrentMarker = m_StartBlockAddress + (other.m_CurrentMarker - other.m_StartBlockAddress);

            // Reset other to initial state
            other.m_CurrentMarker = other.m_StartBlockAddress;
        };

        StackLinearAllocator& operator=(StackLinearAllocator&& other) = default;

        StackLinearAllocator& operator=(const StackLinearAllocator& other) = default;

    private:

        alignas(std::max_align_t) unsigned char m_MemoryBlock[memoryBlockSize] = {};
        unsigned char* m_StartBlockAddress = m_MemoryBlock;
        unsigned char* m_EndBlockAddress = m_StartBlockAddress + memoryBlockSize;
        unsigned char* m_CurrentMarker = m_StartBlockAddress;
    };


template <typename T, typename U, size_t memoryBlockSize>
bool operator==(const StackLinearAllocator<T, memoryBlockSize>& lhs, const StackLinearAllocator<U, memoryBlockSize>& rhs) noexcept {
    return false;
}

template <typename T, typename U, size_t memoryBlockSize>
bool operator!=(const StackLinearAllocator<T, memoryBlockSize>& lhs,
                const StackLinearAllocator<U, memoryBlockSize>& rhs) noexcept {
    return false;
}

    // Non-member swap function (required)
    template <typename T, size_t S>
    void swap(StackLinearAllocator<T,S>& lhs, StackLinearAllocator<T,S>& rhs) noexcept
    {
        lhs.swap(rhs);
    }

}