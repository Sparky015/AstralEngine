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
#include "Renderer/RHI/Common/PipelineStageFlags.h"
#include <Renderer/RHI/Common/ImageUsageFlags.h>

#include <vulkan/vulkan_core.h>

#include "Renderer/RHI/Common/DependencyFlags.h"
#include "Renderer/RHI/Common/ImageSubResourceRange.h"
#include "Renderer/RHI/Common/SamplerSettings.h"
#include "Renderer/RHI/Resources/Renderpass.h"

namespace Astral {

    VkFormat ConvertImageFormatToVkFormat(ImageFormat imageFormat);
    VkImageLayout ConvertImageLayoutToVkImageLayout(ImageLayout imageLayout);
    VkAccessFlags ConvertAccessFlagsToVkAccessFlags(AccessFlags accessFlags);
    VkPipelineStageFlags ConvertPipelineStageToVkPipelineStageFlags(PipelineStageFlags pipelineStageFlags);
    VkImageUsageFlags ConvertImageUsageFlagsToVkImageUsageFlags(ImageUsageFlags imageUsageFlags);
    VkImageAspectFlags ConvertImageAspectFlagsToVkImageAspectFlags(ImageAspectFlags imageAspectFlags);
    VkDependencyFlags ConvertDependencyFlagsToVkDependencyFlags(DependencyFlags dependencyFlags);
    VkFilter ConvertSamplerFilterToVkFilter(SamplerFilter samplerFilter);
    VkSamplerAddressMode ConvertSamplerAddressModeToVkSamplerAddressMode(SamplerAddressMode samplerAddressMode);

    VkSampleCountFlagBits ConvertSampleCountToVkSampleCountBit(SampleCount sampleCount);

    ImageLayout ConvertVkImageLayoutToImageLayout(VkImageLayout imageLayout);
    ImageFormat ConvertVkFormatToImageFormat(VkFormat imageFormat);
}
