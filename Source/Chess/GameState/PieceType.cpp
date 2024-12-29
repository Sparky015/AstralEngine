/**
* @file PieceType.cpp
* @author Andrew Fagan
* @date 12/28/2024
*/

#include "PieceType.h"

std::string PieceType::ToString() const
{
    return std::string(ToStringView());
}


std::string_view PieceType::ToStringView() const
{
    switch (*this)
    {
        case NONE: return "None";
        case PAWN: return "Pawn";
        case BISHOP: return "Bishop";
        case KNIGHT: return "Knight";
        case ROOK: return "Rook";
        case QUEEN: return "Queen";
        case KING: return "King";
        default: ERROR("Invalid Type: " << this->GetRawValue());
    }
}


char PieceType::ToCharacterSymbol() const
{
    switch (*this)
    {
        case NONE: return '-';
        case PAWN: return 'p';
        case BISHOP: return 'b';
        case KNIGHT: return 'n';
        case ROOK: return 'r';
        case QUEEN: return 'q';
        case KING: return 'k';
        default: ERROR("Invalid Type: " << this->GetRawValue());
    }
}


PieceType PieceType::CreateFromCharacter(char symbol)
{
    symbol = tolower(symbol);
    switch (symbol)
    {
        case '-': return NONE;
        case 'p': return PAWN;
        case 'r': return ROOK;
        case 'b': return BISHOP;
        case 'n': return KNIGHT;
        case 'k': return KING;
        case 'q': return QUEEN;
        default: ERROR("Invalid Character: " << symbol);
    }
}
