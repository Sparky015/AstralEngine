/**
* @file LinearAllocator.h
* @author Andrew Fagan
* @date 1/29/25
*/

#pragma once

#include "AllocatorUtils.h"
#include "Core/CoreMacroDefinitions.h"
#include "Core/Memory/Tracking/AllocationTracker.h"
#include "Debug/Macros/Asserts.h"
#include <cstddef>
#include <memory>
#include <new>


namespace Core {

    /**@brief A stack-based linear allocator. Max allocation size is 5.28 KB due to being on the stack.
     *        Deallocate method does nothing. Reset method deallocates the whole memory block.
     * @warning You have to use the Reset method to Deallocate memory. It deallocates all memory being used.
     *          It's all or nothing.
     * @thread_safety This class is NOT thread safe. */

    class LinearAllocator {
    public:

        explicit LinearAllocator(size_t memoryBlockSize) :
                m_StartBlockAddress((unsigned char*)AllocatorUtils::AllocMaxAlignedBlock(memoryBlockSize)),
                m_EndBlockAddress(m_StartBlockAddress + memoryBlockSize),
                m_CurrentMarker(m_StartBlockAddress)
        {}

        ~LinearAllocator()
        {
            AllocatorUtils::FreeMaxAlignedBlock(m_StartBlockAddress);
        }

        /**@brief Allocates a memory block of the given size with the given required alignment.
         * @param size Size of the requested allocated block
         * @param alignment The alignment requirement for the allocation
         * @return A pointer to the allocated block
         * @throw std::bad_alloc When there is not enough memory to complete an allocation */
        void* Allocate(size_t size, uint16 alignment)
        {
            ASSERT(AllocatorUtils::IsAlignmentPowerOfTwo(alignment), "Given alignment is not a power of two!")
            if (AllocatorUtils::DoesCauseOverflow(m_CurrentMarker, size, m_EndBlockAddress)) { throw std::bad_alloc(); }

            std::size_t space = m_EndBlockAddress - m_CurrentMarker;
            void* alignedAddress = m_CurrentMarker;

            // Aligns the address and will return nullptr if there is not enough space
            if (!std::align(alignment, size, alignedAddress, space)) { throw std::bad_alloc(); }

            // Update current marker
            m_CurrentMarker = static_cast<unsigned char*>(alignedAddress) + size;

            TRACK_ALLOCATION(size);
            return alignedAddress;
        }

        /**@brief Resets ALL memory that the allocator owns. Everything gets deallocated. */
        void Reset()
        {
            TRACK_DEALLOCATION(m_CurrentMarker - m_StartBlockAddress);
            m_CurrentMarker = m_StartBlockAddress;
        }

        /**@brief Gets the amount of memory currently allocated out by the allocator. */
        size_t GetUsedBlockSize()
        {
            return m_CurrentMarker - m_StartBlockAddress;
        }

        LinearAllocator(const LinearAllocator& other)
        {

        }

        LinearAllocator& operator=(const LinearAllocator& other)
        {
            if (this != &other)
            {

            }
            return *this;
        }

        LinearAllocator(LinearAllocator&& other) noexcept
        {

        }

        LinearAllocator& operator=(LinearAllocator&& other) noexcept
        {
            if (this != &other)
            {

            }
            return *this;
        }


        bool operator==(const LinearAllocator& other) noexcept
        {
            return (m_CurrentMarker == other.m_CurrentMarker &&
                    m_EndBlockAddress == other.m_EndBlockAddress &&
                    m_StartBlockAddress == other.m_StartBlockAddress);
        }

        bool operator!=(const LinearAllocator& other) noexcept
        {
            return !(*this == other);
        }

    private:

        unsigned char* m_StartBlockAddress;
        unsigned char* m_EndBlockAddress;
        unsigned char* m_CurrentMarker;
    };

}
