/**
* @file VulkanRendererCommands.cpp
* @author Andrew Fagan
* @date 3/4/25
*/

#include "VulkanRendererCommands.h"


#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>
#ifdef ASTRAL_VULKAN_AVAILABLE
#endif
#include "Core/Engine.h"
#include "Debug/ImGui/ImGuiDependencies/imgui_impl_vulkan.h"
#include "Renderer/RendererManager.h"

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
        m_NumberOfDrawCallsThisFrame++;
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


    void VulkanRendererCommands::BeginLabel(CommandBufferHandle commandBufferHandle, std::string_view label, Vec4 color)
    {
        thread_local VkInstance instance = (VkInstance)Engine::Get().GetRendererManager().GetContext().GetInstanceHandle();
        thread_local PFN_vkCmdBeginDebugUtilsLabelEXT vkCmdBeginDebugUtilsLabelEXT = (PFN_vkCmdBeginDebugUtilsLabelEXT)vkGetInstanceProcAddr(instance, "vkCmdBeginDebugUtilsLabelEXT");
        VkCommandBuffer commandBuffer = (VkCommandBuffer)commandBufferHandle->GetNativeHandle();

        VkDebugUtilsLabelEXT debugUtilsLabel = {
            .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_LABEL_EXT,
            .pNext = nullptr,
            .pLabelName = label.data(),
            .color = {color.r, color.g, color.b, color.a},
        };

        vkCmdBeginDebugUtilsLabelEXT(commandBuffer, &debugUtilsLabel);
    }


    void VulkanRendererCommands::EndLabel(CommandBufferHandle commandBufferHandle)
    {
        thread_local VkInstance instance = (VkInstance)Engine::Get().GetRendererManager().GetContext().GetInstanceHandle();
        thread_local PFN_vkCmdEndDebugUtilsLabelEXT vkCmdEndDebugUtilsLabelEXT = (PFN_vkCmdEndDebugUtilsLabelEXT)vkGetInstanceProcAddr(instance, "vkCmdEndDebugUtilsLabelEXT");

        VkCommandBuffer commandBuffer = (VkCommandBuffer)commandBufferHandle->GetNativeHandle();
        vkCmdEndDebugUtilsLabelEXT(commandBuffer);
    }


    void VulkanRendererCommands::CallImGuiDraws(CommandBufferHandle commandBufferHandle)
    {
        VkCommandBuffer commandBuffer = (VkCommandBuffer)commandBufferHandle->GetNativeHandle();
        ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), commandBuffer);
    }

}
