/**
* @file VulkanRenderingContext.cpp
* @author Andrew Fagan
* @date 3/4/2025
*/

#include "VulkanRenderingContext.h"

#include <vulkan/vulkan.h>

namespace Graphics {

    VulkanRenderingContext::VulkanRenderingContext(GLFWwindow* window) : m_Window(window) {}


    void VulkanRenderingContext::Init()
    {

    }


    void VulkanRenderingContext::Shutdown()
    {

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

