//
// Created by Andrew Fagan on 12/25/24.
//

#pragma once

/**
* @class PieceColor
* @brief This identifies the color of a piece
*/
class PieceColor
{
public:
    PieceColor() : m_Color(false) {}

    static const PieceColor BLACK;
    static const PieceColor WHITE;

    constexpr bool operator==(const PieceColor& otherPieceColor) const noexcept
    {
        return m_Color == otherPieceColor.m_Color;
    }

    constexpr bool operator!=(const PieceColor& otherPieceColor) const noexcept
    {
        return m_Color != otherPieceColor.m_Color;
    }

    constexpr PieceColor operator!() const noexcept
    {
        return PieceColor(!m_Color);
    }

    [[nodiscard]] constexpr bool IsWhite() const { return *this == WHITE; }
    [[nodiscard]] constexpr bool IsBlack() const { return *this == BLACK; }
    [[nodiscard]] constexpr PieceColor Opposite() const { return !*this; }

    constexpr PieceColor& operator=(const PieceColor& otherPieceColor) = default;
    constexpr PieceColor& operator=(PieceColor&&) = default;
    constexpr PieceColor(const PieceColor&) = default;
    constexpr PieceColor(PieceColor&&) = default;

    [[nodiscard]] constexpr bool GetRawValue() const
    {
        return m_Color;
    }

    [[nodiscard]] static constexpr PieceColor FromRawValue(const bool value)
    {
        return value ? WHITE : BLACK;
    }

private:
    explicit constexpr PieceColor(bool colorValue) : m_Color(colorValue) {}
    bool m_Color;
};

inline const PieceColor PieceColor::BLACK = PieceColor(false);
inline const PieceColor PieceColor::WHITE = PieceColor(true);
