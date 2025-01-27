//
// Created by Andrew Fagan on 1/26/25.
//

#pragma once

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

    ObjectPool() : m_FreeCount(NumberOfElements)
    {
        for (size_t i = 0; i < m_Data.size(); i++)
        {
            m_FreeArray[i] = &m_Data[i];
        }
    }

    /**@brief Allocates an element from the pool and returns a pointer to it.
     * @param args The arguments to the constructor of the element that will be created in the pool. Leave
     *             empty to create a default constructed element
     */
    template<typename... Args>
    ElementType* Allocate(Args&&... args)
    {
        [[likely]] if (m_FreeCount != 0)
        {
            ElementType* freeElement = m_FreeArray[m_FreeCount - 1];
            m_FreeCount--;

            new (freeElement) ElementType(std::forward<Args>(args)...);
            return freeElement;
        }

        return nullptr;
    }

    /**@brief Frees an element to the pool.
     * @param elementPtr The pointer to the element being freed. */
    void Free(ElementType* elementPtr)
    {
        ASSERT(m_FreeCount != NumberOfElements, "Free has been called more times then Allocate!")
        ASSERT(elementPtr >= &m_Data[0] && elementPtr <= &m_Data[NumberOfElements - 1], "Passed marker does not fall within this allocators memory block.")
        elementPtr->~ElementType();
        m_FreeCount++;
        m_FreeArray[m_FreeCount - 1] = elementPtr;
    }

private:

    alignas(alignof(ElementType)) std::array<ElementType, NumberOfElements> m_Data;
    alignas(alignof(ElementType*)) std::array<ElementType*, NumberOfElements> m_FreeArray;
    size_t m_FreeCount;
};

}