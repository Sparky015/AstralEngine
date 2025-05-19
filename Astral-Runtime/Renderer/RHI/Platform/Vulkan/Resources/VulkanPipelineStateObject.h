/**
* @file VulkanPipelineStateObject.h
* @author Andrew Fagan
* @date 5/17/2025
*/


#pragma once

#include "Renderer/RHI/Resources/PipelineStateObject.h"

#include <vulkan/vulkan_core.h>

#include "Renderer/RHI/Resources/Shader.h"

namespace Astral {

    struct VulkanPipelineStateObjectDesc
    {
        VkDevice Device;
        VkRenderPass RenderPass;
        ShaderHandle VertexShader;
        ShaderHandle FragmentShader;
        int32 WindowWidth;
        int32 WindowHeight;
        DescriptorSetHandle DescriptorSet;
    };

    class VulkanPipelineStateObject : public PipelineStateObject
    {
    public:
        explicit VulkanPipelineStateObject(const VulkanPipelineStateObjectDesc& desc);
        ~VulkanPipelineStateObject() override;

        void Bind(CommandBufferHandle commandBufferHandle) override;
        void BindDescriptorSet(CommandBufferHandle commandBufferHandle, DescriptorSetHandle descriptorSetHandle) override;

    private:

        void CreatePipelineStateObject();
        void DestroyPipelineStateObject();

        VkDevice m_Device;
        VkRenderPass m_RenderPass;
        ShaderHandle m_VertexShader;
        ShaderHandle m_FragmentShader;
        uint32 m_WindowWidth;
        uint32 m_WindowHeight;
        DescriptorSetHandle m_DescriptorSet;

        VkPipeline m_Pipeline;
        VkPipelineLayout m_PipelineLayout;
    };

}
