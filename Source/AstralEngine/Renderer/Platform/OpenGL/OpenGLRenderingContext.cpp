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


    std::string_view OpenGLRenderingContext::GetRenderingAPI()
    {
        static std::string renderingAPI = "OpenGL " + std::string(reinterpret_cast<const char*>(glGetString(GL_VERSION)));
        return renderingAPI;
    }


    std::string_view OpenGLRenderingContext::GetGPUVendor()
    {
        return reinterpret_cast<const char*>(glGetString(GL_VENDOR));
    }


    std::string_view OpenGLRenderingContext::GetGraphicsProcessorName()
    {
        return reinterpret_cast<const char*>(glGetString(GL_RENDERER));
    }

}