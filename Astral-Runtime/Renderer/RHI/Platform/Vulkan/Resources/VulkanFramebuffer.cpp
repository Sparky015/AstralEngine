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
        m_FramebufferWidth(0),
        m_FramebufferHeight(0),
        m_Framebuffer(VK_NULL_HANDLE)
    {

    }

    VulkanFramebuffer::~VulkanFramebuffer()
    {
        VulkanFramebuffer::InvalidateFramebuffer();
    }


    void VulkanFramebuffer::BeginBuildingFramebuffer(uint32 framebufferWidth, uint32 framebufferHeight)
    {
        InvalidateFramebuffer();
        m_FramebufferWidth = framebufferWidth;
        m_FramebufferHeight = framebufferHeight;
    }


    void VulkanFramebuffer::AttachRenderTarget(RenderTargetHandle renderTargetHandle)
    {
        VkImageView imageView = (VkImageView)renderTargetHandle->GetImageView();
        m_ImageViews.push_back(imageView);
        m_Textures.push_back(renderTargetHandle->GetAsTexture());
    }


    void VulkanFramebuffer::AttachTexture(TextureHandle textureHandle)
    {
        VkImageView imageView = (VkImageView)textureHandle->GetNativeImageView();
        m_ImageViews.push_back(imageView);
        m_Textures.push_back(textureHandle);
    }


    void VulkanFramebuffer::EndBuildingFramebuffer()
    {
        CreateFramebuffer();
    }


    TextureHandle VulkanFramebuffer::GetAttachment(uint32 attachmentIndex)
    {
        ASSERT(attachmentIndex < m_Textures.size(), "Given attachment index is out of range of the textures array")
        return m_Textures[attachmentIndex];
    }


    void VulkanFramebuffer::InvalidateFramebuffer()
    {
        if (m_Framebuffer != VK_NULL_HANDLE)
        {
            DestroyFramebuffer();
        }
        m_ImageViews.clear();
        m_Textures.clear();
    }


    void VulkanFramebuffer::CreateFramebuffer()
    {
        VkFramebufferCreateInfo framebufferInfo = {
            .sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
            .renderPass = m_RenderPass,
            .attachmentCount = (uint32)m_ImageViews.size(),
            .pAttachments = m_ImageViews.data(),
            .width = m_FramebufferWidth,
            .height = m_FramebufferHeight,
            .layers = 1,
        };

        VkResult result = vkCreateFramebuffer(m_Device, &framebufferInfo, nullptr, &m_Framebuffer);
        ASSERT(result == VK_SUCCESS, "Failed to create framebuffer!");
    }


    void VulkanFramebuffer::DestroyFramebuffer()
    {
        vkDestroyFramebuffer(m_Device, m_Framebuffer, nullptr);
        m_Framebuffer = VK_NULL_HANDLE;
    }

}
