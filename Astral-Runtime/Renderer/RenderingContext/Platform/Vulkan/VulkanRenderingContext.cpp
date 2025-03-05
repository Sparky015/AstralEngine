/**
* @file VulkanRenderingContext.cpp
* @author Andrew Fagan
* @date 3/4/2025
*/

#include "VulkanRenderingContext.h"

#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>

#include "Debug/Instrumentation/ScopeProfiler.h"
#include "Debug/Utilities/Error.h"

namespace Graphics {

    VulkanRenderingContext::VulkanRenderingContext(GLFWwindow* window) : m_Window(window) {}


    void VulkanRenderingContext::Init()
    {
        PROFILE_SCOPE("Vulkan Rendering Context Initialization");

        glfwMakeContextCurrent(m_Window);

        if (!glfwVulkanSupported()) { ASTRAL_ERROR("GLFW can't find Vulkan!"); }

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    }


    void VulkanRenderingContext::Shutdown()
    {
        PROFILE_SCOPE("Vulkan Rendering Context Shutdown");
        // glfwDestroyWindow(m_Window);
    }


    std::string_view VulkanRenderingContext::GetGraphicsProcessorName()
    {
        return "";
    }


    std::string_view VulkanRenderingContext::GetGPUVendor()
    {
        return "";
    }


    std::string_view VulkanRenderingContext::GetRenderingAPI()
    {
        return "";
    }

}

