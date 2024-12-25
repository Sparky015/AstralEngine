//
// Created by Andrew Fagan on 12/25/24.
//

#pragma once

#include "Core/CoreMacroDefinitions.h"

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

class SquareLocation
{
public:
    constexpr SquareLocation() : m_Location(EMPTY) {}
    constexpr SquareLocation(BoardLocation squareLocation) : m_Location(static_cast<uint8>(squareLocation)) {};
    constexpr SquareLocation(uint8 squareLocation) : m_Location(squareLocation) {}

    [[nodiscard]] constexpr bool IsOnBoard() const { return m_Location < 64; }
    [[nodiscard]] constexpr bool IsOffBoard() const { return m_Location > 63; }
    [[nodiscard]] constexpr uint8 GetRow() const { return 8 - (m_Location / 8); }
    [[nodiscard]] constexpr uint8 GetColumn() const { return (m_Location % 8) + 1; }
    [[nodiscard]] constexpr bool IsInSameRow(SquareLocation squareLocation) const { return GetRow() == squareLocation.GetRow(); }
    [[nodiscard]] constexpr bool IsInSameColumn(SquareLocation squareLocation) const { return GetColumn() == squareLocation.GetColumn(); }
    [[nodiscard]] constexpr uint8 GetRawValue() const { return m_Location; }

    static constexpr SquareLocation FromRowAndColumn(uint8 row, uint8 column)
    {
        uint8 rawLocation = ((8 - row) * 8) + (column - 1);
        return SquareLocation(rawLocation);
    };


    constexpr SquareLocation& operator=(const SquareLocation&) = default;
    constexpr SquareLocation& operator=(SquareLocation&&) = default;
    constexpr SquareLocation(const SquareLocation&) = default;
    constexpr SquareLocation(SquareLocation&&) = default;

    constexpr bool operator==(const SquareLocation& otherSquareLocation) const { return m_Location == otherSquareLocation.m_Location; }
    constexpr bool operator!=(const SquareLocation& otherSquareLocation) const { return m_Location != otherSquareLocation.m_Location; }
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

