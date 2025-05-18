/**
* @file Buffer.h
* @author Andrew Fagan
* @date 5/13/25
*/

#pragma once

#include "Core/CoreMacroDefinitions.h"
#include "Renderer/RHI/Common/GraphicsSmartPointers.h"

namespace Astral {

    class Buffer
    {
    public:
        virtual ~Buffer() = default;


        static Buffer* CreateBuffer();
    };

    using BufferHandle = GraphicsRef<Buffer>;

}
