/**
* @file StackLinearAllocator.h
* @author Andrew Fagan
* @date 1/8/2025
*/

#pragma once

#include <cstddef>
#include <cstring>
#include <memory>
#include <new>


namespace Core {

    /**@brief A stack-based linear allocator. Max allocation size is 5.28 KB due to being on the stack.
     *        Deallocate method does nothing. Reset method deallocates the whole memory block.
     * @warning You have to use the reset method to deallocate memory. It deallocates all memory being used.
     *          It's all or nothing. */
    template <typename T, size_t memoryBlockSize>
    class StackLinearAllocator
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


        /**@brief Allocates memory for n instances of the type of allocator. Hint is completely ignored. */
        pointer allocate(size_type numberOfElements, const void* hint = nullptr)
        {
            const size_t allocatedBytes = numberOfElements * sizeof(T);
            if (m_CurrentMarker + allocatedBytes > m_EndBlockAddress)
            {
                throw std::bad_alloc();
            }


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


        /**@brief This does nothing. Use reset method to deallocate memory. */
        void deallocate(pointer ptr, size_type numberOfElements)
        {
            // Does nothing. Only resets memory on call to reset()
        }

        /**@brief Resets ALL memory that the allocator owns. Everything gets deallocated. */
        void reset()
        {
            memset(m_MemoryBlock, 0, memoryBlockSize);
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


        StackLinearAllocator& operator=(const StackLinearAllocator& other) noexcept
        {
            if (this != &other)
            {
                memcpy(m_MemoryBlock, other.m_MemoryBlock, memoryBlockSize);
                m_StartBlockAddress = m_MemoryBlock;
                m_EndBlockAddress = m_StartBlockAddress + memoryBlockSize;
                m_CurrentMarker = m_StartBlockAddress + (other.m_CurrentMarker - other.m_StartBlockAddress);
            }

            return *this;
        }

        StackLinearAllocator(StackLinearAllocator&& other) noexcept
        {
            std::memcpy(m_MemoryBlock, other.m_MemoryBlock, memoryBlockSize);
            m_StartBlockAddress = m_MemoryBlock;
            m_EndBlockAddress = m_StartBlockAddress + memoryBlockSize;
            m_CurrentMarker = m_StartBlockAddress + (other.m_CurrentMarker - other.m_StartBlockAddress);
            other.m_CurrentMarker = other.m_StartBlockAddress;
        };


        StackLinearAllocator& operator=(StackLinearAllocator&& other) noexcept
        {
            std::memcpy(m_MemoryBlock, other.m_MemoryBlock, memoryBlockSize);
            m_StartBlockAddress = m_MemoryBlock;
            m_EndBlockAddress = m_StartBlockAddress + memoryBlockSize;
            m_CurrentMarker = m_StartBlockAddress + (other.m_CurrentMarker - other.m_StartBlockAddress);
            other.m_CurrentMarker = other.m_StartBlockAddress;
            return *this;
        };


    private:

        alignas(std::max_align_t) unsigned char m_MemoryBlock[memoryBlockSize] = {};
        unsigned char* m_StartBlockAddress = m_MemoryBlock;
        unsigned char* m_EndBlockAddress = m_StartBlockAddress + memoryBlockSize;
        unsigned char* m_CurrentMarker = m_StartBlockAddress;

        template<typename T1, typename T2, size_t S>
        friend bool operator==(const StackLinearAllocator<T1, S>& a1, const StackLinearAllocator<T2, S>& a2) noexcept;

        template<typename T1, typename T2, size_t S>
        friend bool operator!=(const StackLinearAllocator<T1, S>& a1, const StackLinearAllocator<T2, S>& a2) noexcept;
    };


    template <typename T, typename U, size_t memoryBlockSize>
    bool operator==(const StackLinearAllocator<T, memoryBlockSize>& a1, const StackLinearAllocator<U, memoryBlockSize>& a2) noexcept
    {
        return (a1.m_CurrentMarker == a2.m_CurrentMarker &&
            &a1.m_MemoryBlock == &a2.m_MemoryBlock &&
            a1.m_EndBlockAddress == a2.m_EndBlockAddress &&
            a1.m_StartBlockAddress == a2.m_StartBlockAddress);
    }

    template <typename T, typename U, size_t memoryBlockSize>
    bool operator!=(const StackLinearAllocator<T, memoryBlockSize>& a1, const StackLinearAllocator<U, memoryBlockSize>& a2) noexcept
    {
        return !(a1 == a2);
    }

}