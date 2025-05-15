//
// Created by Andrew Fagan on 10/8/24.
//

#pragma once
#include <string_view>

#include "Common/GraphicsSmartPointers.h"
#include "Resources/Device.h"

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

        virtual std::string_view GetRenderingAPI() = 0;
        virtual std::string_view GetGPUVendor() = 0;
        virtual std::string_view GetGraphicsProcessorName() = 0;

        static RenderingContext* CreateRendererContext();
    };

}
