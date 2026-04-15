/**
* @file VulkanComputePipelineState.h
* @author Andrew Fagan
* @date 8/31/25
*/

#pragma once

#include "Renderer/RHI/Resources/PipelineState.h"
#include "Renderer/RHI/Resources/Shader.h"
#include "Renderer/RHI/Resources/VertexBufferLayout.h"

#include <vulkan/vulkan_core.h>

namespace Astral {

    struct VulkanComputePipelineStateDesc
    {
        VkDevice Device;
        ShaderHandle ComputeShader;
        std::vector<DescriptorSetHandle> DescriptorSets;
    };

    class VulkanComputePipelineState : public PipelineState
    {
    public:
        explicit VulkanComputePipelineState(const VulkanComputePipelineStateDesc& desc);
        ~VulkanComputePipelineState() override;

        PipelineType GetPipelineType() override { return PipelineType::COMPUTE; }
        const std::vector<DescriptorSetLayout>& GetDescriptorSetLayout() const override { return m_DescriptorSetLayout; }
        void* GetPipelineLayout() override {return m_PipelineLayout; }
        void* GetHandleHandle() override { return m_Pipeline; }

    private:

        void SetComputeShaderStage();

        void CreateComputePipelineLayout();
        void DestroyPipelineLayout();

        void CreateComputePipelineStateObject();
        void DestroyPipelineState();


        VkDevice m_Device;
        VulkanComputePipelineStateDesc m_ComputeDescription;

        struct PipelineCreateInfos
        {
            VkPipelineShaderStageCreateInfo ShaderState;
            VkPipelineLayoutCreateInfo PipelineLayout;
        };

        PipelineCreateInfos m_PipelineCreateInfos;

        VkPipeline m_Pipeline;
        VkPipelineLayout m_PipelineLayout;
        std::vector<DescriptorSetLayout> m_DescriptorSetLayout;
        VkPushConstantRange m_PushConstantRange;

        UVec2 m_ViewportDimensions;
    };

}
