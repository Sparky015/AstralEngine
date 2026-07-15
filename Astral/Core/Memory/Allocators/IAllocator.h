/**
* @file IAllocator.h
* @author Andrew Fagan
* @date 7/15/26
*/

#pragma once

#include "Core/FixedIntegerTypes.h"

namespace Astral {

    enum class AllocatorType
    {
        LINEAR,
        STACK,
        RING,
        POOL,
        SLAB,
        TLSF
    };

    class IAllocator
    {
    public:
        IAllocator() = default;
        virtual ~IAllocator() = default;

        /**
         * @brief Resets all memory that the allocator owns. Every previous allocation is deallocated.
         */
        virtual void Reset() = 0;

        /**
         * @brief Gets the amount of memory currently allocated out by the allocator.
         * @return The number of bytes currently allocated.
         */
        [[nodiscard]] virtual size_t GetUsedBlockSize() const = 0;

        /**
         * @brief Gets the memory capacity of the allocator.
         * @return The max number of bytes the allocator can allocate
         * @note The max number of bytes might not be able to be used if more than one allocation is made due to overhead depending on allocator type
         */
        [[nodiscard]] virtual size_t GetCapacity() const = 0;

        /**
         * @brief Gets the total owned memory size of an allocator (including overhead)
         * @return The total owned memory size of an allocator (including overhead)
         */
        [[nodiscard]] virtual size_t GetOwnedMemorySize() const = 0;

        /**
         * @brief Gets the allocator's type
         * @return The allocator's type
         */
        [[nodiscard]] virtual AllocatorType GetAllocatorType() const = 0;


        /// Deleting copy constructor and operator because the copied data in use won't be able to be freed.
        /// The new allocator will copy the same address range (some of that will include addresses in use by the user).
        /// Because the new allocator's memory is freshly allocated, no one has the pointers to the new allocator's memory
        /// for the data in use, so they can't be freed.
        IAllocator(const IAllocator& other) = delete;
        IAllocator& operator=(const IAllocator& other) = delete;

    };

}