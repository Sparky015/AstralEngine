//
// Created by Andrew Fagan on 10/8/24.
//

#pragma once

namespace Renderer {

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

        virtual std::string GetRenderingAPI() = 0;
        virtual std::string GetGraphicsVendor() = 0;
        virtual std::string GetGraphicsProcessor() = 0;

        static RendererContext* CreateRendererContext();
    };

}