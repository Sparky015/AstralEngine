/**
* @file PointLightComponent.h
* @author Andrew Fagan
* @date 6/28/25
*/

#pragma once

#include "Core/Math/Math.h"

namespace Astral {

    struct PointLightComponent
    {
        PointLightComponent() : LightColor(1.0f), Intensity(1.0f) {}

        Vec3 LightColor;
        float Intensity;
    };

}
