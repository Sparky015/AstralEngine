//
// Created by Andrew Fagan on 10/8/24.
//

#include "OpenGLRenderingContext.h"

#include <glad/glad.h>

namespace Graphics {

    OpenGLRenderingContext::OpenGLRenderingContext(GLFWwindow* window) :
        m_Window(window)
    {}

    void OpenGLRenderingContext::Init()
    {
        PROFILE_SCOPE("OpenGL Rendering Context Initialization");
        glfwMakeContextCurrent(m_Window);
        int const status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        if (status != 1)
        {
            ERROR("GLAD failed to load!");
        }
    }


    void OpenGLRenderingContext::Shutdown()
    {
        PROFILE_SCOPE("OpenGL Rendering Context Shutdown");
        glfwDestroyWindow(m_Window);
    }


    std::string OpenGLRenderingContext::GetRenderingAPI()
    {
        std::stringstream ss;
        ss << "OpenGL " << glGetString(GL_VERSION);
        return ss.str();
    }


    std::string OpenGLRenderingContext::GetGPUVendor()
    {
        std::stringstream ss;
        ss << glGetString(GL_VENDOR);
        return ss.str();
    }


    std::string OpenGLRenderingContext::GetGraphicsProcessor()
    {
        std::stringstream ss;
        ss << glGetString(GL_RENDERER);
        return ss.str();
    }

}