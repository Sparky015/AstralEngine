//
// Created by Andrew Fagan on 10/8/24.
//

#pragma once

#include "Common/GraphicsSmartPointers.h"
#include "Resources/Device.h"

#include <string_view>

namespace Astral {

    /**
     * RendererContext is an interface to which each Rendering API will override and provide their own way to create
     * their rendering context.
     */
    class RenderingContext
    {
    public:
        virtual ~RenderingContext() = default;

        virtual void Init() = 0;
        virtual void Shutdown() = 0;

        virtual Device& GetDevice() = 0;
        virtual uint32 GetNumValidationErrorsAndWarnings() = 0;
        virtual void ClearNumValidationErrorsAndWarnings() = 0;

        UVec2 GetFramebufferSize();

        virtual std::string_view GetRenderingAPI() = 0;
        virtual std::string_view GetGPUVendor() = 0;
        virtual std::string_view GetGraphicsProcessorName() = 0;

        virtual void* GetInstanceHandle() = 0;

        virtual void InitImGuiForAPIBackend(RenderPassHandle renderPassHandle) = 0;
        virtual void ShutdownImGuiForAPIBackend() = 0;

        static RenderingContext* CreateRendererContext();
    };

}
