//
// Created by Andrew Fagan on 10/8/24.
//

#pragma once

#include "Solas/Renderer/RendererContext.h"

#include <GLFW/glfw3.h>

namespace Solas::Renderer{

    /*
    * This sets up the rendering context for when OpenGL is used. It defines functions that are overridden from
    * RendererContext and also needs a window from the window solution to set the context to.
    */
    class OpenGLRenderingContext : public RendererContext
    {
    public:
        explicit OpenGLRenderingContext(GLFWwindow* window);
        virtual void Init() override;
        virtual void SwapBuffers() override;

    private:
        GLFWwindow* m_Window;
    };

}