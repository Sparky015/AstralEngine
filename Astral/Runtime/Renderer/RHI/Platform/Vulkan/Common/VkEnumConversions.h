/**
* @file VkEnumConversions.h
* @author Andrew Fagan
* @date 6/11/2025
*/

#pragma once


#include "Core/Utilities/Error.h"
#include "Renderer/RHI/Common/ImageFormats.h"
#include "Renderer/RHI/Common/ImageLayouts.h"
#include "Renderer/RHI/Common/AccessFlags.h"
#include "Renderer/RHI/Common/PipelineStageFlags.h"
#include <Renderer/RHI/Common/ImageUsageFlags.h>
#include "Renderer/RHI/Common/BufferUsageFlags.h"
#include "Renderer/RHI/Common/DependencyFlags.h"
#include "Renderer/RHI/Common/ImageSubResourceRange.h"
#include "Renderer/RHI/Common/MemoryTypes.h"
#include "Renderer/RHI/Common/PipelineRasterizerState.h"
#include "Renderer/RHI/Common/SamplerSettings.h"
#include "Renderer/RHI/Common/ShaderStage.h"
#include "Renderer/RHI/Resources/Renderpass.h"

#include <vulkan/vulkan_core.h>


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
    VkCullModeFlags ConvertCullModeToVkCullModeFlags(CullMode cullMode);
    VkMemoryPropertyFlags ConvertMemoryPropertyFlagsToVkMemoryPropertyFlags(GPUMemoryType memoryType);
    VkBufferUsageFlags ConvertBufferUsageFlagsToVkBufferUsageFlags(BufferUsageFlags bufferUsageFlags);
    VkShaderStageFlags GetVkShaderStageFromShaderStage(ShaderStage shaderStage);
    VkSampleCountFlagBits ConvertSampleCountToVkSampleCountBit(SampleCount sampleCount);
    VkAttachmentLoadOp ConvertToVkLoadOp(AttachmentLoadOp loadOp);
    VkAttachmentStoreOp ConvertToVkStoreOp(AttachmentStoreOp storeOp);

    ImageLayout ConvertVkImageLayoutToImageLayout(VkImageLayout imageLayout);
    ImageFormat ConvertVkFormatToImageFormat(VkFormat imageFormat);
}
