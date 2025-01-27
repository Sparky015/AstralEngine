//
// Created by Andrew Fagan on 1/26/25.
//

#pragma once

#include <array>
#include <list>

#include "Debug/Macros/Asserts.h"

namespace Core {

    /**@brief A generic object pool. Constructs many objects at once and lends them out. Objects are not reconstructed and
     *        will maintain the state when released back to the pool.
     * @thread_safety This class is not thread safe. User must handle thread safety if needed. */
    template<typename ElementType, size_t NumberOfElements, class Allocator = std::allocator<ElementType>>
    class ObjectPool
    {
    public:

        /**@brief Constructs an object pool filled with default constructed objects. */
        ObjectPool() : m_FreeCount(NumberOfElements)
        {
            for (size_t i = 0; i < NumberOfElements; i++)
            {
                new (&CastedData()[i]) ElementType();
                m_FreeArray[i] = &CastedData()[i];
            }
        }

        /**@brief Constucts an object pool filled with elements that are copies of the given element.
         * @param templateElement The element that will be copied to the rest of the object pool. */
        explicit ObjectPool(const ElementType& templateElement) : m_FreeCount(NumberOfElements)
        {
            for (size_t i = 0; i < NumberOfElements; i++)
            {
                new (&CastedData()[i]) ElementType(templateElement);
                m_FreeArray[i] = &CastedData()[i];
            }
        }

        /**@brief Constructs an object pool filled with elements that are constructed with the given arguments.
         * @param args The arguments to the constructor that will be used to fill the elements of the pool with. */
        template<typename... Args>
        explicit ObjectPool(Args&&... args) : m_FreeCount(NumberOfElements)
        {
            for (size_t i = 0; i < NumberOfElements; i++)
            {
                new (&CastedData()[i]) ElementType(std::forward<Args>(args)...);
                m_FreeArray[i] = &CastedData()[i];
            }
        }

        ~ObjectPool()
        {
            for (size_t i = 0; i < NumberOfElements; i++)
            {
                if constexpr (!std::is_trivially_destructible_v<ElementType>)
                {
                    // Just in case any of the instances were allocated through placement new (ResetObjects method)
                    CastedData()[i].~ElementType();
                }
            }
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
            ASSERT(elementPtr >= &CastedData()[0] && elementPtr <= &CastedData()[NumberOfElements - 1], "Pointer does not fall within this allocators memory block.")
            PushFreePointer(elementPtr);
        }

        [[nodiscard]] constexpr bool IsFull() const { return m_FreeCount == 0; }
        [[nodiscard]] constexpr size_t GetFreeCount() const { return m_FreeCount; }
        [[nodiscard]] constexpr size_t GetUsedCount() const { return NumberOfElements - m_FreeCount; }
        [[nodiscard]] constexpr size_t GetCapacity() const { return NumberOfElements; }


        /**@brief Resets the object pool to default constructed objects.
         * @warning This also releases any objects in use. */
        void ResetObjects()
        {
            CallAllObjectDestructors();
            for (size_t i = 0; i < NumberOfElements; i++)
            {
                new (&CastedData()[i]) ElementType();
                m_FreeArray[i] = &CastedData()[i];
            }
        }

        /**@brief Resets the object pool to elements that are copies of the given element.
         *        This also frees all objects in the pool.
         * @param templateElement The element that will be copied to the rest of the object pool.
         * @warning This also releases any objects in use. */
        void ResetObjects(const ElementType& templateElement)
        {
            CallAllObjectDestructors();
            for (size_t i = 0; i < NumberOfElements; i++)
            {
                new (&CastedData()[i]) ElementType(templateElement);
                m_FreeArray[i] = &CastedData()[i];
            }
        }

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
                new (&CastedData()[i]) ElementType(std::forward<Args>(args)...);
                m_FreeArray[i] = &CastedData()[i];
            }
        }

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

        inline void StoreAllElementAddressesInFreeArray()
        {
            for (size_t i = 0; i < NumberOfElements; i++)
            {
                m_FreeArray[i] = &CastedData()[i];
            }
        }

        inline void CallAllObjectDestructors()
        {
            for (size_t i = 0; i < NumberOfElements; i++)
            {
                CastedData()[i].~ElementType();
            }
        }

        constexpr ElementType* CastedData()
        {
            return reinterpret_cast<ElementType*>(&m_Data);
        }

        alignas(alignof(ElementType)) std::array<std::byte, sizeof(ElementType) * NumberOfElements> m_Data;
        alignas(alignof(ElementType*)) std::array<ElementType*, NumberOfElements> m_FreeArray;
        size_t m_FreeCount;

    };

}