/**
* @file VulkanShader.cpp
* @author Andrew Fagan
* @date 5/16/2025
*/

#include "VulkanShader.h"

#include "Debug/Utilities/Asserts.h"
#include "Debug/Utilities/Error.h"

#include "glslang/Include/glslang_c_interface.h"
#include "glslang/Public/resource_limits_c.h"

namespace Astral {

    VulkanShader::VulkanShader(const VulkanShaderDesc& desc) :
        m_Device(desc.Device)
    {
        CompileShader(desc.ShaderSource);
    }


    VulkanShader::~VulkanShader()
    {

    }


    void VulkanShader::CompileShader(const ShaderSource& shaderSource)
    {
        // Compiling to SPIR-V

        std::vector<uint32> SPIRV_Code;

        glslang_stage_t shaderStage;
        switch (shaderSource.GetShaderType())
        {
            case VERTEX_SHADER: shaderStage = GLSLANG_STAGE_VERTEX; break;
            case FRAGMENT_SHADER: shaderStage = GLSLANG_STAGE_FRAGMENT; break;
            case COMPUTE_SHADER: shaderStage = GLSLANG_STAGE_COMPUTE; break;
            case GEOMETRY_SHADER: shaderStage = GLSLANG_STAGE_GEOMETRY; break;
            case TESSELLATION_CONTROL_SHADER: shaderStage = GLSLANG_STAGE_TESSCONTROL; break;
            case TESSELLATION_EVALUATION_SHADER: shaderStage = GLSLANG_STAGE_TESSEVALUATION; break;
            case NONE: ASTRAL_ERROR("Invalid shader type!"); break;
            default: ASTRAL_ERROR("Invalid shader type!"); break;
        }

        glslang_initialize_process();

        glslang_input_t input = {
            .language = GLSLANG_SOURCE_GLSL,
            .stage = shaderStage,
            .client = GLSLANG_CLIENT_VULKAN,
            .client_version = GLSLANG_TARGET_VULKAN_1_0,
            .target_language = GLSLANG_TARGET_SPV,
            .target_language_version = GLSLANG_TARGET_SPV_1_0,
            .code = shaderSource.GetShaderCode().c_str(),
            .default_version = 100,
            .default_profile = GLSLANG_NO_PROFILE,
            .force_default_version_and_profile = false,
            .forward_compatible = false,
            .messages = GLSLANG_MSG_DEFAULT_BIT,
            .resource = glslang_default_resource(),
        };

        glslang_shader_t* shader = glslang_shader_create(&input);


        if (!glslang_shader_preprocess(shader, &input))
        {
            printf("GLSL preprocessing failed %s\n", shaderSource.GetFileName().data());
            printf("%s\n", glslang_shader_get_info_log(shader));
            printf("%s\n", glslang_shader_get_info_debug_log(shader));
            printf("%s\n", input.code);
            glslang_shader_delete(shader);
            ASTRAL_ERROR("GLSL preprocessing failed " << shaderSource.GetFileName().data());
        }

        if (!glslang_shader_parse(shader, &input))
        {
            printf("GLSL parsing failed %s\n", shaderSource.GetFileName().data());
            printf("%s\n", glslang_shader_get_info_log(shader));
            printf("%s\n", glslang_shader_get_info_debug_log(shader));
            printf("%s\n", glslang_shader_get_preprocessed_code(shader));
            glslang_shader_delete(shader);
            ASTRAL_ERROR("GLSL parsing failed " << shaderSource.GetFileName().data());
        }

        glslang_program_t* program = glslang_program_create();
        glslang_program_add_shader(program, shader);

        if (!glslang_program_link(program, GLSLANG_MSG_SPV_RULES_BIT | GLSLANG_MSG_VULKAN_RULES_BIT))
        {
            printf("GLSL linking failed %s\n", shaderSource.GetFileName().data());
            printf("%s\n", glslang_program_get_info_log(program));
            printf("%s\n", glslang_program_get_info_debug_log(program));
            glslang_program_delete(program);
            glslang_shader_delete(shader);
            ASTRAL_ERROR("GLSL linking failed " << shaderSource.GetFileName().data());
        }

        glslang_program_SPIRV_generate(program, shaderStage);

        size_t numberOfWords = glslang_program_SPIRV_get_size(program);
        SPIRV_Code.resize(numberOfWords);
        glslang_program_SPIRV_get(program, SPIRV_Code.data());

        const char* spirv_messages = glslang_program_SPIRV_get_messages(program);
        if (spirv_messages)
        {
            printf("(%s) %s\b",  shaderSource.GetFileName().data(), spirv_messages);
        }

        glslang_program_delete(program);
        glslang_shader_delete(shader);

        glslang_finalize_process();

        // SPIR-V Compilation Finished

        VkShaderModuleCreateInfo shaderCreateInfo = {
            .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
            .codeSize = SPIRV_Code.size() * sizeof(uint32),
            .pCode = SPIRV_Code.data(),
        };

        VkResult result = vkCreateShaderModule(m_Device, &shaderCreateInfo, nullptr, &m_ShaderModule);
        ASSERT(result == VK_SUCCESS, "Failed to create shader module!");
    }

}

