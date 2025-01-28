//
// Created by Andrew Fagan on 1/26/25.
//

#pragma once

#include "Debug/Macros/Asserts.h"
#include <memory>

namespace Core {

    /**@brief A generic object pool. Constructs many objects at once and lends them out. Objects are not reconstructed and
     *        will maintain the state when released back to the pool.
     * @thread_safety This class is not thread safe. User must handle thread safety if needed. */
    template<typename ElementType, size_t NumberOfElements, typename ElementAllocator = std::allocator<ElementType>>
    class ObjectPool
    {
    public:

        /**@brief Constructs an object pool filled with default constructed elements. */
        ObjectPool() :
            m_Data(m_ElementAllocator.allocate(NumberOfElements)),
            m_FreeArray(m_PointerAllocator.allocate(NumberOfElements)),
            m_FreeCount(NumberOfElements)
        {
            for (size_t i = 0; i < NumberOfElements; i++)
            {
                new (&m_Data[i]) ElementType();
                m_FreeArray[i] = &m_Data[i];
            }
        }


        /**@brief Constructs an object pool filled with elements that are constructed with the given arguments.
         * @param args The arguments to the constructor that will be used to fill the elements of the pool with. */
        template<typename... Args>
        explicit ObjectPool(Args&&... args) :
            m_Data(m_ElementAllocator.allocate(NumberOfElements)),
            m_FreeArray(m_PointerAllocator.allocate(NumberOfElements)),
            m_FreeCount(NumberOfElements)
        {
            static_assert(std::is_default_constructible_v<ElementType> ||
              std::is_constructible_v<ElementType, Args...>,
              "ElementType must be constructible with the given arguments");

            for (size_t i = 0; i < NumberOfElements; i++)
            {
                new (&m_Data[i]) ElementType(std::forward<Args>(args)...);
                m_FreeArray[i] = &m_Data[i];
            }
        }

        ~ObjectPool()
        {
            CallAllObjectDestructors();
            m_ElementAllocator.deallocate((ElementType*)m_Data, NumberOfElements);
            m_PointerAllocator.deallocate(m_FreeArray, NumberOfElements);
        }

        /**@brief Allocates an element from the pool and returns a pointer to it.
         * @return A pointer to a free object. Returns nullptr if the pool is full.
         * @warning Object maybe in a random state or something other than the default instance state. */
        ElementType* Acquire()
        {
            [[likely]] if (m_FreeCount != 0)
            {
                ElementType* freeElement = PopFreePointer();
                return freeElement;
            }
            return nullptr;
        }

        /**@brief Frees an element to the pool. Does not reset the state of the object.
         * @param elementPtr The pointer to the element being freed. */
        void Release(ElementType* elementPtr)
        {
            ASSERT(elementPtr >= &m_Data[0] && elementPtr <= &m_Data[NumberOfElements - 1], "Pointer does not fall within this allocators memory block.")
            PushFreePointer(elementPtr);
        }

        /**@brief Checks if all objects are in use. True when all objects are in use. */
        [[nodiscard]] constexpr bool IsFull() const { return m_FreeCount == 0; }

        /**@brief Gets the number of free objects in the pool. */
        [[nodiscard]] constexpr size_t GetFreeCount() const { return m_FreeCount; }

        /**@brief Gets the number of used objects in the pool. */
        [[nodiscard]] constexpr size_t GetUsedCount() const { return NumberOfElements - m_FreeCount; }

        /**@brief Gets the total amount of objects that can be stored in the pool. */
        [[nodiscard]] constexpr size_t GetCapacity() const { return NumberOfElements; }

        /**@brief Checks if the object pool is in a valid state. */
        [[nodiscard]] bool IsValid() const noexcept { return m_Data != nullptr && m_FreeArray != nullptr; }


        /**@brief Resets the object pool to elements that are constructed with the given arguments.
         *        This also frees all objects in the pool.
         * @param args The arguments to the constructor that will be used to fill the elements of the pool with.
         * @warning This also releases any objects in use. */
        template<typename... Args>
        void ResetObjects(Args&&... args)
        {
            CallAllObjectDestructors();
            for (size_t i = 0; i < NumberOfElements; i++)
            {
                new (&m_Data[i]) ElementType(std::forward<Args>(args)...);
                m_FreeArray[i] = &m_Data[i];
            }
            m_FreeCount = NumberOfElements;
        }

        ObjectPool(ObjectPool&& other) noexcept :
            m_Data(other.m_Data),
            m_FreeArray(other.m_FreeArray),
            m_FreeCount(other.m_FreeCount),
            m_ElementAllocator(std::move(other.m_ElementAllocator)),
            m_PointerAllocator(std::move(other.m_PointerAllocator))
        {
            other.m_Data = nullptr;
            other.m_FreeArray = nullptr;
            other.m_FreeCount = 0;
        }

        ObjectPool& operator=(ObjectPool&& other) noexcept
        {
            if (this != &other)
            {
                CallAllObjectDestructors();
                m_ElementAllocator.deallocate((ElementType*)m_Data, NumberOfElements);
                m_PointerAllocator.deallocate(m_FreeArray, NumberOfElements);

                m_Data = other.m_Data;
                m_FreeArray = other.m_FreeArray;
                m_FreeCount = other.m_FreeCount;
                m_ElementAllocator = std::move(other.m_ElementAllocator);
                m_PointerAllocator = std::move(other.m_PointerAllocator);
                other.m_Data = nullptr;
                other.m_FreeArray = nullptr;
                other.m_FreeCount = 0;
            }
            return *this;
        }

        ObjectPool(const ObjectPool& other) :
            m_Data(m_ElementAllocator.allocate(NumberOfElements)),
            m_FreeArray(m_PointerAllocator.allocate(NumberOfElements)),
            m_FreeCount(other.m_FreeCount),
            m_ElementAllocator(other.m_ElementAllocator),
            m_PointerAllocator(other.m_PointerAllocator)
        {
            if constexpr (std::is_trivially_copyable_v<ElementType>)
            {
                std::memcpy(m_Data, other.m_Data, sizeof(ElementType) * NumberOfElements);
            }
            else
            {
                for (size_t i = 0; i < NumberOfElements; i++)
                {
                    m_Data[i] = other.m_Data[i];
                }
            }
            std::memcpy(m_FreeArray, other.m_FreeArray, sizeof(ElementType) * NumberOfElements);
        }

        /**@brief Deleted copy assignment operator to prevent accidental copies being made of the object pool. */
        ObjectPool& operator=(const ObjectPool&) = delete;

    private:

        /**@brief Adds a freed pointer back to the free array.
         * @param elementPtr The freed pointer that is being added back. */
        inline void PushFreePointer(ElementType* elementPtr)
        {
            ASSERT(m_FreeCount != NumberOfElements, "Free has been called more times then Allocate!")
            m_FreeCount++;
            m_FreeArray[m_FreeCount - 1] = elementPtr;
        }

        /**@brief Takes a free pointer from the free array and returns it
         * @return A free element pointer.
         * @warning This assumes that the number of free elements is not 0. Check before calling. */
        inline ElementType* PopFreePointer()
        {
            ElementType* freeElement = m_FreeArray[m_FreeCount - 1];
            m_FreeCount--;
            return freeElement;
        }

        /**@brief Calls the destructors of all the objects in the pool. */
        inline void CallAllObjectDestructors()
        {
            if constexpr (!std::is_trivially_destructible_v<ElementType>)
            {
                for (size_t i = 0; i < NumberOfElements; i++)
                {
                    m_Data[i].~ElementType();
                }
            }
        }

        using PointerAllocator = typename std::allocator_traits<ElementAllocator>::template rebind_alloc<ElementType*>;

        [[no_unique_address]] PointerAllocator m_PointerAllocator;
        [[no_unique_address]] ElementAllocator m_ElementAllocator;
        ElementType* m_Data;
        ElementType** m_FreeArray;
        size_t m_FreeCount;
    };

}