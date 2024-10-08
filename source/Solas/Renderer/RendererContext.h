//
// Created by Andrew Fagan on 10/8/24.
//

#pragma once

namespace Solas::Renderer {


    /**
     * RendererContext is an interface to which each Rendering API will override and provide their own way to create
     * their rendering context.
     */
    class RendererContext
    {
    public:
        virtual ~RendererContext() = default;

        virtual void Init() = 0;
        virtual void SwapBuffers() = 0;
    };

}