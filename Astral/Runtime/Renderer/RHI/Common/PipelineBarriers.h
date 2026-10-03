/**
* @file PipelineBarriers.h
* @author Andrew Fagan
* @date 7/13/2025
*/


#pragma once

#include "AccessFlags.h"
#include "DependencyFlags.h"
#include "ImageLayouts.h"
#include "ImageSubResourceRange.h"
#include "PipelineStageFlags.h"
#include "Renderer/RHI/Resources/Buffer.h"
#include "Renderer/RHI/Resources/Texture.h"

namespace Astral {

    struct MemoryBarrier
    {
        AccessFlags SourceAccessMask;
        AccessFlags DestinationAccessMask;
    };

    struct ImageMemoryBarrier
    {
        AccessFlags SourceAccessMask;
        AccessFlags DestinationAccessMask;
        ImageLayout OldLayout;
        ImageLayout NewLayout;
        uint32 SourceQueueFamilyIndex;
        uint32 DestinationQueueFamilyIndex;
        TextureHandle Image;
        ImageSubResourceRange ImageSubresourceRange;
    };

    struct BufferMemoryBarrier
    {
        AccessFlags SourceAccessMask;
        AccessFlags DestinationAccessMask;
        uint32 SourceQueueFamilyIndex;
        uint32 DestinationQueueFamilyIndex;
        BufferHandle Buffer;
        uint32 Offset;
        uint32 Size;
    };

    struct PipelineBarrier
    {
        PipelineStageFlags SourceStageMask;
        PipelineStageFlags DestinationStageMask;
        DependencyFlags DependencyFlags;
        std::vector<MemoryBarrier> MemoryBarriers;
        std::vector<BufferMemoryBarrier> BufferMemoryBarriers;
        std::vector<ImageMemoryBarrier> ImageMemoryBarriers;
    };

}
