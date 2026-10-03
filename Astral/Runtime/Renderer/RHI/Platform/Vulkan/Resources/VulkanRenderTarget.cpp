/**
* @file VulkanRenderTarget.cpp
* @author Andrew Fagan
* @date 5/15/2025
*/

#include "VulkanRenderTarget.h"

namespace Astral {

    VulkanRenderTarget::VulkanRenderTarget(const VulkanRenderTargetDesc& desc) :
        m_Image(desc.Image),
        m_ImageView(desc.ImageView),
        m_ImageFormat(desc.Format),
        m_ImageLayout(desc.Layout),
        m_ImageIndex(desc.ImageIndex),
        m_RenderCompleteSemaphore(desc.RenderCompleteSemaphore),
        m_ImageAvailableSemaphore(desc.ImageAvailableSemaphore),
        m_ImageWidth(desc.ImageWidth),
        m_ImageHeight(desc.ImageHeight)
    {
        m_RenderTargetTexture = CreateGraphicsRef<VulkanTexture>(desc.Device, m_Image, m_ImageView, m_ImageLayout, m_ImageFormat, m_ImageWidth, m_ImageHeight);
    }


    VulkanRenderTarget::~VulkanRenderTarget()
    {

    }

}
