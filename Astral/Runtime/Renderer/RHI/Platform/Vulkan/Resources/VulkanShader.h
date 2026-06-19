/**
* @file VulkanShader.h
* @author Andrew Fagan
* @date 5/16/2025
*/

#pragma once

#include "Renderer/RHI/Resources/Shader.h"

#include <vulkan/vulkan_core.h>

namespace Astral {

    /**
    * @brief Information to create a Vulkan shader from
    */
    struct VulkanShaderDesc
    {
        VkDevice Device;
        const ShaderSource& ShaderSource;
    };

    /**
     * @brief Defines a Vulkan RHI shader module object
     */
    class VulkanShader : public Shader
    {
    public:
        VulkanShader(const VulkanShaderDesc& desc);
        ~VulkanShader() override;

        /**
         * @brief Gets the native shader object handle
         * @return The native shader object handle (MTL::Function*)
         */
        void* GetNativeHandle() override { return m_ShaderModule; }

    private:

        /**
         * @brief Compiles the given shader source into SPIR-V IR
         * @param shaderSource The GLSL shader source
         */
        void CompileShader(const ShaderSource& shaderSource);

        /**
         * @brief Creates the Vulkan shader module from the given SPIR-V IR
         * @param SPIRV_Code The compiled SPIR-V of the shader
         */
        void CreateShaderModule(std::vector<uint32>& SPIRV_Code);

        /**
         * @brief Destroys the Vulkan shader module
         */
        void DestroyShaderModule();

        VkDevice m_Device;
        VkShaderModule m_ShaderModule;
    };

}
