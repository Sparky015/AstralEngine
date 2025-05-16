/**
* @file Renderpass.h
* @author Andrew Fagan
* @date 5/16/25
*/

#pragma once
#include "Renderer/RHI/Common/GraphicsSmartPointers.h"

namespace Astral {

    class Renderpass
    {
    public:
        virtual ~Renderpass() = default;

        virtual void BeginRenderpass(CommandBufferHandle commandBufferHandle) = 0;
        virtual void EndRenderpass(CommandBufferHandle commandBufferHandle) = 0;

        virtual void* GetNativeHandle() = 0;
    };

    using RenderpassHandle = GraphicsRef<Renderpass>;

}
