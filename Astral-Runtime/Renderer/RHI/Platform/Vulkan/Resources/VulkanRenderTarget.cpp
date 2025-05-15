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
        m_ImageIndex(desc.ImageIndex)
    {

    }


    VulkanRenderTarget::~VulkanRenderTarget()
    {

    }

}
