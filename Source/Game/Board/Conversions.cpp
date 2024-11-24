//
// Created by Andrew Fagan on 11/24/24.
//

#include "Conversions.h"

namespace Game {

    PieceType ConvertCharToPieceType(char c)
    {
        c = tolower(c);
        switch (c)
        {
            case 'p':
                return PieceType::PAWN;
            case 'r':
                return PieceType::ROOK;
            case 'b':
                return PieceType::BISHOP;
            case 'n':
                return PieceType::KNIGHT;
            case 'k':
                return PieceType::KING;
            case 'q':
                return PieceType::QUEEN;
            default:
                throw std::logic_error("Char did not map to any character.");
        }
    }


    char ConvertPieceTypeToChar(PieceType pieceType)
    {
        switch (pieceType)
        {
            case PieceType::PAWN:
                return 'p';
            case PieceType::ROOK:
                return 'r';
            case PieceType::BISHOP:
                return 'b';
            case PieceType::KNIGHT:
                return 'n';
            case PieceType::KING:
                return 'k';
            case PieceType::QUEEN:
                return 'q';
            case PieceType::NONE:
                return '_';
            default:
                throw std::logic_error("PieceType did not map to any character.");
        }
    }


    uint8 ConvertChessNotationToInt(const std::string& chessNotation)
    {
        uint8 row;
        uint8 column;

        char rowLetter = chessNotation[0];
        column = chessNotation[1] - '0';

        // Convert the letter to a row number
        switch (toupper(rowLetter))
        {
            case 'A': row = 1; break;
            case 'B': row = 2; break;
            case 'C': row = 3; break;
            case 'D': row = 4; break;
            case 'E': row = 5; break;
            case 'F': row = 6; break;
            case 'G': row = 7; break;
            case 'H': row = 8; break;
        }

        // Invert the column number (i.e. 8 becomes 1, 7, becomes 2, 6 becomes, 3...)
        uint8 invertedColumn = 9 - column;

        return ((invertedColumn - 1) * 8) + (row - 1);
    }


    const std::string ConvertIntToChessNotation(uint8 location)
    {
        int column = (location / 8);
        int row = (location % 8) + 1;
        char rowLetter;

        // Convert row number to letter
        switch (row)
        {
            case 1: rowLetter = 'A'; break;
            case 2: rowLetter = 'B'; break;
            case 3: rowLetter = 'C'; break;
            case 4: rowLetter = 'D'; break;
            case 5: rowLetter = 'E'; break;
            case 6: rowLetter = 'F'; break;
            case 7: rowLetter = 'G'; break;
            case 8: rowLetter = 'H'; break;
        }

        // Invert the column number (i.e. 8 becomes 1, 7, becomes 2, 6 becomes, 3...)
        column = 9 - column;

        return std::string(1, rowLetter) + std::to_string(column - 1);
    }

}