/**
* @file StringLiteral.h
* @author Andrew Fagan
* @date 5/17/26
*/

#pragma once

#include "Core/FixedIntegerTypes.h"

#include <string_view>

namespace Astral {

    /**
     * @brief Defines a container for a string literal with compile-time enforcement and compile-time string hashing
     */
    class StringLiteral
    {
    public:

        /**
         * @brief Constructor that enforces compile-time string literals
         * @param stringLiteral The string literal to store
         */
        template <size_t N>
        consteval StringLiteral(const char (&stringLiteral)[N]);

        /**
         * @brief Gets the pointer to the string literal
         * @return The pointer to the string literal
         */
        constexpr const char* Data() const;

        /**
         * @brief Gets the size of the string literal
         * @return The size of the string literal
         */
        constexpr size_t Size() const;

        /**
         * @brief Checks if the string is empty ("")
         * @return True if the string is empty and false otherwise
         */
        constexpr bool Empty() const;

        /**
         * @brief Gets the compile-time computed hash of the string literal
         * @return The compile-time computed hash of the string literal
         */
        constexpr uint64 PrecomputedHash() const;

        /**
         * @brief Gets the character at the given index of the string literal
         * @param index The index to use
         * @return The character at the given index
         */
        constexpr char operator[](size_t index) const;

        /**
         * @brief Equality operator for StringLiteral classes
         * @param other StringLiteral to compare against
         * @return True if the strings are the same and false otherwise
         * @note This uses compile-time computed hashes for a fast path comparison with a runtime strcmp fallback on hash matches/collisions
         */
        constexpr bool operator==(StringLiteral other) const;

        /**
         * @brief Less Than operator for hash-ordering for string literals
         * @param other StringLiteral to compare against
         * @return True if the string hash is less than the other string hash
         * @note This uses compile-time computed hashes for a fast path comparison with a runtime strcmp fallback on hash matches/collisions
         */
        constexpr bool operator<(StringLiteral other) const;

        /**
         * @brief Compile-time converter to std::string_view for compatability
         */
        constexpr operator std::string_view() const noexcept;


    private:

        /**
         * @brief Computes hash of string at compile-time using FNV-1a hashing function
         */
        consteval uint64 ComputeHash(const char* data, size_t length);

        const char* m_Data;
        size_t m_Size;
        uint64 m_Hash;
    };




// =============================================  Inline Definitions  =============================================


    template<size_t N>
    consteval StringLiteral::StringLiteral(const char (&stringLiteral)[N]) :
        m_Data(stringLiteral),
        m_Size(N - 1),
        m_Hash(ComputeHash(m_Data, m_Size))
    {}


    constexpr const char* StringLiteral::Data() const
    {
        return m_Data;
    }


    constexpr size_t StringLiteral::Size() const
    {
        return m_Size;
    }


    constexpr bool StringLiteral::Empty() const
    {
        return m_Size == 0;
    }


    constexpr uint64 StringLiteral::PrecomputedHash() const
    {
        return m_Hash;
    }


    constexpr char StringLiteral::operator[](size_t index) const
    {
        return m_Data[index];
    }


    constexpr bool StringLiteral::operator==(StringLiteral other) const
    {
        return m_Hash == other.m_Hash && std::string_view(*this) == std::string_view(other);
    }


    constexpr bool StringLiteral::operator<(StringLiteral other) const
    {
        if (m_Hash != other.m_Hash)
        {
            return m_Hash < other.m_Hash;
        }
        else
        {
            return std::string_view(*this) < std::string_view(other);
        }
    }


    constexpr StringLiteral::operator std::string_view() const noexcept
    {
        return std::string_view(m_Data, m_Size);
    }


    consteval uint64 StringLiteral::ComputeHash(const char* data, size_t length)
    {
        const uint64 FNV_PRIME_64  = 1099511628211ull;
        const uint64 FNV_OFFSET_64 = 14695981039346656037ull;

        uint64 hash = FNV_OFFSET_64;

        for (size_t i = 0; i < length; i++)
        {
            hash = (hash ^ static_cast<uint8>(data[i])) * FNV_PRIME_64;
        }

        return hash;
    }

}

template <>
struct std::hash<Astral::StringLiteral>
{
    constexpr size_t operator()(const Astral::StringLiteral& stringLiteral) const noexcept
    {
        return stringLiteral.PrecomputedHash();
    }
};
