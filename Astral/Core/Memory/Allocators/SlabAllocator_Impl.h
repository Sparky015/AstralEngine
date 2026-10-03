/**
* @file SlabAllocator_Impl.h
* @author Andrew Fagan
* @date 8/11/26
*/

#pragma once

template<typename ElementType, size_t NumberOfElements, typename ElementAllocator>
    SlabAllocator<ElementType, NumberOfElements, ElementAllocator>::SlabAllocator() :
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


template<typename ElementType, size_t NumberOfElements, typename ElementAllocator>
template<typename ... Args>
SlabAllocator<ElementType, NumberOfElements, ElementAllocator>::SlabAllocator(Args&&... args) :
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


template<typename ElementType, size_t NumberOfElements, typename ElementAllocator>
SlabAllocator<ElementType, NumberOfElements, ElementAllocator>::~SlabAllocator()
{
    CallAllElementDestructors();
    m_ElementAllocator.deallocate((ElementType*)m_Data, NumberOfElements);
    m_PointerAllocator.deallocate(m_FreeArray, NumberOfElements);
}


template<typename ElementType, size_t NumberOfElements, typename ElementAllocator>
ElementType* SlabAllocator<ElementType, NumberOfElements, ElementAllocator>::Allocate()
{
    [[likely]] if (m_FreeCount != 0)
    {
        ElementType* freeElement = PopFreePointer();
        return freeElement;
    }
    return nullptr;
}


template<typename ElementType, size_t NumberOfElements, typename ElementAllocator>
void SlabAllocator<ElementType, NumberOfElements, ElementAllocator>::Free(ElementType* elementPtr)
{
    [[unlikely]] if (!elementPtr) { return; }
    ASSERT(elementPtr >= &m_Data[0] && elementPtr <= &m_Data[NumberOfElements - 1], "Pointer does not fall within this allocators memory block.")
    PushFreePointer(elementPtr);
}


template<typename ElementType, size_t NumberOfElements, typename ElementAllocator>
bool SlabAllocator<ElementType, NumberOfElements, ElementAllocator>::IsValid() const noexcept
{
    return m_Data != nullptr && m_FreeArray != nullptr;
}


template<typename ElementType, size_t NumberOfElements, typename ElementAllocator>
template<typename ... Args>
void SlabAllocator<ElementType, NumberOfElements, ElementAllocator>::ResetElements(Args&&... args)
{
    CallAllElementDestructors();
    for (size_t i = 0; i < NumberOfElements; i++)
    {
        new (&m_Data[i]) ElementType(std::forward<Args>(args)...);
        m_FreeArray[i] = &m_Data[i];
    }
    m_FreeCount = NumberOfElements;
}


template<typename ElementType, size_t NumberOfElements, typename ElementAllocator>
SlabAllocator<ElementType, NumberOfElements, ElementAllocator>::SlabAllocator(SlabAllocator&& other) noexcept :
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


template<typename ElementType, size_t NumberOfElements, typename ElementAllocator>
SlabAllocator<ElementType, NumberOfElements, ElementAllocator>& SlabAllocator<ElementType, NumberOfElements,
ElementAllocator>::operator=(SlabAllocator&& other) noexcept
{
    if (this != &other)
    {
        CallAllElementDestructors();
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


template<typename ElementType, size_t NumberOfElements, typename ElementAllocator>
void SlabAllocator<ElementType, NumberOfElements, ElementAllocator>::PushFreePointer(ElementType* elementPtr)
{
    ASSERT(m_FreeCount != NumberOfElements, "Free has been called more times then Allocate!")
    m_FreeCount++;
    m_FreeArray[m_FreeCount - 1] = elementPtr;
}


template<typename ElementType, size_t NumberOfElements, typename ElementAllocator>
ElementType* SlabAllocator<ElementType, NumberOfElements, ElementAllocator>::PopFreePointer()
{
    ElementType* freeElement = m_FreeArray[m_FreeCount - 1];
    m_FreeCount--;
    return freeElement;
}


template<typename ElementType, size_t NumberOfElements, typename ElementAllocator>
void SlabAllocator<ElementType, NumberOfElements, ElementAllocator>::CallAllElementDestructors()
{
    if constexpr (!std::is_trivially_destructible_v<ElementType>)
    {
        for (size_t i = 0; i < NumberOfElements; i++)
        {
            m_Data[i].~ElementType();
        }
    }
}


template<typename ElementType, size_t NumberOfElements, typename ElementAllocator>
    constexpr bool SlabAllocator<ElementType, NumberOfElements, ElementAllocator>::IsFull() const
{
    return m_FreeCount == 0;
}


template<typename ElementType, size_t NumberOfElements, typename ElementAllocator>
constexpr size_t SlabAllocator<ElementType, NumberOfElements, ElementAllocator>::GetFreeCount() const
{
    return m_FreeCount;
}


template<typename ElementType, size_t NumberOfElements, typename ElementAllocator>
constexpr size_t SlabAllocator<ElementType, NumberOfElements, ElementAllocator>::GetUsedCount() const
{
    return NumberOfElements - m_FreeCount;
}


template<typename ElementType, size_t NumberOfElements, typename ElementAllocator>
constexpr size_t SlabAllocator<ElementType, NumberOfElements, ElementAllocator>::GetCapacity() const
{
    return NumberOfElements;
}
