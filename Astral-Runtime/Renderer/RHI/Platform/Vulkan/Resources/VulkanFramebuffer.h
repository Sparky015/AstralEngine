/**
* @file VulkanFramebuffer.h
* @author Andrew Fagan
* @date 5/16/25
*/

#pragma once

#include "Renderer/RHI/Resources/Framebuffer.h"

namespace Astral {

    struct VulkanFramebufferDesc
    {

    };

    class VulkanFramebuffer : public Framebuffer
    {
    public:
        explicit VulkanFramebuffer(const VulkanFramebufferDesc& desc);
        ~VulkanFramebuffer() override;



    private:

    };

}
