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
		m_IsSwapchainOwned(true)
    {
    	CreateImageSampler();
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
    	VkImageUsageFlags userUsageFlag = ConvertImageUsageFlagsToVkImageUsageFlags(imageUsageFlags); // TODO: Make the generic ImageUsageFlags contain actual flags
        VkImageUsageFlags imageUsage = VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT | userUsageFlag; // TODO: Remove the predefined flags

        VkImageCreateInfo imageCreateInfo = {
            .sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .imageType = VK_IMAGE_TYPE_2D,
            .format = m_Format,
            .extent = {.width = m_ImageWidth, .height = m_ImageHeight, .depth = 1},
            .mipLevels = 1,
            .arrayLayers = 1,
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

    	if (imageUsageFlags == ImageUsageFlags::COLOR_ATTACHMENT_BIT || imageUsageFlags == ImageUsageFlags::SAMPLED_BIT)
    	{
    		aspectFlags |= VK_IMAGE_ASPECT_COLOR_BIT;
    	}
    	else if (imageUsageFlags == ImageUsageFlags::DEPTH_STENCIL_ATTACHMENT_BIT)
    	{
    		aspectFlags |= VK_IMAGE_ASPECT_DEPTH_BIT;
    	}



    	VkImageViewCreateInfo imageViewCreateInfo = {
			.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
    		.pNext = nullptr,
    		.flags = 0,
    		.image = m_Image,
    		.viewType = VK_IMAGE_VIEW_TYPE_2D,
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
				.layerCount = 1,
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
        uint32 bytesPerPixel = GetBytesPerTexFormat(m_Format);
        uint32 layerSize = m_ImageWidth * m_ImageHeight * bytesPerPixel;
        uint32 layerCount = 1;
        uint32 imageSize = layerCount * layerSize;

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
                .layerCount = 1
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
					(m_Format == VK_FORMAT_D24_UNORM_S8_UINT))
		{
			barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT;
			m_ImageAspect = IMAGE_ASPECT_DEPTH_BIT | IMAGE_ASPECT_STENCIL_BIT;

			// if (HasStencilComponent(m_Format))
			// {
			// 	barrier.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
			// }
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
        CommandQueueHandle queueHandle = m_DeviceManager->GetCommandQueue();
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
    			.layerCount = 1
    		},
    		.imageOffset = {.x = 0, .y = 0, .z = 0},
    		.imageExtent = {.width = m_ImageWidth, .height = m_ImageHeight, .depth = 1},
    	};

        commandBufferHandle->BeginRecording();

		vkCmdCopyBufferToImage(commandBuffer, buffer, m_Image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &bufferImageCopy);

        commandBufferHandle->EndRecording();
        CommandQueueHandle queueHandle = m_DeviceManager->GetCommandQueue();
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


    uint32 VulkanTexture::GetBytesPerTexFormat(VkFormat m_Format)
    {
        switch (m_Format)
        {
            // Single channel 8-bit
            case VK_FORMAT_R8_UNORM: return 1;
            case VK_FORMAT_R8_SNORM: return 1;
            case VK_FORMAT_R8_UINT: return 1;
            case VK_FORMAT_R8_SINT: return 1;
            case VK_FORMAT_R8_SRGB: return 1;

            // Dual channel 8-bit
            case VK_FORMAT_R8G8_UNORM: return 2;
            case VK_FORMAT_R8G8_SNORM: return 2;
            case VK_FORMAT_R8G8_UINT: return 2;
            case VK_FORMAT_R8G8_SINT: return 2;
            case VK_FORMAT_R8G8_SRGB: return 2;

            // Triple channel 8-bit
            case VK_FORMAT_R8G8B8_UNORM: return 3;
            case VK_FORMAT_R8G8B8_SNORM: return 3;
            case VK_FORMAT_R8G8B8_UINT: return 3;
            case VK_FORMAT_R8G8B8_SINT: return 3;
            case VK_FORMAT_R8G8B8_SRGB: return 3;
            case VK_FORMAT_B8G8R8_UNORM: return 3;
            case VK_FORMAT_B8G8R8_SNORM: return 3;
            case VK_FORMAT_B8G8R8_UINT: return 3;
            case VK_FORMAT_B8G8R8_SINT: return 3;
            case VK_FORMAT_B8G8R8_SRGB: return 3;

            // Quad channel 8-bit
            case VK_FORMAT_R8G8B8A8_UNORM: return 4;
            case VK_FORMAT_R8G8B8A8_SNORM: return 4;
            case VK_FORMAT_R8G8B8A8_UINT: return 4;
            case VK_FORMAT_R8G8B8A8_SINT: return 4;
            case VK_FORMAT_R8G8B8A8_SRGB: return 4;
            case VK_FORMAT_B8G8R8A8_UNORM: return 4;
            case VK_FORMAT_B8G8R8A8_SNORM: return 4;
            case VK_FORMAT_B8G8R8A8_UINT: return 4;
            case VK_FORMAT_B8G8R8A8_SINT: return 4;
            case VK_FORMAT_B8G8R8A8_SRGB: return 4;

            // 16-bit single channel
            case VK_FORMAT_R16_UNORM: return 2;
            case VK_FORMAT_R16_SNORM: return 2;
            case VK_FORMAT_R16_UINT: return 2;
            case VK_FORMAT_R16_SINT: return 2;
            case VK_FORMAT_R16_SFLOAT: return 2;

            // 16-bit dual channel
            case VK_FORMAT_R16G16_UNORM: return 4;
            case VK_FORMAT_R16G16_SNORM: return 4;
            case VK_FORMAT_R16G16_UINT: return 4;
            case VK_FORMAT_R16G16_SINT: return 4;
            case VK_FORMAT_R16G16_SFLOAT: return 4;

            // 16-bit triple channel
            case VK_FORMAT_R16G16B16_UNORM: return 6;
            case VK_FORMAT_R16G16B16_SNORM: return 6;
            case VK_FORMAT_R16G16B16_UINT: return 6;
            case VK_FORMAT_R16G16B16_SINT: return 6;
            case VK_FORMAT_R16G16B16_SFLOAT: return 6;

            // 16-bit quad channel (HDR textures)
            case VK_FORMAT_R16G16B16A16_UNORM: return 8;
            case VK_FORMAT_R16G16B16A16_SNORM: return 8;
            case VK_FORMAT_R16G16B16A16_UINT: return 8;
            case VK_FORMAT_R16G16B16A16_SINT: return 8;
            case VK_FORMAT_R16G16B16A16_SFLOAT: return 8;

            // 32-bit single channel
            case VK_FORMAT_R32_UINT: return 4;
            case VK_FORMAT_R32_SINT: return 4;
            case VK_FORMAT_R32_SFLOAT: return 4;

            // 32-bit dual channel
            case VK_FORMAT_R32G32_UINT: return 8;
            case VK_FORMAT_R32G32_SINT: return 8;
            case VK_FORMAT_R32G32_SFLOAT: return 8;

            // 32-bit triple channel
            case VK_FORMAT_R32G32B32_UINT: return 12;
            case VK_FORMAT_R32G32B32_SINT: return 12;
            case VK_FORMAT_R32G32B32_SFLOAT: return 12;

            // 32-bit quad channel
            case VK_FORMAT_R32G32B32A32_UINT: return 16;
            case VK_FORMAT_R32G32B32A32_SINT: return 16;
            case VK_FORMAT_R32G32B32A32_SFLOAT: return 16;

            // Packed m_Formats
            case VK_FORMAT_R5G6B5_UNORM_PACK16: return 2;
            case VK_FORMAT_B5G6R5_UNORM_PACK16: return 2;
            case VK_FORMAT_R4G4B4A4_UNORM_PACK16: return 2;
            case VK_FORMAT_B4G4R4A4_UNORM_PACK16: return 2;
            case VK_FORMAT_R5G5B5A1_UNORM_PACK16: return 2;
            case VK_FORMAT_B5G5R5A1_UNORM_PACK16: return 2;
            case VK_FORMAT_A1R5G5B5_UNORM_PACK16: return 2;

            // Depth/Stencil m_Formats
            case VK_FORMAT_D16_UNORM: return 2;
            case VK_FORMAT_D32_SFLOAT: return 4;
            case VK_FORMAT_D24_UNORM_S8_UINT: return 4;
            case VK_FORMAT_D32_SFLOAT_S8_UINT: return 5;  // 4 for depth + 1 for stencil
            case VK_FORMAT_D16_UNORM_S8_UINT: return 3;   // 2 for depth + 1 for stencil
            case VK_FORMAT_S8_UINT: return 1;

        	case VK_FORMAT_BC1_RGB_UNORM_BLOCK:   return 8;   // BC1
        	case VK_FORMAT_BC1_RGB_SRGB_BLOCK:    return 8;   // BC1 (sRGB)
        	case VK_FORMAT_BC1_RGBA_UNORM_BLOCK:  return 8;   // BC1 (with alpha)
        	case VK_FORMAT_BC1_RGBA_SRGB_BLOCK:   return 8;   // BC1 (sRGB)
        	case VK_FORMAT_BC2_UNORM_BLOCK:       return 16;  // BC2
        	case VK_FORMAT_BC2_SRGB_BLOCK:        return 16;  // BC2 (sRGB)
        	case VK_FORMAT_BC3_UNORM_BLOCK:       return 16;  // BC3
        	case VK_FORMAT_BC3_SRGB_BLOCK:        return 16;  // BC3 (sRGB)
        	case VK_FORMAT_BC4_UNORM_BLOCK:       return 8;   // BC4
        	case VK_FORMAT_BC4_SNORM_BLOCK:       return 8;   // BC4 (signed)
        	case VK_FORMAT_BC5_UNORM_BLOCK:       return 16;  // BC5
        	case VK_FORMAT_BC5_SNORM_BLOCK:       return 16;  // BC5 (signed)
        	case VK_FORMAT_BC6H_UFLOAT_BLOCK:     return 16;  // BC6H (unsigned float)
        	case VK_FORMAT_BC6H_SFLOAT_BLOCK:     return 16;  // BC6H (signed float)
        	case VK_FORMAT_BC7_UNORM_BLOCK:       return 16;  // BC7
        	case VK_FORMAT_BC7_SRGB_BLOCK:        return 16;  // BC7 (sRGB)

            // Special Formats
            case VK_FORMAT_B10G11R11_UFLOAT_PACK32: return 4;  // HDR without alpha
            case VK_FORMAT_E5B9G9R9_UFLOAT_PACK32: return 4;   // Shared exponent HDR

            // Undefined
            case VK_FORMAT_UNDEFINED: ASTRAL_ERROR("Undefined m_Format cannot be applied to texture!");

            default: ASTRAL_ERROR("Unsupported m_Format given to texture!");  // Unknown m_Format
        }
    }

}
