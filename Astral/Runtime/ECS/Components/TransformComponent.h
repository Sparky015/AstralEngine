//
// Created by Andrew Fagan on 11/27/24.
//

#pragma once

#include "pch.h"

struct TransformComponent
{
    TransformComponent() : position(), scale(1.0f), rotation(0) {}
    TransformComponent(float positionX, float positionY) : position(positionX, positionY, 0), scale(1.0f), rotation(0) {}
    TransformComponent(float positionX, float positionY, float scaleX, float scaleY) :
                        position(positionX, positionY, 0), scale(scaleX, scaleY, 1.0f), rotation(0) {}
    TransformComponent(Vec3 position, Vec3 scale) : position(position), scale(scale), rotation(0) {}

    Vec3 position;
    Vec3 scale;
    Vec3 rotation;
};