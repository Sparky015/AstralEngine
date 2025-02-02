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
#include "Core/Memory/Allocators/StackBasedLinearAllocator.h"


namespace Core {

    /**@brief STL compliant version of StackBasedLinearAllocator. It is a stack-based linear allocator. Max allocation size
     *        is 5.28 KB due to being on the stack. Deallocate method does nothing. Reset method deallocates
     *        the whole memory block.
     * @warning You have to use the Reset method to Deallocate memory. It deallocates all memory being used.
     *          It's all or nothing.
     * @thread_safety This class is NOT thread safe. */
    template <typename ElementType, size_t MemoryBlockSize>
    class STLStackBasedLinearAllocator
    {
    public:
        static constexpr size_t MAX_STACK_ALLOCATION_SIZE = 5280; // 5.28 KB
        static_assert(MemoryBlockSize <= MAX_STACK_ALLOCATION_SIZE, "Memory block size for stack is too big!");

        using value_type = ElementType;
        using pointer = ElementType*;
        using size_type = std::size_t;
        using difference_type = std::ptrdiff_t;
        using propagate_on_container_move_assignment = std::true_type;
        using propagate_on_container_copy_assignment = std::true_type;
        using is_always_equal = std::false_type; // This needs to be false for stateful allocators!


        STLStackBasedLinearAllocator() :
            m_StackBasedLinearAllocator(std::make_shared<Core::StackBasedLinearAllocator<MemoryBlockSize>>())
        {}

        explicit STLStackBasedLinearAllocator(size_type) noexcept : STLStackBasedLinearAllocator() {}
        ~STLStackBasedLinearAllocator()
        {
            reset();
        }


        /**@brief Allocates memory for n instances of the type of allocator. Hint is completely ignored. */
        pointer allocate(size_type numberOfElements, const void* hint = nullptr)
        {
            const size_t allocatedBytes = numberOfElements * sizeof(ElementType);
            return (pointer)m_StackBasedLinearAllocator->Allocate(allocatedBytes, alignof(ElementType));
        }


        /**@brief This does nothing. Use Reset method to Deallocate memory. */
        void deallocate(pointer ptr, size_type numberOfElements)
        {
            // Does nothing. Only resets memory on call to reset()
        }

        /**@brief Resets ALL memory that the allocator owns. Everything gets deallocated. */
        void reset()
        {
            m_StackBasedLinearAllocator->Reset();
        }

        /**@brief Gets the amount of memory currently allocated out by the allocator. */
        size_t getUsedBlockSize()
        {
            return m_StackBasedLinearAllocator->GetUsedBlockSize();
        }

        /**@brief Gets the memory capacity of the allocator. */
        [[nodiscard]] size_t getCapacity() const
        {
            return m_StackBasedLinearAllocator->GetCapacity();
        }

        // Rebind struct
        template <typename OtherElementType>
        struct rebind
        {
            using other = STLStackBasedLinearAllocator<OtherElementType, MemoryBlockSize>;
        };

        STLStackBasedLinearAllocator(const STLStackBasedLinearAllocator& other) :
            m_StackBasedLinearAllocator(other.m_StackBasedLinearAllocator)
        {}

        STLStackBasedLinearAllocator& operator=(const STLStackBasedLinearAllocator& other)
        {
            if (this != &other)
            {
                m_StackBasedLinearAllocator = other.m_StackBasedLinearAllocator;
            }
            return *this;
        }

        template <typename OtherElementType>
        STLStackBasedLinearAllocator(const STLStackBasedLinearAllocator<OtherElementType, MemoryBlockSize>& other) :
                m_StackBasedLinearAllocator(other.m_StackBasedLinearAllocator)
        {}

        template <typename OtherElementType>
        STLStackBasedLinearAllocator& operator=(const STLStackBasedLinearAllocator<OtherElementType, MemoryBlockSize>& other)
        {
            if (this != &other)
            {
                m_StackBasedLinearAllocator = other.m_StackBasedLinearAllocator;
            }
            return *this;
        }

        template <typename OtherElementType>
        bool operator==(const STLStackBasedLinearAllocator<OtherElementType, MemoryBlockSize>& other) noexcept
        {
            return (m_StackBasedLinearAllocator == other.m_StackBasedLinearAllocator);
        }

        template <typename OtherElementType>
        bool operator!=(const STLStackBasedLinearAllocator<OtherElementType, MemoryBlockSize>& other) noexcept
        {
            return !(*this == other);
        }

    private:
        template <typename OtherElementType, size_t OtherMemoryBlockSize>
        friend class STLStackBasedLinearAllocator;

        std::shared_ptr<Core::StackBasedLinearAllocator<MemoryBlockSize>> m_StackBasedLinearAllocator;

    };

}