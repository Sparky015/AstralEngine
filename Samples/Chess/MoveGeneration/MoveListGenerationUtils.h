/**
* @file MoveListGenerationUtils.h
* @author Andrew Fagan
* @date 12/14/2024
*/

#pragma once

#include "Core/CoreMacroDefinitions.h"
#include "Common/ChessPieceTypes.h"
#include <cmath>



namespace Game {

    constexpr bool IsMoveWithinBounds(SquareLocation currentLocation, int8 moveStep)
    {
        SquareLocation potentialLocation = (currentLocation + moveStep);
        if (potentialLocation.IsOffBoard()) { return false; }

        const int8 currentLocationRow = currentLocation.GetRow();
        const int8 currentLocationColumn = currentLocation.GetColumn();
        const int8 potentialLocationRow = potentialLocation.GetRow();
        const int8 potentialLocationColumn = potentialLocation.GetColumn();

        if (std::abs(currentLocationRow - potentialLocationRow) > 2) {return false;}
        if (std::abs(currentLocationColumn - potentialLocationColumn) > 2) {return false;}

        return true;
    }

}
