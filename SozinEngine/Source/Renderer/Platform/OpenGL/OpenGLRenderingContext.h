//
// Created by Andrew Fagan on 10/8/24.
//

#pragma once

#include "Renderer/RendererContext.h"
#include <GLFW/glfw3.h>

namespace Graphics {

    /*
    * This sets up the rendering context for when OpenGL is used. It defines functions that are overridden from
    * RendererContext and also needs a window from the window solution to set the context to.
    */
    class OpenGLRenderingContext : public RendererContext
    {
    public:
        explicit OpenGLRenderingContext(GLFWwindow* window);
        void Init() override;
        void Shutdown() override;

        std::string GetGraphicsProcessor() override;
        std::string GetGraphicsVendor() override;
        std::string GetRenderingAPI() override;

    private:
        GLFWwindow* m_Window;
    };

}