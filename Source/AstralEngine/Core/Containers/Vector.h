//
// Created by Andrew Fagan on 1/23/25.
//

#pragma once


#include <memory>

#include "Core/CoreMacroDefinitions.h"
#include "Debug/Macros/Asserts.h"

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

        /**@brief Constructs a Vector with the specified reserve size */
        explicit Vector(const size_t reserveSize) :
            m_Size(0),
            m_Capacity(reserveSize),
            m_Buffer(new ElementType[reserveSize])
        {}

        /**@brief Constructs a Vector with the given elements. Allocates a buffer that fits the exact amount of elements. */
        Vector(std::initializer_list<ElementType> initializerList) :
            m_Size(initializerList.size()),
            m_Capacity(initializerList.size()),
            m_Buffer(new ElementType[m_Capacity])
        {
            std::copy(initializerList.begin(), initializerList.end(), m_Buffer);
        }

        ~Vector()
        {
            delete[] m_Buffer;
        }

        /**@brief Adds an element to the end of the vector */
        inline void Add(ElementType&& element)
            requires (sizeof(ElementType) >= 8)
        {
            if (m_Size == m_Capacity) { ResizeBuffer((m_Capacity * GROWTH_FACTOR) + 1); }
            m_Buffer[m_Size] = std::move(element);
            m_Size++;
        }

        /**@brief Adds an element to the end of the vector */
        inline void Add(const ElementType& element)
            requires (sizeof(ElementType) >= 8)
        {
            if (m_Size == m_Capacity) { ResizeBuffer((m_Capacity * GROWTH_FACTOR) + 1); }
            m_Buffer[m_Size] = element;
            m_Size++;
        }

        /**@brief Adds an element to the end of the vector */
        inline void Add(ElementType element)
            requires (sizeof(ElementType) < 8)
        {
            if (m_Size == m_Capacity) { ResizeBuffer((m_Capacity * GROWTH_FACTOR) + 1); }
            m_Buffer[m_Size] = element;
            m_Size++;
        }

        // TODO: Fix this impl
        void Emplace(std::initializer_list<ElementType> elementInitializerList)
        {
            if (m_Size == m_Capacity) { ResizeBuffer((m_Capacity * GROWTH_FACTOR) + 1); }
            m_Buffer[m_Size] = ElementType(elementInitializerList);
            m_Size++;
        }

        void Remove(const ElementType& element)
        {

        }

        void Insert(size_t index)
        {

        }

        void Contains(const ElementType& element)
        {

        }

        /**@brief Sets the capacity of the vector to the specified size and allocates a buffer of the given size.
         *        Does nothing if the size passed in is smaller than the current capacity. */
        void Reserve(size_t size)
        {
            if (m_Capacity >= size) { return; }
            ResizeBuffer(size);
        }

        void ShrinkToFit();

        ElementType& operator[](size_t index)
        {
            ASSERT(index < m_Size, "Index is out of bounds!");
            return m_Buffer[index];
        }

        const ElementType& operator[](size_t index) const
        {
            ASSERT(index < m_Size, "Index is out of bounds!");
            return m_Buffer[index];
        }

        // TODO: Write iterator

        [[nodiscard]] constexpr size_t GetSize() const { return m_Size; }
        [[nodiscard]] constexpr size_t GetCapacity() const { return m_Capacity; }
        [[nodiscard]] constexpr size_t GetEndIndex() const { return m_Size - 1; }
        [[nodiscard]] constexpr bool IsValid() const { return m_Buffer != nullptr; }

        static constexpr int GROWTH_FACTOR = 2;

        bool operator==(const Vector& other) const
        {
            if (m_Size != other.m_Size) { return false; }

            if constexpr (std::is_trivially_copyable_v<ElementType>)
            {
                 return std::memcmp(m_Buffer, other.m_Buffer, m_Size * sizeof(ElementType)) == 0;
            }
            else
            {
                for (size_t i = 0; i < m_Size; i++)
                {
                    if (m_Buffer[i] != other.m_Buffer[i]) { return false; }
                }
            }

            return true;
        }

        Vector(const Vector& other)
        {
            m_Size = other.m_Size;
            m_Capacity = other.m_Capacity;

            // Copy data from old buffer
            m_Buffer = new ElementType[m_Capacity];
            std::memcpy(m_Buffer, other.m_Buffer, m_Size * sizeof(ElementType)); // Copies only the used part of the buffer
        }

        Vector& operator=(const Vector& other)
        {
            [[unlikely]] if (this == &other) { return *this; }
            Vector<ElementType> temp(other);
            std::swap(*this, temp);
            return *this;
        }

        Vector(Vector&& other) noexcept
        {
            m_Size = other.m_Size;
            m_Capacity = other.m_Capacity;
            m_Buffer = other.m_Buffer;
            other.m_Buffer = nullptr;
            other.m_Size = 0;
            other.m_Capacity = 0;
        }

        Vector& operator=(Vector&& other) noexcept
        {
            [[unlikely]] if (this == &other) { return *this; }
            m_Size = other.m_Size;
            m_Capacity = other.m_Capacity;
            m_Buffer = other.m_Buffer;
            other.m_Buffer = nullptr;
            other.m_Size = 0;
            other.m_Capacity = 0;
            return *this;
        }



    private:

        /**@brief Writes the data from the current buffer to the new buffer passed in.
         * @param outNewBuffer The new buffer to write the data to */
        void WriteDataToNewBuffer(ElementType* outNewBuffer)
        {
            if constexpr (std::is_trivially_copyable_v<ElementType>)
            {
                std::memcpy(outNewBuffer, m_Buffer, m_Size * sizeof(ElementType)); // Copies only the used part of the buffer
            }
            else
            {
                for (size_t i = 0; i < m_Size; i++)
                {
                    outNewBuffer[i] = std::move(m_Buffer[i]);
                }
            }
        }

        /**@brief Resizes the internal buffer to given capacity and copies the data over.
         *        Does nothing if the newCapacity is less than or equal to the current capacity.
         * @param newCapacity The new capacity to resize the internal array to */
        inline void ResizeBuffer(const size_t newCapacity)
        {
            ASSERT(newCapacity > m_Capacity, "New capacity must be greater than the current capacity!")

            [[unlikely]] if (m_Buffer == nullptr)
            {
                m_Buffer = new ElementType[newCapacity];
                m_Capacity = newCapacity;
                return;
            }

            ElementType* newBuffer = new ElementType[newCapacity];

            WriteDataToNewBuffer(newBuffer);

            delete[] m_Buffer;

            m_Capacity = newCapacity;
            m_Buffer = newBuffer;
        }



        size_t m_Size;
        size_t m_Capacity;
        ElementType* m_Buffer;
    };

}
