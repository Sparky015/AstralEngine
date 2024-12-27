//
// Created by Andrew Fagan on 11/27/24.
//

#pragma once

#include "pch.h"

struct TransformComponent
{
    TransformComponent() : x(0), y(0), z(0), scaleX(1), scaleY(1), scaleZ(1), isUsed(false) {}
    TransformComponent(float x, float y) : x(x), y(y), z(0), scaleX(1), scaleY(1), scaleZ(1), isUsed(true) {}
    TransformComponent(float x, float y, float scaleX, float scaleY) :
            x(x), y(y), z(0), scaleX(scaleX), scaleY(scaleY), scaleZ(1), isUsed(true) {}
    TransformComponent(Vec3 position, Vec3 scale) :
            x(position.x), y(position.y), z(position.z), scaleX(scale.x), scaleY(scale.y), scaleZ(scale.z), isUsed(true) {}

    float x;
    float y;
    float z;
    float scaleX;
    float scaleY;
    float scaleZ;
    bool isUsed;
};