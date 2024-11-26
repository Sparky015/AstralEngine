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
        uint8 column;
        uint8 row;

        char rowLetter = chessNotation[0];
        row = chessNotation[1] - '0';

        // Convert the letter to a row number
        switch (toupper(rowLetter))
        {
            case 'A': column = 1; break;
            case 'B': column = 2; break;
            case 'C': column = 3; break;
            case 'D': column = 4; break;
            case 'E': column = 5; break;
            case 'F': column = 6; break;
            case 'G': column = 7; break;
            case 'H': column = 8; break;
        }

        // Invert the column number (i.e. 8 becomes 1, 7, becomes 2, 6 becomes, 3...)
        uint8 invertedColumn = 9 - row;

        return ((invertedColumn - 1) * 8) + (column - 1);
    }


    const std::string ConvertIntToChessNotation(uint8 location)
    {
        if (location == 255) {return "Empty";}
        int row = (location / 8);
        int column = (location % 8) + 1;
        char columnLetter;

        // Convert row number to letter
        switch (column)
        {
            case 1: columnLetter = 'A'; break;
            case 2: columnLetter = 'B'; break;
            case 3: columnLetter = 'C'; break;
            case 4: columnLetter = 'D'; break;
            case 5: columnLetter = 'E'; break;
            case 6: columnLetter = 'F'; break;
            case 7: columnLetter = 'G'; break;
            case 8: columnLetter = 'H'; break;
        }

        // Invert the column number (i.e. 8 becomes 1, 7, becomes 2, 6 becomes, 3...)
        row = 9 - row;

        return std::string(1, columnLetter) + std::to_string(row - 1);
    }

    std::string ConvertVectorToString(const std::vector<uint8>& vec)
    {
        std::ostringstream oss;
        oss << "[";
        for (size_t i = 0; i < vec.size(); ++i)
        {
            oss << ConvertIntToChessNotation((int)vec[i]);
            if (i != vec.size() - 1)
            {
                oss << ", ";
            }
        }
        oss << "]";
        return oss.str();
    }

    std::string ConvertPieceTypeToString(const PieceType pieceType)
    {
        switch (pieceType)
        {
            case PieceType::PAWN: return "Pawn";
            case PieceType::ROOK: return "Rook";
            case PieceType::BISHOP: return "Bishop";
            case PieceType::KNIGHT: return "Knight";
            case PieceType::KING: return "King";
            case PieceType::QUEEN: return "Queen";
            case PieceType::NONE: return "None";
            default: throw std::logic_error("PieceType did not map to any string.");
        }
    }

}