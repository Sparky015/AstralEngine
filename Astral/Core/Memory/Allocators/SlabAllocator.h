//
// Created by Andrew Fagan on 1/26/25.
//

#pragma once

#include "Core/Utilities/Asserts.h"
#include <cstring>
#include <memory>

namespace Astral {

    /**
     * @brief Allocates preconstructed elements from slabs. Constructs many objects at once and lends them out. Objects are not reconstructed and
     *        will maintain the state when released back to the pool.
     * @thread_safety This class is not thread safe. User must handle thread safety if needed.
     */
    template<typename ElementType, size_t NumberOfElements, typename ElementAllocator = std::allocator<ElementType>>
    class SlabAllocator
    {
    public:

        /**
         * @brief Constructs an initial slab filled with default constructed elements.
         */
        SlabAllocator();

        /**
         * @brief Constructs an initial slab filled with elements that are constructed with the given arguments.
         * @param args The arguments to the constructor that will be used to fill the elements of the pool with.
         */
        template<typename... Args>
        explicit SlabAllocator(Args&&... args);
        ~SlabAllocator();

        /**
         * @brief Allocates a preconstructed element from a slab and returns a pointer to it.
         * @return A pointer to a free object. Returns nullptr if the slab is fully used.
         * @warning Object maybe in a random state or something other than the default instance state.
         */
        ElementType* Allocate();

        /**
         * @brief Frees an element to a slab. Does not reset the state of the object.
         * @param elementPtr The pointer to the element being freed.
         */
        void Free(ElementType* elementPtr);

        /**
         * @brief Checks if all slabs are fully used. True when all slabs are in full use.
         */
        [[nodiscard]] constexpr bool IsFull() const;

        /**
         * @brief Gets the number of free elements in the pool.
         */
        [[nodiscard]] constexpr size_t GetFreeCount() const;

        /**
         * @brief Gets the number of used elements in the pool.
         */
        [[nodiscard]] constexpr size_t GetUsedCount() const;

        /**
         * @brief Gets the total amount of elements that can be stored in the pool.
         */
        [[nodiscard]] constexpr size_t GetCapacity() const;

        /**
         * @brief Checks if the slab allocator is in a valid state.
         */
        [[nodiscard]] bool IsValid() const noexcept;

        /**
         * @brief Resets the slabs with elements that are constructed with the given arguments.
         *        This also frees all elements in the pool.
         * @param args The arguments to the constructor that will be used to fill the elements of the pool with.
         * @warning This also releases any elements in use.
         */
        template<typename... Args>
        void ResetElements(Args&&... args);

        SlabAllocator(SlabAllocator&& other) noexcept;
        SlabAllocator& operator=(SlabAllocator&& other) noexcept;
        SlabAllocator(const SlabAllocator& other) = delete;
        SlabAllocator& operator=(const SlabAllocator&) = delete;

    private:

        /**
         * @brief Adds a freed pointer back to the free array.
         * @param elementPtr The freed pointer that is being added back.
         */
        inline void PushFreePointer(ElementType* elementPtr);

        /**
         * @brief Takes a free pointer from the free array and returns it
         * @return A free element pointer.
         * @warning This assumes that the number of free elements is not 0. Check before calling.
         */
        inline ElementType* PopFreePointer();

        /**
         * @brief Calls the destructors of all the elements in the pool.
         */
        inline void CallAllElementDestructors();


        using PointerAllocator = typename std::allocator_traits<ElementAllocator>::template rebind_alloc<ElementType*>;

        [[no_unique_address]] PointerAllocator m_PointerAllocator;
        [[no_unique_address]] ElementAllocator m_ElementAllocator;
        ElementType* m_Data;
        ElementType** m_FreeArray;
        size_t m_FreeCount;
    };

    #include "SlabAllocator_Impl.h"
}
