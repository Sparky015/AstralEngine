/**
* @file Semaphore.h
* @author Andrew Fagan
* @date 5/14/25
*/

#pragma once
#include "Renderer/RHI/Common/GraphicsSmartPointers.h"

namespace Astral {

    class Semaphore
    {
    public:
        virtual ~Semaphore();

        virtual void* GetNativeHandle() = 0;
    };

    using SemaphoreHandle = GraphicsRef<Semaphore>;

}
