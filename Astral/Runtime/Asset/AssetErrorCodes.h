/**
* @file AssetErrorCodes.h
* @author Andrew Fagan
* @date 4/11/25
*/

#pragma once

#include "Core/FixedIntegerTypes.h"

namespace Astral {

    enum class AssetErrorCode : uint8
    {
        SUCCESS,
        FAILED_TO_LOAD_RESOURCE,
    };

}