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


    const char* OpenGLRenderingContext::GetRenderingAPI()
    {
        static char renderingAPI[100] = "OpenGL ";
        static char* renderingAPIWithVersion = std::strcat(renderingAPI, reinterpret_cast<const char*>(glGetString(GL_VERSION)));
        return renderingAPIWithVersion;
    }


    const char* OpenGLRenderingContext::GetGPUVendor()
    {
        return reinterpret_cast<const char*>(glGetString(GL_VENDOR));
    }


    const char* OpenGLRenderingContext::GetGraphicsProcessorName()
    {
        return reinterpret_cast<const char*>(glGetString(GL_RENDERER));
    }

}