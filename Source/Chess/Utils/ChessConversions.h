//
// Created by Andrew Fagan on 11/24/24.
//

#pragma once

#include "../GameState/ChessPieceTypes.h"
#include "MoveGeneration/PieceMoveList.h"

namespace Game {

    uint8 ConvertChessNotationToInt(const std::string& chessNotation);
    const std::string ConvertIntToChessNotation(uint8 chessNotation);

    std::string ConvertVectorToString(const PieceMoveList& vec);

    template <typename T>
    std::string ConvertVectorToString(const std::vector<T>& vec)
    {
        std::ostringstream oss;
        oss << "[";
        for (size_t i = 0; i < vec.size(); ++i)
        {
            oss << vec[i];
            if (i != vec.size() - 1)
            {
                oss << ", ";
            }
        }
        oss << "]";
        return oss.str();
    }

    Vec2 ConvertPieceLocationToCoordinates(uint8 pieceLocation);
    uint8 ConvertCoordinatesToPieceLocation(Vec2 coordinates);

}
