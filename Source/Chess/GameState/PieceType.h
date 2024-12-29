/**
* @file PieceType.h
* @author Andrew Fagan
* @date 12/28/2024
*/

#pragma once


#include "pch.h"
#include "Core/CoreMacroDefinitions.h"

/**
 * @enum PieceType This identifies the type of a piece.
*/
enum class PieceTypeEnum : uint8
{
    NONE,

    PAWN,
    BISHOP,
    KNIGHT,
    ROOK,
    QUEEN,
    KING
};


class PieceType final
{
public:
    constexpr PieceType() : m_Type(NONE) {}
    constexpr PieceType(const PieceTypeEnum type) : m_Type(type) {}

    static constexpr PieceTypeEnum NONE = PieceTypeEnum::NONE;
    static constexpr PieceTypeEnum PAWN = PieceTypeEnum::PAWN;
    static constexpr PieceTypeEnum BISHOP = PieceTypeEnum::BISHOP;
    static constexpr PieceTypeEnum KNIGHT = PieceTypeEnum::KNIGHT;
    static constexpr PieceTypeEnum ROOK = PieceTypeEnum::ROOK;
    static constexpr PieceTypeEnum QUEEN = PieceTypeEnum::QUEEN;
    static constexpr PieceTypeEnum KING = PieceTypeEnum::KING;

    static constexpr uint8 NUM_OF_PIECE_TYPES = static_cast<uint8>(KING) + 1;

    [[nodiscard]] constexpr bool IsNone() const { return m_Type == NONE; }
    [[nodiscard]] constexpr bool IsPawn() const { return m_Type == PAWN; }
    [[nodiscard]] constexpr bool IsBishop() const { return m_Type == BISHOP; }
    [[nodiscard]] constexpr bool IsKnight() const { return m_Type == KNIGHT; }
    [[nodiscard]] constexpr bool IsRook() const { return m_Type == ROOK; }
    [[nodiscard]] constexpr bool IsQueen() const { return m_Type == QUEEN; }
    [[nodiscard]] constexpr bool IsKing() const { return m_Type == KING; }

    [[nodiscard]] std::string ToString() const;
    [[nodiscard]] std::string_view ToStringView() const;
    [[nodiscard]] char ToCharacterSymbol() const;
    [[nodiscard]] static PieceType CreateFromCharacter(char symbol);

    [[nodiscard]] constexpr PieceTypeEnum GetEnumValue() const { return m_Type; }
    [[nodiscard]] constexpr uint8 GetRawValue() const { return static_cast<uint8>(m_Type); }
    [[nodiscard]] static constexpr PieceType CreateFromRawValue(const uint8 typeValue) { return static_cast<PieceTypeEnum>(typeValue); }

    constexpr bool operator==(const PieceType& otherPieceType) const { return m_Type == otherPieceType.m_Type; }
    constexpr bool operator!=(const PieceType& otherPieceType) const { return m_Type != otherPieceType.m_Type; }
    constexpr bool operator==(const PieceTypeEnum& otherPieceTypeEnum) const { return m_Type == otherPieceTypeEnum; }
    constexpr bool operator!=(const PieceTypeEnum& otherPieceTypeEnum) const { return m_Type != otherPieceTypeEnum; }
    constexpr operator PieceTypeEnum() const { return m_Type; }

    PieceType(const PieceType&) = default;
    PieceType(PieceType&&) = default;
    PieceType& operator=(const PieceType&) = default;
    PieceType& operator=(PieceType&&) = default;

private:
    PieceTypeEnum m_Type;
};
