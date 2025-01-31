/**
* @file STLStackBasedLinearAllocator.h
* @author Andrew Fagan
* @date 1/8/2025
*/

#pragma once

#include "Core/Memory/Allocators/AllocatorUtils.h"
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

        STLLinearAllocator(size_t memoryBlockSize) :
            m_StartBlockAddress((unsigned char*)AllocatorUtils::AllocMaxAlignedBlock(memoryBlockSize)),
            m_EndBlockAddress(m_StartBlockAddress + memoryBlockSize),
            m_CurrentMarker(m_StartBlockAddress)
        {}


        ~STLLinearAllocator()
        {
            AllocatorUtils::FreeMaxAlignedBlock(m_StartBlockAddress);
        }

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
        [[nodiscard]] size_t getUsedBlockSize() const
        {
            return m_CurrentMarker - m_StartBlockAddress;
        }

        /**@brief Gets the memory capacity of the allocator. */
        [[nodiscard]] size_t getCapacity() const
        {
            return m_EndBlockAddress - m_StartBlockAddress;
        }

        // Rebind struct
        template <typename U>
        struct rebind
        {
            using other = STLLinearAllocator<U>;
        };

        STLLinearAllocator(const STLLinearAllocator& other) :
                m_StartBlockAddress((unsigned char*)AllocatorUtils::AllocMaxAlignedBlock(other.getCapacity())),
                m_EndBlockAddress(m_StartBlockAddress + other.getCapacity()),
                m_CurrentMarker(m_StartBlockAddress + other.getUsedBlockSize())
        {
            std::memcpy(m_StartBlockAddress, other.m_StartBlockAddress, other.getCapacity());
        }

        STLLinearAllocator& operator=(const STLLinearAllocator& other)
        {
            if (this != &other)
            {
                m_StartBlockAddress = (unsigned char*)AllocatorUtils::AllocMaxAlignedBlock(other.getCapacity());
                std::memcpy(m_StartBlockAddress, other.m_StartBlockAddress, other.getCapacity());
                m_EndBlockAddress = m_StartBlockAddress + other.getCapacity();
                m_CurrentMarker = m_StartBlockAddress + other.getUsedBlockSize();
            }
            return *this;
        }

        STLLinearAllocator(STLLinearAllocator&& other) noexcept :
        m_StartBlockAddress(other.m_StartBlockAddress),
        m_EndBlockAddress(other.m_EndBlockAddress),
        m_CurrentMarker(other.m_CurrentMarker)
        {
            other.m_StartBlockAddress = nullptr;
            other.m_EndBlockAddress = nullptr;
            other.m_CurrentMarker = nullptr;
        }

        STLLinearAllocator& operator=(STLLinearAllocator&& other) noexcept
        {
            if (this != &other)
            {
                m_StartBlockAddress = other.m_StartBlockAddress;
                m_EndBlockAddress = other.m_EndBlockAddress;
                m_CurrentMarker = other.m_CurrentMarker;
                other.m_StartBlockAddress = nullptr;
                other.m_EndBlockAddress = nullptr;
                other.m_CurrentMarker = nullptr;
            }
            return *this;
        }

        template <typename U>
        bool operator==(const STLLinearAllocator<U>& other) noexcept
        {
            return (m_CurrentMarker == other.m_CurrentMarker &&
                    m_EndBlockAddress == other.m_EndBlockAddress &&
                    m_StartBlockAddress == other.m_StartBlockAddress);
        }

        template <typename U>
        bool operator!=(const STLLinearAllocator<U>& other) noexcept
        {
            return !(*this == other);
        }


    private:

        unsigned char* m_StartBlockAddress;
        unsigned char* m_EndBlockAddress;
        unsigned char* m_CurrentMarker;
    };

}