/**
* @file RenderTarget.h
* @author Andrew Fagan
* @date 5/13/25
*/

#pragma once

namespace Astral {

    class RenderTarget
    {
    public:
        virtual ~RenderTarget() = default;


        static RenderTarget* CreateRenderTarget();
    };

}