//
// Created by Andrew Fagan on 1/2/25.
//

#include "SquareLocation.h"

std::string SquareLocation::GetChessNotation() const
{
    if (IsEmpty()) {return "Empty";}
    int row = GetRow();
    int column = GetColumn();
    std::string columnLetter;

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

    return columnLetter + std::to_string(row);
}
