/**
* @file VulkanDescriptorSet.h
* @author Andrew Fagan
* @date 5/18/2025
*/


#pragma once

#include <queue>

#include "VulkanDescriptorSet.h"
#include "Renderer/RHI/Resources/DescriptorSet.h"

#include <vulkan/vulkan_core.h>

#include "VulkanVertexBuffer.h"
#include "Renderer/RHI/Resources/Sampler.h"

namespace Astral {

    struct VulkanDescriptorSetDesc
    {
        VkDevice Device;
    };

    class VulkanDescriptorSet : public DescriptorSet
    {
    public:
        VulkanDescriptorSet(const VulkanDescriptorSetDesc& desc);
        ~VulkanDescriptorSet() override;

        void BeginBuildingSet() override;
        void AddDescriptorStorageBuffer(BufferHandle bufferHandle, ShaderStage bindStage) override;
        void AddDescriptorUniformBuffer(BufferHandle bufferHandle, ShaderStage bindStage) override;
        void AddDescriptorImageSampler(SamplerHandle samplerHandle) override;
        void EndBuildingSet() override;

        void* GetLayout() override { return m_DescriptorSetLayout; }
        void* GetNativeHandle() override { return m_DescriptorSet; }

    private:

        // void CreateDescriptorSet();
        // void DestroyDescriptorSet();

        void CreateDescriptorPool();
        void DestroyDescriptorPool();

        void CreateDescriptorSetLayout();
        void DestroyDescriptorSetLayout();

        void AllocateDescriptorSets();
        void DestroyDescriptorSets();

        void UpdateDescriptorSets();

        VkDevice m_Device;

        std::vector<VkDescriptorSetLayoutBinding> m_DescriptorSetLayoutBindings;
        uint32 m_NumberOfBindings;

        std::queue<BufferHandle, std::deque <BufferHandle>> m_Buffers;
        std::queue<SamplerHandle, std::deque <SamplerHandle>> m_Samplers;

        VkDescriptorPool m_DescriptorPool;
        VkDescriptorSet m_DescriptorSet;
        VkDescriptorSetLayout m_DescriptorSetLayout;
    };

}
