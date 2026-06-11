/**
* @file ShaderStage.h
* @author Andrew Fagan
* @date 5/30/26
*/

#pragma once

#include "Core/CoreMacroDefinitions.h"

namespace Astral {

    /**
    * @brief Defines shader stages
    */
    enum class ShaderStage : uint8
    {
        VERTEX,
        FRAGMENT,
        COMPUTE,
        ALL
    };

}
