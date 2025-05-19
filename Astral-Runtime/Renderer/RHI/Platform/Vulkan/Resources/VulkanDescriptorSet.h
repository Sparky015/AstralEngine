/**
* @file VulkanDescriptorSet.h
* @author Andrew Fagan
* @date 5/18/2025
*/


#pragma once

#include "VulkanDescriptorSet.h"
#include "Renderer/RHI/Resources/DescriptorSet.h"

#include <vulkan/vulkan_core.h>

#include "VulkanVertexBuffer.h"

namespace Astral {

    struct VulkanDescriptorSetDesc
    {
        VkDevice Device;
        VertexBufferHandle VertexBuffer;
    };

    class VulkanDescriptorSet : public DescriptorSet
    {
    public:
        VulkanDescriptorSet(const VulkanDescriptorSetDesc& desc);
        ~VulkanDescriptorSet() override;

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
        VertexBufferHandle m_VertexBuffer;

        VkDescriptorPool m_DescriptorPool;
        VkDescriptorSet m_DescriptorSet;
        VkDescriptorSetLayout m_DescriptorSetLayout;
    };

}
