//
// Created by Andrew Fagan on 1/23/25.
//

#pragma once

#include "Debug/Macros/Asserts.h"
#include <cstring>
#include <memory>

namespace Astral {

    /**@brief Represents a fixed-size c-style array with convenience functions. */
    template<typename ElementType, size_t ArraySize>
    class Array
    {
    public:

        /**@brief Constructs an array filled with default constructed elements. */
        constexpr Array() noexcept { Fill(ElementType()); }

        /**@brief Constructs an array filled with the given element. */
        constexpr explicit Array(const ElementType& elementType) noexcept { Fill(elementType); }

        /**@brief Constructs an array with the given elements in the initializer list */
        Array(std::initializer_list<ElementType> initializerList)
        {
            ASSERT(initializerList.size() == ArraySize, "Size of initializer list and array do not match!")
            std::copy(initializerList.begin(), initializerList.end(), m_Data);
        };

        /**@brief Assigns the array instance the given elements in the initializer list */
        Array& operator=(std::initializer_list<ElementType> initializerList)
        {
            ASSERT(initializerList.size() == ArraySize, "Size of initializer list and array do not match!")
            std::copy(initializerList.begin(), initializerList.end(), m_Data);
            return *this;
        }

        /**@brief Fills the array with the given element.
         * @param fillElement The element to set the array to. */
        constexpr void Fill(const ElementType& fillElement) noexcept
        {
            std::fill_n(Begin(), ArraySize, fillElement);
        }

        /**@brief Checks if the given element is in the array.
         * @param element The element that will be checked for.
         * @return True if the element is in the array, false otherwise. */
        constexpr bool Contains(const ElementType& element) const noexcept
        {
            for (size_t i = 0; i < ArraySize; i++)
            {
                if (m_Data[i] == element) { return true; }
            }
            return false;
        }

        /**@brief Gets the size of the array. */
        [[nodiscard]] constexpr size_t Size() const noexcept { return ArraySize; }

        /**@brief Gets the last index of the array. */
        [[nodiscard]] constexpr size_t EndIndex() const noexcept { return ArraySize - 1; }

        /**@brief Gets the raw pointer of the internal c-style array. */
        [[nodiscard]] constexpr ElementType* RawData() noexcept { return m_Data; }

        /**@brief Gets the raw pointer of the internal c-style array. */
        [[nodiscard]] constexpr const ElementType* RawData() const noexcept { return m_Data; }

        /**@brief Reads an element at the given index. */
        [[nodiscard]] ElementType& Index(size_t index)
        {
            ASSERT(index < ArraySize, "Index is out of bounds!");
            return m_Data[index];
        }

        /**@brief Reads an element at the given index. */
        [[nodiscard]] ElementType& operator[](size_t index)
        {
            ASSERT(index < ArraySize, "Index is out of bounds!");
            return m_Data[index];
        }

        /**@brief Reads an element at the given index. */
        [[nodiscard]] const ElementType& operator[](size_t index) const
        {
            ASSERT(index < ArraySize, "Index is out of bounds!");
            return m_Data[index];
        }

        // Iterators (lowercase too for STL compatability)
        constexpr ElementType* Begin() noexcept { return &m_Data[0]; }
        constexpr ElementType* End() noexcept { return &m_Data[ArraySize]; }
        constexpr const ElementType* Begin() const noexcept{ return &m_Data[0]; }
        constexpr const ElementType* End() const noexcept { return &m_Data[ArraySize]; }
        constexpr ElementType* begin() noexcept{ return Begin(); }
        constexpr ElementType* end() noexcept { return End(); }
        constexpr const ElementType* begin() const noexcept { return Begin(); }
        constexpr const ElementType* end() const noexcept { return End(); }


        /**@brief Compares one array with another array
         * @warning The memory layout is compared and custom equality operators will be ignored for trivially copyable types. */
        bool operator==(const Array& other) const
        {
            if constexpr (std::is_trivially_copyable_v<ElementType>)
            {
                return std::memcmp(m_Data, other.m_Data, ArraySize * sizeof(ElementType)) == 0;
            }
            else
            {
                for (size_t i = 0; i < ArraySize; i++)
                {
                    if (m_Data[i] != other.m_Data[i]) { return false; }
                }
            }
            return true;
        }

        using value_type = ElementType;
        using size_type = std::size_t;
        using difference_type = std::ptrdiff_t;
        using reference = ElementType&;
        using const_reference = const ElementType&;
        using pointer = ElementType*;
        using const_pointer = const ElementType*;

    private:

        ElementType m_Data[ArraySize];
    };

}