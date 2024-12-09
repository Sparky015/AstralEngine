//
// Created by Andrew Fagan on 11/25/24.
//

#include "MoveGenerationHelpers.h"
#include <cmath>

namespace Game {

    bool IsWithinBounds(uint8 currentLocation, int8 moveStep)
    {
        uint8 potentialLocation = (currentLocation + moveStep);
        if (potentialLocation > 63) { return false; }

        int8 currentLocationRow = 9 - (currentLocation / 8);
        int8 currentLocationColumn = (currentLocation % 8) + 1;
        int8 potentialLocationRow = 9 - (potentialLocation / 8);
        int8 potentialLocationColumn = (potentialLocation % 8) + 1;

        if (std::abs(currentLocationRow - potentialLocationRow) > 2) {return false;}
        if (std::abs(currentLocationColumn - potentialLocationColumn) > 2) {return false;}

        return true;
    }

}