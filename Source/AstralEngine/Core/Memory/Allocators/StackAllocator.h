//
// Created by Andrew Fagan on 1/15/25.
//

#pragma once

#include <memory>

namespace Core {

    template <typename T, size_t memoryBlockSize>
    class StackAllocator
    {
    public:
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
                m_LastAllocatedNonAlignedAddress = m_CurrentMarker;
                m_CurrentMarker = static_cast<unsigned char*>(alignedAddress) + allocatedBytes;
                return static_cast<pointer>(returnPointer);
            }

            throw std::bad_alloc();
        }


        /**@brief Resets all memory that was allocated after the pointer.
         * @warning Even if there is memory after the passed in pointer which the pointer doesn't own, it will still
         *          be deallocated. */
        void deallocate(pointer ptr, size_type numberOfElements)
        {
            if (!m_LastAllocatedPointer) { return; } // if there is no previously allocated pointer, do nothing

            if (m_LastAllocatedPointer != ptr)
                { throw std::runtime_error("Given pointer can not be deallocated because it was not the last allocated pointer!"); }

            m_CurrentMarker = m_LastAllocatedNonAlignedAddress;
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
            using other = StackAllocator<U, memoryBlockSize>;
        };

        StackAllocator() noexcept = default;
        StackAllocator(size_type memoryBlock) noexcept : StackAllocator() {}
        ~StackAllocator() { reset(); }


        constexpr StackAllocator(const StackAllocator& other) noexcept
        {
            memcpy(m_MemoryBlock, other.m_MemoryBlock, memoryBlockSize);
            m_StartBlockAddress = m_MemoryBlock;
            m_EndBlockAddress = m_StartBlockAddress + memoryBlockSize;
            m_CurrentMarker = m_StartBlockAddress + (other.m_CurrentMarker - other.m_StartBlockAddress);
        };


        StackAllocator& operator=(const StackAllocator& other) noexcept
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

        StackAllocator(StackAllocator&& other) noexcept
        {
            std::memcpy(m_MemoryBlock, other.m_MemoryBlock, memoryBlockSize);
            m_StartBlockAddress = m_MemoryBlock;
            m_EndBlockAddress = m_StartBlockAddress + memoryBlockSize;
            m_CurrentMarker = m_StartBlockAddress + (other.m_CurrentMarker - other.m_StartBlockAddress);
            other.m_CurrentMarker = other.m_StartBlockAddress;
        };


        StackAllocator& operator=(StackAllocator&& other) noexcept
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

        unsigned char* m_LastAllocatedNonAlignedAddress = m_StartBlockAddress;
        pointer m_LastAllocatedPointer = nullptr;

        template<typename T1, typename T2, size_t S>
        friend bool operator==(const StackAllocator<T1, S>& a1, const StackAllocator<T2, S>& a2) noexcept;

        template<typename T1, typename T2, size_t S>
        friend bool operator!=(const StackAllocator<T1, S>& a1, const StackAllocator<T2, S>& a2) noexcept;
    };


    template <typename T, typename U, size_t memoryBlockSize>
    bool operator==(const StackAllocator<T, memoryBlockSize>& a1, const StackAllocator<U, memoryBlockSize>& a2) noexcept
    {
        return (a1.m_CurrentMarker == a2.m_CurrentMarker &&
            &a1.m_MemoryBlock == &a2.m_MemoryBlock &&
            a1.m_EndBlockAddress == a2.m_EndBlockAddress &&
            a1.m_StartBlockAddress == a2.m_StartBlockAddress);
    }

    template <typename T, typename U, size_t memoryBlockSize>
    bool operator!=(const StackAllocator<T, memoryBlockSize>& a1, const StackAllocator<U, memoryBlockSize>& a2) noexcept
    {
        return !(a1 == a2);
    }

}