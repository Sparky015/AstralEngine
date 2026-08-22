/**
* @file STLTLSFAllocator.h
* @author Andrew Fagan
* @date 8/17/2026
*/

#pragma once

#include "Core/Memory/Allocators/TLSFAllocator.h"

#include <cstddef>
#include <memory>
#include <new>


namespace Astral {

    /**
     * @brief STL compliant version of TLSFAllocator.
     * @thread_safety This class is NOT thread safe.
     */
    template <typename T>
    class STLTLSFAllocator
    {
    public:

        explicit STLTLSFAllocator(size_t memoryBlockSize) :
            m_TLSFAllocator(std::make_shared<Astral::TLSFAllocator>(memoryBlockSize))
        {}

        ~STLTLSFAllocator() = default;


        using value_type = T;
        using pointer = T*;
        using size_type = std::size_t;
        using difference_type = std::ptrdiff_t;
        using propagate_on_container_copy_assignment = std::true_type;
        using is_always_equal = std::false_type; // This needs to be false for stateful allocators!


        /**
         * @brief Allocates memory for n instances of the type of allocator. Hint is completely ignored.
         */
        pointer allocate(size_type numberOfElements, const void* hint = nullptr)
        {
            [[unlikely]] if (m_TLSFAllocator == nullptr) { return nullptr; }
            const size_t allocatedBytes = numberOfElements * sizeof(T);

            void* returnPointer = m_TLSFAllocator->Allocate(allocatedBytes);
            if (!returnPointer)
            {
                throw std::bad_alloc(); // Pointer was nullptr so throw bad_alloc for STL only
            };

            return (pointer)returnPointer;
        }


        /**@brief This does nothing. Use Reset method to Deallocate memory. */
        void deallocate(pointer ptr, size_type numberOfElements)
        {
            // Does nothing. Only resets memory on call to reset()
        }

        /**@brief Resets ALL memory that the allocator owns. Everything gets deallocated. */
        void reset()
        {
            [[unlikely]] if (m_TLSFAllocator == nullptr) { return; }
            m_TLSFAllocator->Reset();
        }

        /**@brief Gets the amount of memory currently allocated out by the allocator. */
        [[nodiscard]] size_t getUsedBlockSize() const
        {
            [[unlikely]] if (m_TLSFAllocator == nullptr) { return 0; }
            return m_TLSFAllocator->GetUsedBlockSize();
        }

        /**@brief Gets the memory capacity of the allocator. */
        [[nodiscard]] size_t getCapacity() const
        {
            [[unlikely]] if (m_TLSFAllocator == nullptr) { return 0; }
            return m_TLSFAllocator->GetCapacity();
        }

        // Rebind struct
        template <typename U>
        struct rebind
        {
            using other = STLTLSFAllocator<U>;
        };

        STLTLSFAllocator(const STLTLSFAllocator& other) :
            m_TLSFAllocator(other.m_TLSFAllocator)
        {}

        STLTLSFAllocator& operator=(const STLTLSFAllocator& other)
        {
            if (this != &other)
            {
                m_TLSFAllocator = other.m_TLSFAllocator;
            }
            return *this;
        }

        template <typename U>
        STLTLSFAllocator(const STLTLSFAllocator<U>& other) :
                m_TLSFAllocator(other.m_TLSFAllocator)
        {}

        template <typename U>
        STLTLSFAllocator& operator=(const STLTLSFAllocator<U>& other)
        {
            if (this != &other)
            {
                m_TLSFAllocator = other.m_TLSFAllocator;
            }
            return *this;
        }

        template <typename U>
        bool operator==(const STLTLSFAllocator<U>& other) noexcept
        {
            return (m_TLSFAllocator == other.m_TLSFAllocator);
        }

        template <typename U>
        bool operator!=(const STLTLSFAllocator<U>& other) noexcept
        {
            return !(*this == other);
        }

    private:
        template <typename U>
        friend class STLTLSFAllocator;

        std::shared_ptr<Astral::TLSFAllocator> m_TLSFAllocator;
    };

}
