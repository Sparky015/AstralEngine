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


    void VulkanRendererCommands::DrawElements(CommandBufferHandle commandBufferHandle)
    {
        VkCommandBuffer commandBuffer = (VkCommandBuffer)commandBufferHandle->GetNativeHandle();
        vkCmdDraw(commandBuffer, 3, 1, 0, 0);

        m_NumberOfDrawCalls++;
    }


    void VulkanRendererCommands::SetBlending(bool enable)
    {

    }

    struct Vertex {
        Vertex(const glm::vec3& p, const glm::vec2& t)
        {
            Pos = p;
            Tex = t;
        }

        glm::vec3 Pos;
        glm::vec2 Tex;
    };


    void VulkanRendererCommands::TestInit()
    {
        RenderingContext& context = g_RendererManager.GetContext();
        Device& device = context.GetDevice();
        RenderTargetHandle renderTarget = device.GetSwapchain().AcquireNextImage();
        m_CommandBuffer = device.AllocateCommandBuffer();
        m_RenderPass = device.CreateRenderPass(renderTarget);
        m_Framebuffer = device.CreateFramebuffer(m_RenderPass, renderTarget);

        ShaderSource vertexSource = ShaderSource( "SecondTriangle.vert");
        ShaderSource fragmentSource = ShaderSource( "SecondTriangle.frag");



        std::vector<Vertex> Vertices = {
            Vertex({-1.0f, -1.0f, 0.0f}, {0.0f, 0.0f}),     // top left
            Vertex({1.0f, -1.0f, 0.0f}, {0.0f, 1.0f} ),      // top right
            Vertex({0.0f, 1.0f, 0.0f}, {1.0f, 1.0f})       // bottom middle
        };

        m_VertexBuffer = device.CreateVertexBuffer(Vertices.data(), sizeof(Vertices[0]) * Vertices.size() );
        m_VertexShader = device.CreateShader(vertexSource);
        m_FragmentShader = device.CreateShader(fragmentSource);
        m_DescriptorSet = device.CreateDescriptorSet(m_VertexBuffer);

        m_PipelineStateObject = device.CreatePipelineStateObject(m_RenderPass, m_VertexShader, m_FragmentShader, m_DescriptorSet);

        m_CommandBuffer->BeginRecording();
        m_RenderPass->BeginRenderPass(m_CommandBuffer, m_Framebuffer);
        m_PipelineStateObject->Bind(m_CommandBuffer);
        m_PipelineStateObject->BindDescriptorSet(m_CommandBuffer, m_DescriptorSet);
        DrawElements(m_CommandBuffer);
        m_RenderPass->EndRenderPass(m_CommandBuffer);
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
