/**
* @file VulkanCommandBuffer.cpp
* @author Andrew Fagan
* @date 5/13/25
*/

#include "VulkanCommandBuffer.h"

#include "Astral.h"
#include "Astral.h"
#include "Astral.h"
#include "Astral.h"
#include "Core/Engine.h"
#include "Debug/Utilities/Asserts.h"
#include "Debug/Utilities/Loggers.h"
#include "Renderer/RendererManager.h"
#include "Renderer/RHI/RendererAPI.h"
#include "Renderer/RHI/Platform/Vulkan/Common/VkEnumConversions.h"


namespace Astral {

    VulkanCommandBuffer::VulkanCommandBuffer(const VulkanCommandBufferDesc& desc) :
        m_Device(desc.Device),
        m_CommandPool(desc.CommandPool),
        m_CommandBuffer(VK_NULL_HANDLE)
    {
        m_IsValid = true;
        m_State = State::EMPTY;
        AllocateCommandBuffer();
    }

    VulkanCommandBuffer::~VulkanCommandBuffer()
    {
        FreeCommandBuffer();

        m_BoundPipeline = nullptr;
        m_BoundDescriptorSets.clear();
    }


    void VulkanCommandBuffer::BeginRecording()
    {
        m_State = State::RECORDING;

        VkCommandBufferBeginInfo commandBufferBeginInfo = {
            .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
            .pNext = nullptr,
            .flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT,
            .pInheritanceInfo = nullptr
        };

        VkResult result = vkBeginCommandBuffer(m_CommandBuffer, &commandBufferBeginInfo);
        ASSERT(result == VK_SUCCESS, "Command buffer failed to begin recording!");
    }


    void VulkanCommandBuffer::EndRecording()
    {
        m_State = State::RECORDED;
        vkEndCommandBuffer(m_CommandBuffer);
    }


    void VulkanCommandBuffer::Reset()
    {
        m_State = State::EMPTY;
        vkResetCommandBuffer(m_CommandBuffer, 0);

        m_BoundPipeline = nullptr;
        m_BoundDescriptorSets.clear();
        m_PipelineDynamicStateViewportAndScissor = UVec2{-1, -1};
    }


    void VulkanCommandBuffer::BindPipeline(const PipelineStateHandle& pipeline)
    {
        if (!pipeline)
        {
            AE_WARN("Tried to bind pipeline handle that was null!");
            return;
        }

        if (m_BoundPipeline && pipeline->GetHandleHandle() == m_BoundPipeline->GetHandleHandle()) { return; } // Prevent redundant pipeline bind call
        if (m_BoundPipeline && pipeline->GetDescriptorSetLayout() != m_BoundPipeline->GetDescriptorSetLayout()) { m_BoundDescriptorSets.clear(); }

        VkPipeline vkPipeline = (VkPipeline)pipeline->GetHandleHandle();
        VkPipelineBindPoint bindPoint = pipeline->GetPipelineType() == PipelineType::GRAPHICS ? VK_PIPELINE_BIND_POINT_GRAPHICS : VK_PIPELINE_BIND_POINT_COMPUTE;
        vkCmdBindPipeline(m_CommandBuffer, bindPoint, vkPipeline);
        m_BoundPipeline = pipeline;
        m_PipelineDynamicStateViewportAndScissor = UVec2{-1, -1};
        m_BoundIndexBuffer = nullptr;
        m_BoundVertexBuffer = nullptr;
    }


    void VulkanCommandBuffer::BindDescriptorSet(const DescriptorSetHandle& descriptorSet, uint32 binding)
    {
        if (!descriptorSet)
        {
            AE_WARN("Tried to bind descriptor set handle that was null!");
            return;
        }

        if (!m_BoundPipeline)
        {
            AE_WARN("Tried to bind descriptor set handle with no bound pipeline!");
            return;
        }

        if (m_BoundDescriptorSets.size() <= binding) { m_BoundDescriptorSets.resize(binding + 1); }
        if (m_BoundDescriptorSets[binding] && m_BoundDescriptorSets[binding]->GetNativeHandle() == descriptorSet->GetNativeHandle()) { return; } // Prevent redundant descriptor set bind call

        VkDescriptorSet vkDescriptorSet = (VkDescriptorSet)descriptorSet->GetNativeHandle();
        VkPipelineLayout vkPipelineLayout = (VkPipelineLayout)m_BoundPipeline->GetPipelineLayout();
        VkPipelineBindPoint bindPoint = m_BoundPipeline->GetPipelineType() == PipelineType::GRAPHICS ? VK_PIPELINE_BIND_POINT_GRAPHICS : VK_PIPELINE_BIND_POINT_COMPUTE;
        vkCmdBindDescriptorSets(m_CommandBuffer, bindPoint, vkPipelineLayout,
                    binding, 1, &vkDescriptorSet, 0, nullptr);

        m_BoundDescriptorSets[binding] = descriptorSet;
    }


    void VulkanCommandBuffer::BindVertexBuffer(const VertexBufferHandle& vertexBuffer)
    {
        if (!vertexBuffer)
        {
            AE_WARN("Tried to bind vertex buffer handle that was null!");
            return;
        }

        if (!m_BoundPipeline)
        {
            AE_WARN("Tried to bind descriptor set handle with no bound pipeline!");
            return;
        }

        if (m_BoundVertexBuffer && m_BoundVertexBuffer->GetNativeHandle() == vertexBuffer->GetNativeHandle()) { return; } // Prevent redundant pipeline bind call

        VkBuffer buffer = (VkBuffer)vertexBuffer->GetNativeHandle();
        VkDeviceSize offsets[] = {0};
        vkCmdBindVertexBuffers(m_CommandBuffer, 0, 1, &buffer, offsets);

        m_BoundVertexBuffer = vertexBuffer;
    }


    void VulkanCommandBuffer::BindIndexBuffer(const IndexBufferHandle& indexBuffer)
    {
        if (!indexBuffer)
        {
            AE_WARN("Tried to bind index buffer handle that was null!");
            return;
        }

        if (!m_BoundPipeline)
        {
            AE_WARN("Tried to bind descriptor set handle with no bound pipeline!");
            return;
        }

        if (m_BoundIndexBuffer && m_BoundIndexBuffer->GetNativeHandle() == indexBuffer->GetNativeHandle()) { return; } // Prevent redundant pipeline bind call

        VkBuffer buffer = (VkBuffer)indexBuffer->GetNativeHandle();
        vkCmdBindIndexBuffer(m_CommandBuffer, buffer, 0, VK_INDEX_TYPE_UINT32);

        m_BoundIndexBuffer = indexBuffer;
    }


    void VulkanCommandBuffer::SetViewportAndScissor(UVec2 dimensions)
    {
        if (!m_BoundPipeline)
        {
            AE_WARN("Tried to set viewport and scissor with no bound pipeline!");
            return;
        }

        if (m_PipelineDynamicStateViewportAndScissor == dimensions) { return; } // Prevent redundant command call

        VkViewport viewport = {
            .x = 0.0f,
            .y = 0.0f,
            .width = (float)dimensions.x,
            .height = (float)dimensions.y,
            .minDepth = 0.0f,
            .maxDepth = 1.0f,
        };
        vkCmdSetViewport(m_CommandBuffer, 0, 1, &viewport);

        VkRect2D rect = {
            .offset = {0, 0},
            .extent = {dimensions.x, dimensions.y},
        };
        vkCmdSetScissor(m_CommandBuffer, 0, 1, &rect);

        m_PipelineDynamicStateViewportAndScissor= dimensions;
    }


    void VulkanCommandBuffer::BeginRenderPass(const RenderPassHandle& renderPassHandle, const FramebufferHandle& frameBufferHandle)
    {
        m_ActiveRenderPass = renderPassHandle;

        VkRenderPass renderPass = (VkRenderPass)renderPassHandle->GetNativeHandle();
        VkFramebuffer framebuffer = (VkFramebuffer)frameBufferHandle->GetNativeHandle();
        UVec2 extent = frameBufferHandle->GetExtent();
        const std::vector<Vec4>& clearColors = renderPassHandle->GetClearColors();

        VkRenderPassBeginInfo renderPassBeginInfo = {
            .sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
            .pNext = nullptr,
            .renderPass = renderPass,
            .framebuffer = framebuffer,
            .renderArea = {
                .offset = {0,0},
                .extent = {extent.x, extent.y}
            },
            .clearValueCount = (uint32)clearColors.size(),
            .pClearValues = (VkClearValue*)clearColors.data(), // VkClearValue and Vec4 have same data layout
        };

        vkCmdBeginRenderPass(m_CommandBuffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

        m_ActiveRenderPass->BeginRenderPass(frameBufferHandle);
    }


    void VulkanCommandBuffer::NextSubpass()
    {
        vkCmdNextSubpass(m_CommandBuffer, VK_SUBPASS_CONTENTS_INLINE);
        m_ActiveRenderPass->NextSubpass();
    }


    void VulkanCommandBuffer::EndRenderPass()
    {
        vkCmdEndRenderPass(m_CommandBuffer);
        m_ActiveRenderPass->EndRenderPass();
    }


    void VulkanCommandBuffer::DrawElementsIndexed(const IndexBufferHandle& indexBufferHandle)
    {
        vkCmdDrawIndexed(m_CommandBuffer, indexBufferHandle->GetCount(), 1, 0, 0, 0);

        RendererDebugStats& inProgressDebugStats = RendererAPI::GetInProgressRendererDebugStats();
        inProgressDebugStats.NumberOfDrawCalls++;
        inProgressDebugStats.NumberOfTriangles += (indexBufferHandle->GetCount() / 3);
    }


    void VulkanCommandBuffer::DrawElementsInstanced(const IndexBufferHandle& indexBufferHandle, uint32 numberOfInstances)
    {
        vkCmdDrawIndexed(m_CommandBuffer, indexBufferHandle->GetCount(), numberOfInstances, 0, 0, 0);

        RendererDebugStats& inProgressDebugStats = RendererAPI::GetInProgressRendererDebugStats();
        inProgressDebugStats.NumberOfDrawCalls++;
        inProgressDebugStats.NumberOfTriangles += (indexBufferHandle->GetCount() / 3);
    }


    void VulkanCommandBuffer::Dispatch(uint32 groupCountX, uint32 groupCountY, uint32 groupCountZ)
    {
        vkCmdDispatch(m_CommandBuffer, groupCountX, groupCountY, groupCountZ);
    }


    void VulkanCommandBuffer::PushConstants(void* data, uint32 sizeInBytes)
    {
        if (!m_BoundPipeline)
        {
            AE_WARN("Tried to push constants with no bound pipeline!");
            return;
        }

        VkPipelineLayout pipelineLayout = (VkPipelineLayout)m_BoundPipeline->GetPipelineLayout();
        vkCmdPushConstants(m_CommandBuffer, pipelineLayout, VK_SHADER_STAGE_ALL, 0, sizeInBytes, data);
    }


    void VulkanCommandBuffer::SetPipelineBarrier(const PipelineBarrier& pipelineBarrier)
    {
        std::vector<VkMemoryBarrier> memoryBarriers;
        memoryBarriers.reserve(pipelineBarrier.MemoryBarriers.size());

        for (const MemoryBarrier& memoryBarrier : pipelineBarrier.MemoryBarriers)
        {
            VkMemoryBarrier vkMemoryBarrier = {
                .sType          =  VK_STRUCTURE_TYPE_MEMORY_BARRIER,
                .pNext          =  nullptr,
                .srcAccessMask  =  ConvertAccessFlagsToVkAccessFlags(memoryBarrier.SourceAccessMask),
                .dstAccessMask  =  ConvertAccessFlagsToVkAccessFlags(memoryBarrier.DestinationAccessMask)
            };

            memoryBarriers.push_back(vkMemoryBarrier);
        }


        std::vector<VkBufferMemoryBarrier> bufferMemoryBarriers;
        bufferMemoryBarriers.reserve(pipelineBarrier.BufferMemoryBarriers.size());

        for (const BufferMemoryBarrier& bufferMemoryBarrier : pipelineBarrier.BufferMemoryBarriers)
        {
            VkBuffer buffer = (VkBuffer)bufferMemoryBarrier.Buffer->GetNativeHandle();

            VkBufferMemoryBarrier vkBufferMemoryBarrier = {
                .sType                 =   VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER,
                .pNext                 =   nullptr,
                .srcAccessMask         =   ConvertAccessFlagsToVkAccessFlags(bufferMemoryBarrier.SourceAccessMask),
                .dstAccessMask         =   ConvertAccessFlagsToVkAccessFlags(bufferMemoryBarrier.DestinationAccessMask),
                .srcQueueFamilyIndex   =   bufferMemoryBarrier.SourceQueueFamilyIndex,
                .dstQueueFamilyIndex   =   bufferMemoryBarrier.DestinationQueueFamilyIndex,
                .buffer                =   buffer,
                .offset                =   bufferMemoryBarrier.Offset,
                .size                  =   bufferMemoryBarrier.Size
            };

            if (vkBufferMemoryBarrier.srcQueueFamilyIndex == QueueFamilyIgnored) { vkBufferMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED; }
            if (vkBufferMemoryBarrier.dstQueueFamilyIndex == QueueFamilyIgnored) { vkBufferMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED; }

            bufferMemoryBarriers.push_back(vkBufferMemoryBarrier);
        }


        std::vector<VkImageMemoryBarrier> imageMemoryBarriers;
        imageMemoryBarriers.reserve(pipelineBarrier.ImageMemoryBarriers.size());

        for (const ImageMemoryBarrier& imageMemoryBarrier : pipelineBarrier.ImageMemoryBarriers)
        {
            VkImage image = (VkImage)imageMemoryBarrier.Image->GetNativeImage();

            VkImageSubresourceRange vkImageSubresource = {
                .aspectMask      =   ConvertImageAspectFlagsToVkImageAspectFlags(imageMemoryBarrier.ImageSubresourceRange.AspectMask),
                .baseMipLevel    =   imageMemoryBarrier.ImageSubresourceRange.BaseMipLevel,
                .levelCount      =   imageMemoryBarrier.ImageSubresourceRange.LevelCount,
                .baseArrayLayer  =   imageMemoryBarrier.ImageSubresourceRange.BaseArrayLayer,
                .layerCount      =   imageMemoryBarrier.ImageSubresourceRange.LayerCount,
            };


            VkImageMemoryBarrier vkImageMemoryBarrier = {
                .sType                =  VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
                .pNext                =  nullptr,
                .srcAccessMask        =  ConvertAccessFlagsToVkAccessFlags(imageMemoryBarrier.SourceAccessMask),
                .dstAccessMask        =  ConvertAccessFlagsToVkAccessFlags(imageMemoryBarrier.DestinationAccessMask),
                .oldLayout            =  ConvertImageLayoutToVkImageLayout(imageMemoryBarrier.OldLayout),
                .newLayout            =  ConvertImageLayoutToVkImageLayout(imageMemoryBarrier.NewLayout),
                .srcQueueFamilyIndex  =  imageMemoryBarrier.SourceQueueFamilyIndex,
                .dstQueueFamilyIndex  =  imageMemoryBarrier.DestinationQueueFamilyIndex,
                .image                =  image,
                .subresourceRange     =  vkImageSubresource
            };

            if (vkImageMemoryBarrier.srcQueueFamilyIndex == QueueFamilyIgnored) { vkImageMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED; }
            if (vkImageMemoryBarrier.dstQueueFamilyIndex == QueueFamilyIgnored) { vkImageMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED; }

            imageMemoryBarriers.push_back(vkImageMemoryBarrier);
        }

        vkCmdPipelineBarrier(m_CommandBuffer,
            ConvertPipelineStageToVkPipelineStageFlags(pipelineBarrier.SourceStageMask),
            ConvertPipelineStageToVkPipelineStageFlags(pipelineBarrier.DestinationStageMask),
            ConvertDependencyFlagsToVkDependencyFlags(pipelineBarrier.DependencyFlags),
            (uint32)memoryBarriers.size(),
            memoryBarriers.data(),
            (uint32)bufferMemoryBarriers.size(),
            bufferMemoryBarriers.data(),
            (uint32)imageMemoryBarriers.size(),
            imageMemoryBarriers.data()
        );

        // Update texture layouts
        for (const ImageMemoryBarrier& imageMemoryBarrier : pipelineBarrier.ImageMemoryBarriers)
        {
            ImageLayout newLayout = imageMemoryBarrier.NewLayout;
            imageMemoryBarrier.Image->UpdateLayout(newLayout);
        }
    }


    void VulkanCommandBuffer::BeginLabel(const ::std::string_view& label, Vec4 color)
    {
        thread_local VkInstance instance = (VkInstance)Engine::Get().GetRendererManager().GetContext().GetInstanceHandle();
        thread_local PFN_vkCmdBeginDebugUtilsLabelEXT vkCmdBeginDebugUtilsLabelEXT = (PFN_vkCmdBeginDebugUtilsLabelEXT)vkGetInstanceProcAddr(instance, "vkCmdBeginDebugUtilsLabelEXT");

        VkDebugUtilsLabelEXT debugUtilsLabel = {
            .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_LABEL_EXT,
            .pNext = nullptr,
            .pLabelName = label.data(),
            .color = {color.r, color.g, color.b, color.a},
        };

        vkCmdBeginDebugUtilsLabelEXT(m_CommandBuffer, &debugUtilsLabel);
    }


    void VulkanCommandBuffer::EndLabel()
    {
        thread_local VkInstance instance = (VkInstance)Engine::Get().GetRendererManager().GetContext().GetInstanceHandle();
        thread_local PFN_vkCmdEndDebugUtilsLabelEXT vkCmdEndDebugUtilsLabelEXT = (PFN_vkCmdEndDebugUtilsLabelEXT)vkGetInstanceProcAddr(instance, "vkCmdEndDebugUtilsLabelEXT");

        vkCmdEndDebugUtilsLabelEXT(m_CommandBuffer);
    }


    void VulkanCommandBuffer::InsertMarker(const ::std::string_view& label, Vec4 color)
    {
        thread_local VkInstance instance = (VkInstance)Engine::Get().GetRendererManager().GetContext().GetInstanceHandle();
        thread_local PFN_vkCmdInsertDebugUtilsLabelEXT vkCmdInsertDebugUtilsLabelEXT = (PFN_vkCmdInsertDebugUtilsLabelEXT)vkGetInstanceProcAddr(instance, "vkCmdInsertDebugUtilsLabelEXT");

        VkDebugUtilsLabelEXT debugUtilsLabel = {
            .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_LABEL_EXT,
            .pNext = nullptr,
            .pLabelName = label.data(),
            .color = {color.r, color.g, color.b, color.a},
        };

        vkCmdInsertDebugUtilsLabelEXT(m_CommandBuffer, &debugUtilsLabel);
    }


    void VulkanCommandBuffer::AllocateCommandBuffer()
    {
        VkCommandBufferAllocateInfo commandBufferAllocate = {
            .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
            .pNext = nullptr,
            .commandPool = m_CommandPool,
            .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
            .commandBufferCount = 1,
        };

        VkResult result = vkAllocateCommandBuffers(m_Device, &commandBufferAllocate, &m_CommandBuffer);
        ASSERT(result == VK_SUCCESS, "Vulkan command buffer(s) failed to allocate!");
    }


    void VulkanCommandBuffer::FreeCommandBuffer()
    {
        vkFreeCommandBuffers(m_Device, m_CommandPool, 1, &m_CommandBuffer);
    }

}
