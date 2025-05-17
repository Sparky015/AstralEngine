/**
* @file VulkanShader.h
* @author Andrew Fagan
* @date 5/16/2025
*/


#pragma once

#include "Renderer/RHI/Resources/Shader.h"

#include <vulkan/vulkan_core.h>

namespace Astral {

    struct VulkanShaderDesc
    {
        VkDevice Device;
        const ShaderSource& ShaderSource;
    };

    class VulkanShader : public Shader
    {
    public:
        VulkanShader(const VulkanShaderDesc& desc);
        ~VulkanShader() override;

    private:

        void CompileShader(const ShaderSource& shaderSource);

        VkDevice m_Device;
        VkShaderModule m_ShaderModule;
    };

}
