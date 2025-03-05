/**
* @file VulkanRenderingContext.h
* @author Andrew Fagan
* @date 3/4/2025
*/

#pragma once

#include "Renderer/RenderingContext/RenderingContext.h"
#include <GLFW/glfw3.h>

namespace Graphics {

    /*
    * This sets up the rendering context for when Vulkan is used. It defines functions that are overridden from
    * RendererContext and also needs a window from the window solution to set the context to.
    */
    class VulkanRenderingContext : public RenderingContext
    {
    public:
        explicit VulkanRenderingContext(GLFWwindow* window);
        void Init() override;
        void Shutdown() override;

        std::string_view GetGraphicsProcessorName() override;
        std::string_view GetGPUVendor() override;
        std::string_view GetRenderingAPI() override;

    private:
        GLFWwindow* m_Window;
    };

}