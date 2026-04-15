/**
* @file RendererEvents.h
* @author Andrew Fagan
* @date 6/13/2025
*/


#pragma once

#include "Core/FixedIntegerTypes.h"

namespace Astral {

    /**
     * @struct ViewportResizedEvent
     * @brief Defines the size of the new viewport to for when the viewport is resized
     */
    struct ViewportResizedEvent
    {
        uint32 Width;
        uint32 Height;
    };

}
