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
        m_HeaderPointer = &m_Data[0];
        for (size_t i = 0; i < NumberOfElements - 1; i++)
        {
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
        [[likely]] if (m_HeaderPointer)
        {
            // m_HeaderPointer points to the first element address
            ElementType* secondElementAddress = *reinterpret_cast<ElementType**>(m_HeaderPointer);
            [[likely]] if (secondElementAddress)
            {
                ElementType* firstElementAddress = m_HeaderPointer;
                m_HeaderPointer = secondElementAddress;
                new (firstElementAddress) ElementType(std::forward<Args>(args)...);
                return firstElementAddress;
            }
            else
            {
                // The next pointer is nullptr, so m_HeaderPointer is the last free element pointer
                ElementType* lastFreeElementAddress = m_HeaderPointer;
                m_HeaderPointer = nullptr;
                new (lastFreeElementAddress) ElementType(std::forward<Args>(args)...);
                return lastFreeElementAddress;
            }

        }

        return nullptr;
    }

    /**@brief Frees an element to the pool.
     * @param elementPtr The pointer to the element being freed. */
    void Free(ElementType* elementPtr)
    {
        // ASSERT(m_FreeCount != NumberOfElements, "Free has been called more times than Allocate!")
        ASSERT(elementPtr >= &m_Data[0] && elementPtr <= &m_Data[NumberOfElements - 1], "Pointer does not fall within this pool's memory block.")
        elementPtr->~ElementType();

        if (!m_HeaderPointer)
        {
            m_HeaderPointer = elementPtr;
            *reinterpret_cast<ElementType**>(elementPtr) = nullptr;
            return;
        }

        ElementType* nextPointer = m_HeaderPointer;
        m_HeaderPointer = elementPtr;
        *reinterpret_cast<ElementType**>(elementPtr) = nextPointer;
    }

private:

    alignas(alignof(ElementType)) std::array<ElementType, NumberOfElements> m_Data;
    ElementType* m_HeaderPointer; // Points to the first free element address
};

}