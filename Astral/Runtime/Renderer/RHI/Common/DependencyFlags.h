/**
* @file DependencyFlags.h
* @author Andrew Fagan
* @date 7/13/2025
*/


#pragma once

#include "Core/FixedIntegerTypes.h"

namespace Astral {

    enum DependencyFlags : uint8
    {
        BY_REGION_BIT      =  1u,
        DEVICE_GROUP_BIT   =  1u << 1,
        VIEW_LOCAL_BIT     =  1u << 2,
        FEEDBACK_LOOP_BIT  =  1u << 3
    };

}
