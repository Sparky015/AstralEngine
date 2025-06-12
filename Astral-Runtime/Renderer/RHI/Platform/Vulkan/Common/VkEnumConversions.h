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
#include "Renderer/RHI/Common/AccessFlags.h"
#include "Renderer/RHI/Resources/PipelineStateObject.h"

#include <vulkan/vulkan_core.h>

namespace Astral {

    VkFormat ConvertImageFormatToVkFormat(ImageFormat imageFormat);
    VkImageLayout ConvertImageLayoutToVkImageLayout(ImageLayout imageLayout);
    VkAccessFlags ConvertAccessFlagsToVkAccessFlags(AccessFlags accessFlags);
    VkPipelineStageFlags ConvertPipelineStageToVkPipelineStageFlags(PipelineStageFlags pipelineStageFlags);

    ImageFormat ConvertVkFormatToImageFormat(VkFormat imageFormat);

}
