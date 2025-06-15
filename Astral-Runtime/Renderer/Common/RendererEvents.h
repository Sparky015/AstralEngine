/**
* @file RendererEvents.h
* @author Andrew Fagan
* @date 6/13/2025
*/


#pragma once

#include "Core/FixedIntegerTypes.h"

namespace Astral {

    struct ViewportResizedEvent
    {
        uint32 Width;
        uint32 Height;
    };

}
