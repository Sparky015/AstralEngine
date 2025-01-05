//
// Created by Andrew Fagan on 12/25/24.
//

#pragma once

#include "Core/CoreMacroDefinitions.h"
#include "pch.h"

enum [[maybe_unused]] BoardLocation : uint8
{
    A8, B8, C8, D8, E8, F8, G8, H8,
    A7, B7, C7, D7, E7, F7, G7, H7,
    A6, B6, C6, D6, E6, F6, G6, H6,
    A5, B5, C5, D5, E5, F5, G5, H5,
    A4, B4, C4, D4, E4, F4, G4, H4,
    A3, B3, C3, D3, E3, F3, G3, H3,
    A2, B2, C2, D2, E2, F2, G2, H2,
    A1, B1, C1, D1, E1, F1, G1, H1,

    EMPTY = 255
};

class SquareLocation final
{
public:
    constexpr SquareLocation() : m_Location(EMPTY) {}
    constexpr SquareLocation(const BoardLocation squareLocation) : m_Location(static_cast<uint8>(squareLocation)) {};
    constexpr SquareLocation(const uint8 squareLocation) : m_Location(squareLocation) {}


    [[nodiscard]] constexpr bool IsOnBoard() const { return m_Location < 64; }
    [[nodiscard]] constexpr bool IsOffBoard() const { return m_Location > 63; }
    [[nodiscard]] constexpr bool IsEmpty() const { return m_Location == EMPTY; }
    [[nodiscard]] constexpr uint8 GetRow() const { return 8 - (m_Location / 8); }
    [[nodiscard]] constexpr uint8 GetColumn() const { return (m_Location % 8) + 1; }
    [[nodiscard]] constexpr bool IsInSameRow(const SquareLocation squareLocation) const { return GetRow() == squareLocation.GetRow(); }
    [[nodiscard]] constexpr bool IsInSameColumn(const SquareLocation squareLocation) const { return GetColumn() == squareLocation.GetColumn(); }
    [[nodiscard]] std::string GetString() const { return std::to_string(m_Location); }
    [[nodiscard]] constexpr uint8 GetRawValue() const { return m_Location; }
    [[nodiscard]] std::string GetChessNotation() const;

    static constexpr SquareLocation FromRowAndColumn(const uint8 row, const uint8 column)
    {
        const uint8 rawLocation = ((8 - row) * 8) + (column - 1);
        return SquareLocation(rawLocation);
    }


    constexpr SquareLocation& operator=(const SquareLocation&) = default;
    constexpr SquareLocation& operator=(SquareLocation&&) = default;
    constexpr SquareLocation(const SquareLocation&) = default;
    constexpr SquareLocation(SquareLocation&&) = default;

    constexpr bool operator==(const SquareLocation& otherSquareLocation) const { return m_Location == otherSquareLocation.m_Location; }
    constexpr bool operator!=(const SquareLocation& otherSquareLocation) const { return m_Location != otherSquareLocation.m_Location; }
    constexpr std::strong_ordering operator<=>(const SquareLocation& otherSquareLocation) const { return m_Location <=> otherSquareLocation.m_Location; }

    constexpr SquareLocation operator+(const int8 moveStep) const { return SquareLocation(m_Location + moveStep); }
    constexpr SquareLocation operator-(const int8 moveStep) const { return SquareLocation(m_Location - moveStep); }
    constexpr SquareLocation operator+(const SquareLocation& otherSquareLocation) const { return SquareLocation(m_Location + otherSquareLocation.m_Location); }
    constexpr SquareLocation operator-(const SquareLocation& otherSquareLocation) const { return SquareLocation(m_Location - otherSquareLocation.m_Location); }
    constexpr SquareLocation& operator+=(const SquareLocation& otherSquareLocation) { m_Location += otherSquareLocation.m_Location; return *this; }
    constexpr SquareLocation& operator-=(const SquareLocation& otherSquareLocation) { m_Location -= otherSquareLocation.m_Location; return *this; }
    constexpr uint8 operator%(const uint8 modByValue) const { return m_Location % modByValue; }
    constexpr SquareLocation& operator++() { ++m_Location; return *this; }
    constexpr SquareLocation operator++(int) { SquareLocation temp = *this; ++m_Location; return temp; }
    constexpr SquareLocation& operator--() { --m_Location; return *this; }
    constexpr SquareLocation operator--(int) { SquareLocation temp = *this; --m_Location; return temp; }


private:

    uint8 m_Location;
};

