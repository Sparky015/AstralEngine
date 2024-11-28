//
// Created by Andrew Fagan on 11/27/24.
//

#pragma once

#include "pch.h"

struct TransformComponent
{
    TransformComponent() : x(0), y(0), isUsed(false) {}
    TransformComponent(int16 x, int16 y) : x(x), y(y), isUsed(true) {}
    int16 x;
    int16 y;
    bool isUsed;
};