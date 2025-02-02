/**
* @file STLStackBasedLinearAllocator.h
* @author Andrew Fagan
* @date 1/8/2025
*/

#pragma once

#include "Core/Memory/Allocators/AllocatorUtils.h"
#include "Core/Memory/Allocators/LinearAllocator.h"
#include "Core/Memory/Tracking/AllocationTracker.h"
#include <cstddef>
#include <cstring>
#include <memory>
#include <new>


namespace Core {

    /**@brief STL compliant version of LinearAllocator. It is a heap-based linear allocator.
     *        Deallocate method does nothing. Reset method deallocates the whole memory block.
     * @warning You have to use the Reset method to Deallocate memory. It deallocates all memory being used.
     *          It's all or nothing.
     * @thread_safety This class is NOT thread safe. */
    template <typename T>
    class STLLinearAllocator
    {
    public:

        explicit STLLinearAllocator(size_t memoryBlockSize) :
            m_LinearAllocator(std::make_shared<Core::LinearAllocator>(memoryBlockSize))
        {}

        ~STLLinearAllocator() = default;


        using value_type = T;
        using pointer = T*;
        using size_type = std::size_t;
        using difference_type = std::ptrdiff_t;
        using propagate_on_container_copy_assignment = std::true_type;
        using is_always_equal = std::false_type; // This needs to be false for stateful allocators!


        /**@brief Allocates memory for n instances of the type of allocator. Hint is completely ignored. */
        pointer allocate(size_type numberOfElements, const void* hint = nullptr)
        {
            [[unlikely]] if (m_LinearAllocator == nullptr) { return nullptr; }
            const size_t allocatedBytes = numberOfElements * sizeof(T);
            return (pointer)m_LinearAllocator->Allocate(allocatedBytes, alignof(T));
        }


        /**@brief This does nothing. Use Reset method to Deallocate memory. */
        void deallocate(pointer ptr, size_type numberOfElements)
        {
            // Does nothing. Only resets memory on call to reset()
        }

        /**@brief Resets ALL memory that the allocator owns. Everything gets deallocated. */
        void reset()
        {
            [[unlikely]] if (m_LinearAllocator == nullptr) { return; }
            m_LinearAllocator->Reset();
        }

        /**@brief Gets the amount of memory currently allocated out by the allocator. */
        [[nodiscard]] size_t getUsedBlockSize() const
        {
            [[unlikely]] if (m_LinearAllocator == nullptr) { return 0; }
            return m_LinearAllocator->GetUsedBlockSize();
        }

        /**@brief Gets the memory capacity of the allocator. */
        [[nodiscard]] size_t getCapacity() const
        {
            [[unlikely]] if (m_LinearAllocator == nullptr) { return 0; }
            return m_LinearAllocator->GetCapacity();
        }

        // Rebind struct
        template <typename U>
        struct rebind
        {
            using other = STLLinearAllocator<U>;
        };

        STLLinearAllocator(const STLLinearAllocator& other) :
            m_LinearAllocator(other.m_LinearAllocator)
        {}

        STLLinearAllocator& operator=(const STLLinearAllocator& other)
        {
            if (this != &other)
            {
                m_LinearAllocator = other.m_LinearAllocator;
            }
            return *this;
        }

        template <typename U>
        STLLinearAllocator(const STLLinearAllocator<U>& other) :
                m_LinearAllocator(other.m_LinearAllocator)
        {}

        template <typename U>
        STLLinearAllocator& operator=(const STLLinearAllocator<U>& other)
        {
            if (this != &other)
            {
                m_LinearAllocator = other.m_LinearAllocator;
            }
            return *this;
        }

        template <typename U>
        bool operator==(const STLLinearAllocator<U>& other) noexcept
        {
            return (m_LinearAllocator == other.m_LinearAllocator);
        }

        template <typename U>
        bool operator!=(const STLLinearAllocator<U>& other) noexcept
        {
            return !(*this == other);
        }

    private:
        template <typename U>
        friend class STLLinearAllocator;

        std::shared_ptr<Core::LinearAllocator> m_LinearAllocator;
    };

}