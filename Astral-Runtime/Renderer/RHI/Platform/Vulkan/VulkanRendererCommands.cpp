/**
* @file VulkanRendererCommands.cpp
* @author Andrew Fagan
* @date 3/4/25
*/

#include "VulkanRendererCommands.h"

#include "Renderer/RendererManager.h"

#include <vulkan/vulkan.h>
#include "stb_image.h"

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


    void VulkanRendererCommands::DrawElements(CommandBufferHandle commandBufferHandle, VertexBufferHandle vertexBufferHandle)
    {
        VkCommandBuffer commandBuffer = (VkCommandBuffer)commandBufferHandle->GetNativeHandle();
        // TODO: Query the vertex buffers on the amount of vertices when it gets implemented
        // vkCmdDraw(commandBuffer, 3, 1, 0, 0);
        vkCmdDrawIndexed(commandBuffer, 3, 1, 0, 0, 0);
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
        m_RenderPass = device.CreateRenderPass(renderTarget);
        m_Framebuffer = device.CreateFramebuffer(m_RenderPass, renderTarget);

        ShaderSource vertexSource = ShaderSource( "SecondTriangle.vert");
        ShaderSource fragmentSource = ShaderSource( "SecondTriangle.frag");


        std::vector<VertexBuffer::Vertex> Vertices = {
            {{0.0f, -0.5f}},
            {{0.5f, 0.5f}},
            {{-0.5f, 0.5f}}
        };

        std::vector<uint32_t> Indices = {
            0, 1, 2
        };


        m_VertexBuffer = device.CreateVertexBuffer(Vertices.data(), sizeof(Vertices[0]) * Vertices.size() );
        m_IndexBuffer = device.CreateIndexBuffer(Indices.data(), sizeof(Indices[0]) * Indices.size());
        m_VertexShader = device.CreateShader(vertexSource);
        m_FragmentShader = device.CreateShader(fragmentSource);
        m_DescriptorSet = device.CreateDescriptorSet();

        int m_Width;
        int m_Height;
        int m_BPP; // bits per pixel
        stbi_set_flip_vertically_on_load(1);
        unsigned char* data = stbi_load("C:\\Home\\Programming\\Coding Projects\\AstralEngine\\Astral-Runtime\\Renderer\\Shaders\\water.jpeg", &m_Width, &m_Height, &m_BPP, 4);
        m_Texture = device.CreateTexture(data, m_Width, m_Height);


        Vec4 color = {1.0f, 0.0f, 1.0f, 1.0f};
        BufferHandle storageBuffer = device.CreateStorageBuffer(&color, sizeof(color));

        float mult = .5f;
        BufferHandle uniformBuffer = device.CreateUniformBuffer(&mult, sizeof(mult));

        m_DescriptorSet->BeginBuildingSet();
        m_DescriptorSet->AddDescriptorStorageBuffer(storageBuffer, ShaderStage::FRAGMENT);
        m_DescriptorSet->AddDescriptorUniformBuffer(uniformBuffer, ShaderStage::FRAGMENT);
        m_DescriptorSet->AddDescriptorImageSampler(m_Texture, ShaderStage::FRAGMENT);
        m_DescriptorSet->EndBuildingSet();


        m_PipelineStateObject = device.CreatePipelineStateObject(m_RenderPass, m_VertexShader, m_FragmentShader, m_DescriptorSet);

        m_CommandBuffer->BeginRecording();
        m_RenderPass->BeginRenderPass(m_CommandBuffer, m_Framebuffer);
        m_PipelineStateObject->Bind(m_CommandBuffer);
        m_PipelineStateObject->BindDescriptorSet(m_CommandBuffer, m_DescriptorSet);
        m_VertexBuffer->Bind(m_CommandBuffer);
        m_IndexBuffer->Bind(m_CommandBuffer);
        DrawElements(m_CommandBuffer, m_VertexBuffer);
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
