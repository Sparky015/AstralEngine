//
// Created by Andrew Fagan on 11/18/24.
//

#pragma once

#include "Game/PieceInfo.h"

namespace Game {

    class ChessPiece
    {
        virtual void GenerateMoves(const uint8 pieceLocation, const PieceColor pieceColor) = 0;
    };

}

