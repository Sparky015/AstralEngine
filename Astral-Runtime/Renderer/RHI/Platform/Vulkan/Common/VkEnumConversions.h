/**
* @file VkEnumConversions.h
* @author Andrew Fagan
* @date 6/11/2025
* @brief Includes conversion functions for ImageFormat to VkFormat and ImageLayout to VkImageLayout
*/

#pragma once

#include "Debug/Utilities/Error.h"
#include "Renderer/RHI/Common/ImageFormats.h"
#include "Renderer/RHI/Common/ImageLayouts.h"

#include <vulkan/vulkan_core.h>

namespace Astral {

    VkFormat AstralImageFormatToVkFormat(ImageFormat imageFormat);
    VkImageLayout AstralImageLayoutToVkImageLayout(ImageLayout imageLayout);

}