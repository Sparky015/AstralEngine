//
// Created by Andrew Fagan on 1/23/25.
//

#pragma once

#include <initializer_list>
#include <memory>

namespace Astral {

    template<typename ElementType>
    class Vector
    {
    public:
        Vector() :
            m_Size(0),
            m_Capacity(0),
            m_Buffer(nullptr)
        {}

        explicit Vector(const size_t reserveSize) :
            m_Size(0),
            m_Capacity(reserveSize),
            m_Buffer(new ElementType[reserveSize])
        {}

        Vector(std::initializer_list<ElementType> initializerList) :
            m_Size(initializerList.size()),
            m_Capacity(initializerList.size()),
            m_Buffer(new ElementType[m_Capacity])
        {

        }

        ~Vector()
        {
            delete[] m_Buffer;
        }

        /**@brief  */
        inline void Add(const ElementType& element)
        {

        }

        void Emplace(std::initializer_list<ElementType> elementInitializerList);
        void Remove(const ElementType& element);
        void Insert(size_t index);
        void Contains(const ElementType& element);

        /**@brief  */
        void Reserve(size_t size)
        {

        }

        void ShrinkToFix();

        ElementType& operator[](size_t index);

        // TODO: Write iterator

        [[nodiscard]] constexpr size_t GetSize() const { return m_Size; }
        [[nodiscard]] constexpr size_t GetCapacity() const { return m_Capacity; }

        static constexpr int GROWTH_FACTOR = 2;
    private:

        inline void ResizeBuffer()
        {
            const size_t newCapacity = m_Capacity * GROWTH_FACTOR;
            ElementType* newBuffer = new ElementType[newCapacity];

            std::memcpy(newBuffer, m_Buffer, m_Capacity);
            delete[] m_Buffer;

            m_Capacity = newCapacity;
            m_Buffer = newBuffer;
        }

        size_t m_Size;
        size_t m_Capacity;
        ElementType* m_Buffer;
    };

}
