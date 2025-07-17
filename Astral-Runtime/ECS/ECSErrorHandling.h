/**
* @file ECSErrorHandling.h
* @author Andrew Fagan
* @date 3/8/2025
*/

#pragma once

#include "Core/FixedIntegerTypes.h"

namespace Astral {

    enum class ECS_Result : uint8
    {
        ECS_SUCCESS = 0,
        ECS_UNKNOWN_ERROR = 1,
        ECS_COMPONENT_NOT_PRESENT = 2
        // Will add more later as the error conditions appear
    };

}