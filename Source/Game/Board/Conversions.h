//
// Created by Andrew Fagan on 11/24/24.
//

#pragma once

#include "Game/PieceInfo.h"

namespace Game {

    PieceType ConvertCharToPieceType(char c);
    char ConvertPieceTypeToChar(PieceType pieceType);
    uint8 ConvertChessNotationToInt(const std::string& chessNotation);
    const std::string ConvertIntToChessNotation(uint8 chessNotation);

    std::string VectorToString(const std::vector<uint8>& vec);

    template <typename T>
    std::string VectorToString(const std::vector<T>& vec)
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

}