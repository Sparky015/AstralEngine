/**
* @file VulkanRendererCommands.cpp
* @author Andrew Fagan
* @date 3/4/25
*/

#include "VulkanRendererCommands.h"

#include <vulkan/vulkan.h>
#ifdef ASTRAL_VULKAN_AVAILABLE
#endif

#include "Debug/ImGui/ImGuiDependencies/imgui_impl_vulkan.h"

namespace Astral {

    void VulkanRendererCommands::Clear(CommandBufferHandle commandBufferHandle, RenderTargetHandle renderTargetHandle)
    {
        VkClearColorValue clearColor = {{1.0f, 0.0f, 0.0f, 1.0f}};

        VkImageSubresourceRange subresourceRange = {
            .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
            .baseMipLevel = 0,
            .levelCount = 1,
            .baseArrayLayer = 0,
            .layerCount = 1,
        };

        VkCommandBuffer commandBuffer = (VkCommandBuffer)commandBufferHandle->GetNativeHandle();
        VkImage image = (VkImage)renderTargetHandle->GetNativeHandle();

        vkCmdClearColorImage(commandBuffer, image, VK_IMAGE_LAYOUT_GENERAL, &clearColor, 1, &subresourceRange);
    }


    void VulkanRendererCommands::SetClearColor(float r, float g, float b, float a)
    {

    }


    void VulkanRendererCommands::DrawElementsIndexed(CommandBufferHandle commandBufferHandle, IndexBufferHandle indexBufferHandle)
    {
        VkCommandBuffer commandBuffer = (VkCommandBuffer)commandBufferHandle->GetNativeHandle();
        // vkCmdDraw(commandBuffer, 3, 1, 0, 0); // For just vertex buffer
        vkCmdDrawIndexed(commandBuffer, indexBufferHandle->GetCount(), 1, 0, 0, 0);
        m_NumberOfDrawCalls++;
    }
\

    void VulkanRendererCommands::PushConstants(CommandBufferHandle commandBufferHandle, PipelineStateObjectHandle pipelineStateObjectHandle,
                                                    void* data, uint32 sizeInBytes)
    {
        VkCommandBuffer commandBuffer = (VkCommandBuffer)commandBufferHandle->GetNativeHandle();
        VkPipelineLayout pipelineLayout = (VkPipelineLayout)pipelineStateObjectHandle->GetPipelineLayout();
        vkCmdPushConstants(commandBuffer, pipelineLayout, VK_SHADER_STAGE_ALL, 0, sizeInBytes, data);
    }


    void VulkanRendererCommands::SetBlending(bool enable)
    {

    }


    void VulkanRendererCommands::CallImGuiDraws(CommandBufferHandle commandBufferHandle)
    {
        VkCommandBuffer commandBuffer = (VkCommandBuffer)commandBufferHandle->GetNativeHandle();
        ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), commandBuffer);
    }

}
