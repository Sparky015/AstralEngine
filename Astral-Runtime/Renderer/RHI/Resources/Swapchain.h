/**
* @file Swapchain.h
* @author Andrew Fagan
* @date 5/14/25
*/

#pragma once

#include "Core/CoreMacroDefinitions.h"

namespace Astral {

    class Swapchain
    {
    public:
        virtual ~Swapchain() = default;

        virtual uint32 AcquireNextImage() = 0;

        virtual void* GetNativeHandle() = 0;
    };

}
