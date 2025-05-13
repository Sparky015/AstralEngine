/**
* @file Framebuffer.h
* @author Andrew Fagan
* @date 5/13/25
*/

#pragma once

namespace Astral {

    class Framebuffer
    {
    public:
        virtual ~Framebuffer() = default;


        static Framebuffer* CreateFramebuffer();
    };

}