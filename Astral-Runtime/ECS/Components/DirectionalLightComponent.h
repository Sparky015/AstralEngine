/**
* @file DirectionalLightComponent.h
* @author Andrew Fagan
* @date 9/1/25
*/

#pragma once

#include "Core/Math/Math.h"

namespace Astral {

    struct DirectionalLightComponent
    {
        DirectionalLightComponent() : Direction(0, -1, 0), LightColor(1.0f), Intensity(1.0f) {}
        Vec3 Direction;
        Vec3 LightColor;
        float Intensity;
    };

}
