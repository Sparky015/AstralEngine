/**
* @file Allocator.h
* @author Andrew Fagan
* @date 7/15/26
*/

#pragma once

#include "Core/FixedIntegerTypes.h"

namespace Astral {

    class Allocator
    {
    public:
        virtual ~Allocator() = default;

        /**
         * @brief Allocates a memory block of the given size with the given required alignment.
         * @param size Size of the requested allocated block
         * @param alignment The alignment requirement for the allocation
         * @return A pointer to the allocated block or nullptr if the allocation failed.
         */
        virtual void* Allocate(size_t size, uint16 alignment) = 0;

        /**
         * @brief Resets all memory that the allocator owns. Every previous allocation is deallocated.
         */
        void Reset();

        /**
         * @brief Gets the amount of memory currently allocated out by the allocator.
         * @return The number of bytes currently allocated.
         */
        [[nodiscard]] virtual size_t GetUsedBlockSize() const = 0;

        /**
         * @brief Gets the memory capacity of the allocator.
         * @return The max number of bytes the allocator can allocate.
         */
        [[nodiscard]] virtual size_t GetCapacity() const = 0;

        /**
         * @brief Gets the total owned memory size of an allocator (including overhead)
         * @return The total owned memory size of an allocator (including overhead)
         */
        [[nodiscard]] virtual size_t GetOwnedMemorySize() const = 0;


        /// Deleting copy constructor and operator because the copied data in use won't be able to be freed.
        /// The new allocator will copy the same address range (some of that will include addresses in use by the user).
        /// Because the new allocator's memory is freshly allocated, no one has the pointers to the new allocator's memory
        /// for the data in use, so they can't be freed.
        Allocator(const Allocator& other) = delete;
        Allocator& operator=(const Allocator& other) = delete;

    };

}