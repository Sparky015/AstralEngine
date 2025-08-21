/**
* @file VulkanTexture.cpp
* @author Andrew Fagan
* @date 5/22/2025
*/

#include "VulkanTexture.h"

#include "Debug/Utilities/Asserts.h"
#include "Debug/Utilities/Error.h"
#include "VulkanBuffer.h"
#include "Renderer/RHI/Platform/Vulkan/Common/VkEnumConversions.h"
#include "Renderer/RHI/Common/ImageFormats.h"

#include "Debug/ImGui/ImGuiDependencies/imgui_impl_vulkan.h"

namespace Astral {

    VulkanTexture::VulkanTexture(const VulkanTextureDesc& desc) :
		m_DeviceManager(desc.VulkanDevice),
        m_Device(desc.Device),
		m_PhysicalDeviceMemoryProperties(desc.PhysicalDeviceMemoryProperties),
        m_ImageWidth(desc.ImageWidth),
        m_ImageHeight(desc.ImageHeight),
		m_Format(ConvertImageFormatToVkFormat(desc.ImageFormat)),
        m_Image(),
        m_ImageView(),
        m_Sampler(),
        m_NumLayers(desc.NumLayers),
		m_TextureType(desc.TextureType),
		m_IsSwapchainOwned(false)
    {
        CreateTexture(desc.ImageUsageFlags);
    	AllocateTextureMemory();
    	CreateImageView(desc.ImageUsageFlags);
    	CreateImageSampler();

    	if (desc.ImageData)
    	{
    		// Image has data
    		m_CurrentLayout = ConvertImageLayoutToVkImageLayout(desc.ImageLayout);
    		TransitionImageLayout(VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
    		UploadDataToTexture(desc.ImageData);
    		TransitionImageLayout(VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, m_CurrentLayout);
    	}
    	else
    	{
    		// Image has no data
    		m_CurrentLayout = ConvertImageLayoutToVkImageLayout(desc.ImageLayout);
    		if (desc.ImageLayout != ImageLayout::UNDEFINED)
    		{
    			TransitionImageLayout(VK_IMAGE_LAYOUT_UNDEFINED, m_CurrentLayout);
    		}
    	}
    }


    VulkanTexture::VulkanTexture(VkDevice device, VkImage image, VkImageView imageView, ImageLayout layout, ImageFormat format, uint32 width, uint32 height) :
		m_DeviceManager(nullptr),
		m_Device(device),
		m_PhysicalDeviceMemoryProperties(),
		m_ImageWidth(width),
		m_ImageHeight(height),
		m_Format(ConvertImageFormatToVkFormat(format)),
		m_Image(image),
		m_ImageView(imageView),
		m_CurrentLayout(ConvertImageLayoutToVkImageLayout(layout)),
		m_Sampler(),
        m_NumLayers(1),
		m_TextureType(TextureType::IMAGE_2D),
		m_IsSwapchainOwned(true)
    {
    	CreateImageSampler();

    	if (m_CurrentLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL ||
					(m_Format == VK_FORMAT_D16_UNORM) ||
					(m_Format == VK_FORMAT_X8_D24_UNORM_PACK32) ||
					(m_Format == VK_FORMAT_D32_SFLOAT) ||
					(m_Format == VK_FORMAT_S8_UINT) ||
					(m_Format == VK_FORMAT_D16_UNORM_S8_UINT) ||
					(m_Format == VK_FORMAT_D24_UNORM_S8_UINT))
    	{
    		m_ImageAspect = IMAGE_ASPECT_DEPTH_BIT | IMAGE_ASPECT_STENCIL_BIT;
    	}
    	else
    	{
    		m_ImageAspect = IMAGE_ASPECT_COLOR_BIT;
    	}
    }


    VulkanTexture::~VulkanTexture()
    {
    	DestroyImageSampler();

    	if (!m_IsSwapchainOwned)
    	{
    		DestroyImageView();
    		DestroyTexture();
    		FreeTextureMemory();
    	}
    }


    void VulkanTexture::UpdateLayout(ImageLayout imageLayout)
    {
    	m_CurrentLayout = ConvertImageLayoutToVkImageLayout(imageLayout);
    }


    void VulkanTexture::CreateTexture(ImageUsageFlags imageUsageFlags)
    {
    	VkImageUsageFlags userUsageFlag = ConvertImageUsageFlagsToVkImageUsageFlags(imageUsageFlags);
        VkImageUsageFlags imageUsage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT | userUsageFlag; // TODO: Remove the predefined flags

    	VkImageCreateFlags createFlags = (m_TextureType == TextureType::CUBEMAP) ? VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT : 0u;

		VkImageType imageType;
    	switch (m_TextureType)
    	{
    		case TextureType::IMAGE_2D: imageType = VK_IMAGE_TYPE_2D; break;
    		case TextureType::IMAGE_3D: imageType = VK_IMAGE_TYPE_3D; break;
    		case TextureType::CUBEMAP: imageType = VK_IMAGE_TYPE_2D; break;
    		default: imageType = VK_IMAGE_TYPE_2D;
    	}

    	if (m_TextureType == TextureType::IMAGE_3D)
    	{
    		m_ImageDepth = m_ImageWidth;
    	}
    	else
    	{
    		m_ImageDepth = 1;
    	}



        VkImageCreateInfo imageCreateInfo = {
            .sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
            .pNext = nullptr,
            .flags = createFlags,
            .imageType = imageType,
            .format = m_Format,
            .extent = {.width = m_ImageWidth, .height = m_ImageHeight, .depth = m_ImageDepth},
            .mipLevels = 1,
            .arrayLayers = m_NumLayers,
            .samples = VK_SAMPLE_COUNT_1_BIT,
            .tiling = VK_IMAGE_TILING_OPTIMAL,
            .usage = imageUsage,
            .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
            .queueFamilyIndexCount = 0,
            .pQueueFamilyIndices = nullptr,
            .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED
        };


        VkResult result = vkCreateImage(m_Device, &imageCreateInfo, nullptr, &m_Image);
        ASSERT(result == VK_SUCCESS, "Failed to create image!");
    }


    void VulkanTexture::DestroyTexture()
    {
    	vkDestroyImage(m_Device, m_Image, nullptr);
    }


    void VulkanTexture::AllocateTextureMemory()
    {
    	VkMemoryRequirements memoryRequirements = {};
    	vkGetImageMemoryRequirements(m_Device, m_Image, &memoryRequirements);

    	uint32 memoryTypeIndex = GetMemoryTypeIndex(memoryRequirements.memoryTypeBits);

    	VkMemoryAllocateInfo memoryAllocateInfo = {
    		.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
			.pNext = nullptr,
			.allocationSize = memoryRequirements.size,
			.memoryTypeIndex = memoryTypeIndex,
		};

    	VkResult result = vkAllocateMemory(m_Device, &memoryAllocateInfo, nullptr, &m_ImageMemory);
    	ASSERT(result == VK_SUCCESS, "Failed to allocate memory!");

    	result = vkBindImageMemory(m_Device, m_Image, m_ImageMemory, 0);
    	ASSERT(result == VK_SUCCESS, "Failed to bind image!");
    }


    void VulkanTexture::FreeTextureMemory()
    {
    	vkFreeMemory(m_Device, m_ImageMemory, nullptr);
    }


    void VulkanTexture::CreateImageView(ImageUsageFlags imageUsageFlags)
    {
    	VkImageAspectFlags aspectFlags{};

    	if (imageUsageFlags & IMAGE_USAGE_COLOR_ATTACHMENT_BIT || imageUsageFlags & ImageUsageFlagBits::IMAGE_USAGE_SAMPLED_BIT)
    	{
    		aspectFlags |= VK_IMAGE_ASPECT_COLOR_BIT;
    	}
    	else if (imageUsageFlags & ImageUsageFlagBits::IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT)
    	{
    		aspectFlags |= VK_IMAGE_ASPECT_DEPTH_BIT;
    	}

    	VkImageViewType viewType;
    	switch (m_TextureType)
    	{
    		case TextureType::IMAGE_2D: viewType = VK_IMAGE_VIEW_TYPE_2D; break;
    		case TextureType::IMAGE_3D: viewType = VK_IMAGE_VIEW_TYPE_3D; break;
    		case TextureType::CUBEMAP: viewType = VK_IMAGE_VIEW_TYPE_CUBE; break;
		    default: viewType = VK_IMAGE_VIEW_TYPE_2D;
    	}

    	VkImageViewCreateInfo imageViewCreateInfo = {
			.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
    		.pNext = nullptr,
    		.flags = 0,
    		.image = m_Image,
    		.viewType = viewType,
    		.format = m_Format,
    		.components = {
				.r = VK_COMPONENT_SWIZZLE_R,
				.g = VK_COMPONENT_SWIZZLE_G,
				.b = VK_COMPONENT_SWIZZLE_B,
				.a = VK_COMPONENT_SWIZZLE_A,
    		},
    		.subresourceRange = {
				.aspectMask = aspectFlags,
				.baseMipLevel = 0,
				.levelCount = 1,
				.baseArrayLayer = 0,
				.layerCount = m_NumLayers,
    		}
    	};

    	VkResult result = vkCreateImageView(m_Device, &imageViewCreateInfo, nullptr, &m_ImageView);
    	ASSERT(result == VK_SUCCESS, "Failed to create image view!");
    }


    void VulkanTexture::DestroyImageView()
    {
    	vkDestroyImageView(m_Device, m_ImageView, nullptr);
    }


    void VulkanTexture::UploadDataToTexture(uint8* data)
    {
    	uint32 imageSize = 0;
    	if (IsCompressed(ConvertVkFormatToImageFormat(m_Format)))
    	{
    		uint32 bytesPerBlock = GetBytesPerTexel(ConvertVkFormatToImageFormat(m_Format));
    		Vec2 blockExtent = GetCompressedFormatBlockExtent(ConvertVkFormatToImageFormat(m_Format));
    		uint32 numBlocksX = (m_ImageWidth + blockExtent.x - 1) / blockExtent.x;
    		uint32 numBlocksY = (m_ImageHeight + blockExtent.y - 1) / blockExtent.y;
    		uint32 layerSize = numBlocksX * numBlocksY * bytesPerBlock;
    		imageSize = m_NumLayers * layerSize;
    	}
    	else
    	{
    		uint32 bytesPerPixel = GetBytesPerTexel(ConvertVkFormatToImageFormat(m_Format));
    		uint32 layerSize = m_ImageWidth * m_ImageHeight * m_ImageDepth * bytesPerPixel;
    		imageSize = m_NumLayers * layerSize;
    	}


        VulkanBufferDesc bufferDesc = {
            .Device = m_Device,
            .Usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
            .Size = imageSize,
            .DeviceMemoryProperties = m_PhysicalDeviceMemoryProperties,
            .RequestedMemoryPropertyFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
        };

        VulkanBuffer stagingBuffer = VulkanBuffer{bufferDesc};
        stagingBuffer.CopyDataToBuffer(data, imageSize);

		CopyFromStagingBuffer(stagingBuffer);
    }


    void VulkanTexture::TransitionImageLayout(VkImageLayout currentLayout, VkImageLayout newLayout)
    {
        CommandBufferHandle commandBufferHandle = m_DeviceManager->AllocateCommandBuffer();
        VkCommandBuffer commandBuffer = (VkCommandBuffer)commandBufferHandle->GetNativeHandle();

        VkImageMemoryBarrier barrier = {
            .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
            .pNext = nullptr,
            .srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT,
            .dstAccessMask = VK_ACCESS_MEMORY_READ_BIT,
            .oldLayout = currentLayout,
            .newLayout = newLayout,
            .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
            .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
            .image = m_Image,
            .subresourceRange = {
                .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                .baseMipLevel = 0,
                .levelCount = 1,
                .baseArrayLayer = 0,
                .layerCount = m_NumLayers
            }
        };

    	// TODO: Abstract this better than a ton of if statements
        VkPipelineStageFlags sourceStage = VK_PIPELINE_STAGE_NONE;
		VkPipelineStageFlags destinationStage = VK_PIPELINE_STAGE_NONE;

		if (newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL ||
					(m_Format == VK_FORMAT_D16_UNORM) ||
					(m_Format == VK_FORMAT_X8_D24_UNORM_PACK32) ||
					(m_Format == VK_FORMAT_D32_SFLOAT) ||
					(m_Format == VK_FORMAT_S8_UINT) ||
					(m_Format == VK_FORMAT_D16_UNORM_S8_UINT) ||
					(m_Format == VK_FORMAT_D24_UNORM_S8_UINT) ||
					(m_Format == VK_FORMAT_D32_SFLOAT_S8_UINT))
		{
			barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT;
			m_ImageAspect = IMAGE_ASPECT_DEPTH_BIT | IMAGE_ASPECT_STENCIL_BIT;
		}
		else
		{
			barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			m_ImageAspect = IMAGE_ASPECT_COLOR_BIT;
		}

		if (currentLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
		{
			barrier.srcAccessMask = 0;
			barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

			sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
			destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
		}
		else if (currentLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_GENERAL)
		{
			barrier.srcAccessMask = 0;
			barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

			sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
			destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
		}

		if (currentLayout == VK_IMAGE_LAYOUT_UNDEFINED &&
			newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
		{
			barrier.srcAccessMask = 0;
			barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

			sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
			destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
		} /* Convert back from read-only to updateable */
		else if (currentLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
		{
			barrier.srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
			barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

			sourceStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
			destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
		} /* Convert from updateable texture to shader read-only */
		else if (currentLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL &&
			     newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
		{
			barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

			sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
			destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
		} /* Convert depth texture from undefined state to depth-stencil buffer */


		else if (currentLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL)
		{
			barrier.srcAccessMask = 0;
			barrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

			sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
			destinationStage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
		} /* Wait for render pass to complete */


		else if (currentLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
		{
			barrier.srcAccessMask = 0; // VK_ACCESS_SHADER_READ_BIT;
			barrier.dstAccessMask = 0;
			/*
					sourceStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
			///		destinationStage = VK_PIPELINE_STAGE_ALL_GRAPHICS_BIT;
					destinationStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
			*/
			sourceStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
			destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
		} /* Convert back from read-only to color attachment */
		else if (currentLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL)
		{
			barrier.srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
			barrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

			sourceStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
			destinationStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		} /* Convert from updateable texture to shader read-only */
		else if (currentLayout == VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
		{
			barrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
			barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

			sourceStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
			destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
		} /* Convert back from read-only to depth attachment */
		else if (currentLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL)
		{
			barrier.srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
			barrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

			sourceStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
			destinationStage = VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
		} /* Convert from updateable depth texture to shader read-only */
		else if (currentLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
		{
			barrier.srcAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
			barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

			sourceStage = VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
			destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
		}
		else if (currentLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL)
		{
			barrier.srcAccessMask = 0;
			barrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

			sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
			destinationStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		}
		else if (currentLayout == VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_PRESENT_SRC_KHR)
		{
			barrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
			barrier.dstAccessMask = 0;

			sourceStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
			destinationStage = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
		}
    	else if (currentLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
    	{
    		barrier.srcAccessMask = 0;
    		barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

    		sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
    		destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    	}
		else
		{
			ASTRAL_ERROR("Unknown barrier transition!");
		}

        commandBufferHandle->BeginRecording();

        vkCmdPipelineBarrier(commandBuffer, sourceStage, destinationStage, 0, 0, nullptr, 0, nullptr, 1, &barrier);

        commandBufferHandle->EndRecording();
        CommandQueueHandle queueHandle = m_DeviceManager->GetPrimaryCommandQueue();
        queueHandle->SubmitSync(commandBufferHandle);
        queueHandle->WaitIdle();
    }


    void VulkanTexture::CopyFromStagingBuffer(VulkanBuffer& stagingBuffer)
    {
        VkBuffer buffer = (VkBuffer)stagingBuffer.GetNativeHandle();
        CommandBufferHandle commandBufferHandle = m_DeviceManager->AllocateCommandBuffer();
        VkCommandBuffer commandBuffer = (VkCommandBuffer)commandBufferHandle->GetNativeHandle();

    	VkBufferImageCopy bufferImageCopy = {
    		.bufferOffset = 0,
    		.bufferRowLength = 0,
    		.bufferImageHeight = 0,
    		.imageSubresource = {
				.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
    			.mipLevel = 0,
    			.baseArrayLayer = 0,
    			.layerCount = m_NumLayers
    		},
    		.imageOffset = {.x = 0, .y = 0, .z = 0},
    		.imageExtent = {.width = m_ImageWidth, .height = m_ImageHeight, .depth = m_ImageDepth},
    	};

        commandBufferHandle->BeginRecording();

		vkCmdCopyBufferToImage(commandBuffer, buffer, m_Image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &bufferImageCopy);

        commandBufferHandle->EndRecording();
        CommandQueueHandle queueHandle = m_DeviceManager->GetPrimaryCommandQueue();
        queueHandle->SubmitSync(commandBufferHandle);
        queueHandle->WaitIdle();
    }


    void VulkanTexture::CreateImageSampler()
    {
    	VkSamplerCreateInfo samplerCreateInfo = {
			.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
    		.pNext = nullptr,
    		.flags = 0,
    		.magFilter = VK_FILTER_LINEAR,
    		.minFilter = VK_FILTER_LINEAR,
    		.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR,
    		.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT,
    		.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT,
    		.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT,
    		.mipLodBias = 0.0f,
    		.anisotropyEnable = VK_FALSE,
    		.compareEnable = VK_FALSE,
    		.compareOp = VK_COMPARE_OP_ALWAYS,
    		.minLod = 0.0f,
    		.maxLod = 0.0f,
    		.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK,
    		.unnormalizedCoordinates = VK_FALSE,
    	};

    	VkResult result = vkCreateSampler(m_Device, &samplerCreateInfo, nullptr, &m_Sampler);
    	ASSERT(result == VK_SUCCESS, "Failed to create sampler!");
    }


    void VulkanTexture::DestroyImageSampler()
    {
    	if (m_Sampler) { vkDestroySampler(m_Device, m_Sampler, nullptr); }
    }


    uint32 VulkanTexture::GetMemoryTypeIndex(uint32 memoryTypeBitsMask)
    {
        VkMemoryPropertyFlagBits requestedMemoryPropertyFlags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;

        for (uint32 i = 0; i < m_PhysicalDeviceMemoryProperties.memoryTypeCount; i++)
        {
            const VkMemoryType& memoryType = m_PhysicalDeviceMemoryProperties.memoryTypes[i];
            uint32 currentBitmask = (1 << i);
            bool isCurrentMemoryTypeSupported = (memoryTypeBitsMask & currentBitmask);
            bool hasRequiredMemoryProperties = ((memoryType.propertyFlags & requestedMemoryPropertyFlags) == requestedMemoryPropertyFlags);
            if (isCurrentMemoryTypeSupported && hasRequiredMemoryProperties) { return i; }
        }

        ASTRAL_ERROR("Failed to find a suitable memory type for Buffer object!");
    }

}
