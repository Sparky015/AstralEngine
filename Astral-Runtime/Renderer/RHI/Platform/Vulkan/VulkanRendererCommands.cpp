/**
* @file VulkanRendererCommands.cpp
* @author Andrew Fagan
* @date 3/4/25
*/

#include "VulkanRendererCommands.h"

#include "Renderer/RendererManager.h"

#include <vulkan/vulkan.h>

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


    void VulkanRendererCommands::DrawElements(Astral::VertexArrayObject* vertexArrayObject)
    {
        m_NumberOfDrawCalls++;
    }


    void VulkanRendererCommands::SetBlending(bool enable)
    {

    }


    void VulkanRendererCommands::TestInit()
    {
        RenderingContext& context = g_RendererManager.GetContext();
        Device& device = context.GetDevice();
        RenderTargetHandle renderTarget = device.GetSwapchain().AcquireNextImage();
        m_CommandBuffer = device.AllocateCommandBuffer();

        m_CommandBuffer->BeginRecording();
        Clear(m_CommandBuffer, renderTarget);
        m_CommandBuffer->EndRecording();

        CommandQueueHandle commandQueue = context.GetDevice().GetCommandQueue();
        commandQueue->Submit(m_CommandBuffer, renderTarget);
        commandQueue->Present(renderTarget);
    }


    void VulkanRendererCommands::TestUpdate()
    {

    }


    void VulkanRendererCommands::TestShutdown()
    {

    }

}
