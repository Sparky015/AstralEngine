//
// Created by Andrew Fagan on 1/26/25.
//

#pragma once

#include <array>
#include <list>

#include "Debug/Macros/Asserts.h"

namespace Core {

    template<typename ElementType, size_t NumberOfElements>
    class ObjectPool
    {
    public:
        static_assert(sizeof(ElementType) >= 8, "ElementType of object pool must be at least the size of a pointer!");

        ObjectPool()
        {
            m_FreeListHead = &m_Data[0];
            for (size_t i = 0; i < NumberOfElements - 1; i++)
            {
                // The next free element's address is stored in the memory space of the previous free element
                *reinterpret_cast<ElementType**>(&m_Data[i]) = &m_Data[i + 1];
            }
            *reinterpret_cast<ElementType**>(&m_Data[NumberOfElements - 1]) = nullptr;
        }

        /**@brief Allocates an element from the pool and returns a pointer to it.
         * @param args The arguments to the constructor of the element that will be created in the pool. Leave
         *             empty to create a default constructed element
         */
        template<typename... Args>
        ElementType* Allocate(Args&&... args)
        {
            [[likely]] if (m_FreeListHead)
            {
                // m_HeaderPointer points to the first element address
                ElementType* firstElementAddress = m_FreeListHead;
                ElementType* secondElementAddress = *reinterpret_cast<ElementType**>(m_FreeListHead);
                m_FreeListHead = secondElementAddress; // Can be nullptr

                new (firstElementAddress) ElementType(std::forward<Args>(args)...);
                return firstElementAddress;
            }

            return nullptr;
        }

        /**@brief Frees an element to the pool.
         * @param elementPtr The pointer to the element being freed. */
        void Free(ElementType* elementPtr)
        {
            if (!elementPtr) return;

            ASSERT(elementPtr >= &m_Data[0] && elementPtr <= &m_Data[NumberOfElements - 1], "Pointer does not fall within this pool's memory block.")
            elementPtr->~ElementType();

            ElementType* nextPointer = m_FreeListHead; // Can be nullptr
            m_FreeListHead = elementPtr;
            *reinterpret_cast<ElementType**>(elementPtr) = nextPointer;
        }

    private:

        alignas(alignof(ElementType)) std::array<ElementType, NumberOfElements> m_Data;
        ElementType* m_FreeListHead; // Points to the first free element address
    };

}