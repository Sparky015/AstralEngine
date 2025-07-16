//
// Created by Andrew Fagan on 10/8/24.
//

#include "OpenGLRenderingContext.h"

#include <glad/glad.h>

namespace Solas::Renderer{

    OpenGLRenderingContext::OpenGLRenderingContext(GLFWwindow* window) :
        m_Window(window)
    {}

    void OpenGLRenderingContext::Init()
    {
        glfwMakeContextCurrent(m_Window);
        int const status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        if (status != 1)
        {
            AY_ERROR("GLAD failed to load!");
        }
    }

    void OpenGLRenderingContext::SwapBuffers()
    {
        glfwSwapBuffers(m_Window);
    }

}