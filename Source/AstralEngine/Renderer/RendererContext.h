//
// Created by Andrew Fagan on 10/8/24.
//

#pragma once

namespace Graphics {

    /**
     * RendererContext is an interface to which each Rendering API will override and provide their own way to create
     * their rendering context.
     */
    class RendererContext
    {
    public:
        virtual ~RendererContext() = default;

        virtual void Init() = 0;
        virtual void Shutdown() = 0;

        virtual const char* GetRenderingAPI() = 0;
        virtual const char* GetGPUVendor() = 0;
        virtual const char* GetGraphicsProcessorName() = 0;

        static RendererContext* CreateRendererContext();
    };

}