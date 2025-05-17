/**
* @file VulkanFramebuffer.cpp
* @author Andrew Fagan
* @date 5/16/25
*/

#include "VulkanFramebuffer.h"

#include "Debug/Utilities/Asserts.h"

namespace Astral {

    VulkanFramebuffer::VulkanFramebuffer(const VulkanFramebufferDesc& desc) :
        m_Device(desc.Device),
        m_RenderPass(desc.RenderPass),
        m_WindowWidth(desc.WindowWidth),
        m_WindowHeight(desc.WindowHeight),
        m_ImageView(desc.ImageView)
    {
        CreateFramebuffer();
    }

    VulkanFramebuffer::~VulkanFramebuffer()
    {
        DestroyFramebuffer();
    }


    void VulkanFramebuffer::CreateFramebuffer()
    {
        VkFramebufferCreateInfo framebufferInfo = {
            .sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
            .renderPass = m_RenderPass,
            .attachmentCount = 1,
            .pAttachments = &m_ImageView,
            .width = m_WindowWidth,
            .height = m_WindowHeight,
            .layers = 1,
        };

        VkResult result = vkCreateFramebuffer(m_Device, &framebufferInfo, nullptr, &m_Framebuffer);
        ASSERT(result == VK_SUCCESS, "Failed to create framebuffer!");
    }


    void VulkanFramebuffer::DestroyFramebuffer()
    {
        vkDestroyFramebuffer(m_Device, m_Framebuffer, nullptr);
    }

}
