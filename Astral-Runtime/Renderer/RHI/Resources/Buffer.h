/**
* @file Buffer.h
* @author Andrew Fagan
* @date 5/13/25
*/

#pragma once

#include "Core/CoreMacroDefinitions.h"

namespace Astral {

    class Buffer
    {
    public:
        virtual ~Buffer() = default;


        static Buffer* CreateBuffer();
    };

}