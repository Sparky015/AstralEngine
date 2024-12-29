//
// Created by Andrew Fagan on 11/24/24.
//

#include "ChessConversions.h"


namespace Game {

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
            default: ERROR("Invalid Switch Statement Input");
        }

        // Invert the column number (i.e. 8 becomes 1, 7, becomes 2, 6 becomes, 3...)
        row = 9 - row;

        return std::string(1, columnLetter) + std::to_string(row - 1);
    }


    std::string ConvertPieceMoveListToString(const PieceMoveList& vec)
    {
        std::ostringstream oss;
        oss << "[";
        for (size_t i = 0; i < vec.Size(); ++i)
        {
            oss << ConvertIntToChessNotation((int)vec[i].GetRawValue());
            if (i != vec.Size() - 1) // Adding comma in between each location
            {
                oss << ", ";
            }
        }
        oss << "]";
        return oss.str();
    }





    Vec2 ConvertPieceLocationToCoordinates(uint8 pieceLocation)
    {
        if (pieceLocation == 255) {return {0,0};}
        int row = (pieceLocation / 8);
        int column = (pieceLocation % 8) + 1;
        row = 9 - row; // invert the row
        return Vec2((column - 1) * 100 + 50,(row - 2) * 100 + 50);
    }


    uint8 ConvertCoordinatesToPieceLocation(Vec2 coordinates)
    {
        int row = ((800 - coordinates.x) / 100);
        int column = ((800 - coordinates.y) / 100) + 1;
        row = 9 - row; // invert the row
        return ((column - 1) * 8) + (row - 1) - 1;
    }

}
