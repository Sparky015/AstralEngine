/**
* @file VulkanRendererAPI.cpp
* @author Andrew Fagan
* @date 3/4/25
*/

#include "VulkanRendererCommands.h"

namespace Astral {

    void VulkanRendererCommands::Clear()
    {

    }


    void VulkanRendererCommands::SetClearColor(float r, float g, float b, float a)
    {

    }


    void VulkanRendererCommands::DrawElements(Astral::VertexArrayObject* vertexArrayObject)
    {
        m_NumberOfDrawCalls++;
    }


    void VulkanRendererCommands::SetBlending(bool enable)
    {

    }

}
