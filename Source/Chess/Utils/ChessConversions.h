//
// Created by Andrew Fagan on 11/24/24.
//

#pragma once

#include "../GameState/ChessPieceTypes.h"

namespace Game {

    PieceType ConvertCharToPieceType(char c);
    char ConvertPieceTypeToChar(PieceType pieceType);
    uint8 ConvertChessNotationToInt(const std::string& chessNotation);
    const std::string ConvertIntToChessNotation(uint8 chessNotation);

    std::string ConvertVectorToString(const std::vector<uint8>& vec);
    std::string ConvertPieceTypeToString(const PieceType pieceType);

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