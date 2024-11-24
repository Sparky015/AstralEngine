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

}