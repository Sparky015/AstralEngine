//
// Created by Andrew Fagan on 11/28/24.
//

#pragma once

#include "pch.h"

struct SpriteComponent
{
    // TEMP PLACEHOLDER CODE
    SpriteComponent() : x(0), y(0), isUsed(false) {}
    SpriteComponent(int16 x, int16 y) : x(x), y(y), isUsed(true) {}
    int16 x;
    int16 y;
    bool isUsed;
};