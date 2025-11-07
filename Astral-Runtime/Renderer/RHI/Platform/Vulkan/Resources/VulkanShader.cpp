/**
* @file VulkanShader.cpp
* @author Andrew Fagan
* @date 5/16/2025
*/

#include "VulkanShader.h"

#include "Debug/Instrumentation/ScopeProfiler.h"
#include "Debug/Utilities/Asserts.h"
#include "Debug/Utilities/Error.h"

#ifdef ASTRAL_PLATFORM_WINDOWS
#include <atlbase.h>
#endif

#include "dxc/dxcapi.h"
#include "glslang/Include/glslang_c_interface.h"
#include "glslang/Public/resource_limits_c.h"
#include "glslang/Public/ShaderLang.h"


namespace Astral {

    VulkanShader::VulkanShader(const VulkanShaderDesc& desc) :
        m_Device(desc.Device)
    {
        CompileShader(desc.ShaderSource);
    }


    VulkanShader::~VulkanShader()
    {
        DestroyShaderModule();
    }


    void VulkanShader::CompileShader(const ShaderSource& shaderSource)
    {
        PROFILE_SCOPE("VulkanShader::CompileShader");

        if (shaderSource.GetShaderLanguage() == ShaderLanguage::GLSL)
        {
            CompileGLSLShader(shaderSource);
        }
        else
        {
            CompileHLSLShader(shaderSource);
        }
    }


    void VulkanShader::CompileGLSLShader(const ShaderSource& shaderSource)
    {
        // Compiling to SPIR-V
        std::vector<uint32> SPIRV_Code;

        glslang_stage_t shaderStage;
        switch (shaderSource.GetShaderType())
        {
            case ShaderType::VERTEX_SHADER: shaderStage = GLSLANG_STAGE_VERTEX; break;
            case ShaderType::FRAGMENT_SHADER: shaderStage = GLSLANG_STAGE_FRAGMENT; break;
            case ShaderType::COMPUTE_SHADER: shaderStage = GLSLANG_STAGE_COMPUTE; break;
            case ShaderType::GEOMETRY_SHADER: shaderStage = GLSLANG_STAGE_GEOMETRY; break;
            case ShaderType::TESSELLATION_CONTROL_SHADER: shaderStage = GLSLANG_STAGE_TESSCONTROL; break;
            case ShaderType::TESSELLATION_EVALUATION_SHADER: shaderStage = GLSLANG_STAGE_TESSEVALUATION; break;
            case ShaderType::NONE: AE_ERROR("Invalid shader type!"); break;
            default: AE_ERROR("Invalid shader type!"); break;
        }

        ShaderLanguage shaderLanguage = shaderSource.GetShaderLanguage();

        const glslang_input_t input = {
            .language = shaderLanguage == ShaderLanguage::GLSL ? GLSLANG_SOURCE_GLSL : GLSLANG_SOURCE_HLSL,
            .stage = shaderStage,
            .client = GLSLANG_CLIENT_VULKAN,
            .client_version = GLSLANG_TARGET_VULKAN_1_2,
            .target_language = GLSLANG_TARGET_SPV,
            .target_language_version = GLSLANG_TARGET_SPV_1_5,
            .code = shaderSource.GetShaderCode().c_str(),
            .default_version = 450,
            .default_profile = GLSLANG_NO_PROFILE,
            .force_default_version_and_profile = false,
            .forward_compatible = false,
            .messages = GLSLANG_MSG_DEFAULT_BIT,
            .resource = glslang_default_resource(),
            .callbacks = {},
        };

        glslang_shader_t* shader = glslang_shader_create(&input);


        if (!glslang_shader_preprocess(shader, &input))
        {
            printf("GLSL preprocessing failed %s\n", shaderSource.GetFileName().data());
            printf("%s\n", glslang_shader_get_info_log(shader));
            printf("%s\n", glslang_shader_get_info_debug_log(shader));
            printf("%s\n", input.code);
            glslang_shader_delete(shader);
            AE_ERROR("GLSL preprocessing failed " << shaderSource.GetFileName().data());
        }

        if (!glslang_shader_parse(shader, &input))
        {
            printf("GLSL parsing failed %s\n", shaderSource.GetFileName().data());
            printf("%s\n", glslang_shader_get_info_log(shader));
            printf("%s\n", glslang_shader_get_info_debug_log(shader));
            printf("%s\n", glslang_shader_get_preprocessed_code(shader));
            glslang_shader_delete(shader);
            AE_ERROR("GLSL parsing failed " << shaderSource.GetFileName().data());
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
            AE_ERROR("GLSL linking failed " << shaderSource.GetFileName().data());
        }

        glslang_spv_options_t spv_options = {
            .generate_debug_info = true,                    // Enable debug info
            .strip_debug_info = false,                      // Don't strip debug info
            .disable_optimizer = true,                      // Disable optimizer to preserve debug info
            .optimize_size = false,
            .disassemble = false,
            .validate = true,
            .emit_nonsemantic_shader_debug_info = true,     // Enable NonSemantic debug info
            .emit_nonsemantic_shader_debug_source = true,   // Include source in debug info
            .compile_only = false,
            .optimize_allow_expanded_id_bound = false
        };

        glslang_program_SPIRV_generate_with_options(program, shaderStage, &spv_options);

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

        // SPIR-V Compilation Finished

        CreateShaderModule(SPIRV_Code);
    }


    void VulkanShader::CompileHLSLShader(const ShaderSource& shaderSource)
    {
        CComPtr<IDxcUtils> pUtils;
        CComPtr<IDxcCompiler3> pCompiler;

        // Create utils instance
        HRESULT result = DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&pUtils));
        ASSERT(SUCCEEDED(result), "Failed to create IDxcUtils");

        // Create compiler instance
        result = DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&pCompiler));
        ASSERT(SUCCEEDED(result), "Failed to create IDxcCompiler3");

        // Create source blob
        CComPtr<IDxcBlobEncoding> pSource;
        result = pUtils->CreateBlob(shaderSource.GetShaderCode().data(),
                                     shaderSource.GetShaderCode().size(),
                                     CP_UTF8, &pSource);
        ASSERT(SUCCEEDED(result), "Failed to create source blob");

        // Set up DXC buffer
        DxcBuffer buffer;
        buffer.Ptr = pSource->GetBufferPointer();
        buffer.Size = pSource->GetBufferSize();
        buffer.Encoding = DXC_CP_UTF8;




        const wchar_t* shaderStageProfile = L"";
        switch (shaderSource.GetShaderType())
        {
            case ShaderType::VERTEX_SHADER: shaderStageProfile = L"vs_6_0"; break;
            case ShaderType::FRAGMENT_SHADER: shaderStageProfile = L"ps_6_0"; break;
            case ShaderType::COMPUTE_SHADER: shaderStageProfile = L"cs_6_0"; break;
            case ShaderType::GEOMETRY_SHADER: shaderStageProfile = L"gs_6_0"; break;
            case ShaderType::TESSELLATION_CONTROL_SHADER: shaderStageProfile = L"hs_6_0"; break;
            case ShaderType::TESSELLATION_EVALUATION_SHADER: shaderStageProfile = L"ds_6_0"; break;
            case ShaderType::NONE: AE_ERROR("Invalid shader type!"); break;
            default: AE_ERROR("Invalid shader type!"); break;
        }


        // Prepare compiler arguments
        std::vector<LPCWSTR> args = {
            L"-E", L"main",                      // Entry point
            L"-T", shaderStageProfile,        // Target profile (e.g., ps_6_0)
            L"-spirv",                           // Enable SPIR-V output
            L"-fspv-target-env=vulkan1.2",       // Target Vulkan 1.2
            L"/Zi",                              // Debug info
        };

        // Compile
        CComPtr<IDxcResult> pResult;
        result = pCompiler->Compile(&buffer, args.data(), (UINT32)args.size(),
                                     nullptr, IID_PPV_ARGS(&pResult));
        ASSERT(SUCCEEDED(result), "Compilation call failed");

        // Check compilation status
        HRESULT compilationStatus;
        result = pResult->GetStatus(&compilationStatus);
        ASSERT(SUCCEEDED(result), "Failed to get compilation status");

        // Handle compilation failure
        if (FAILED(compilationStatus)) {
            CComPtr<IDxcBlobEncoding> pErrors;
            pResult->GetOutput(DXC_OUT_ERRORS, IID_PPV_ARGS(&pErrors), nullptr);

            if (pErrors && pErrors->GetBufferSize() > 0)
            {
                std::string errorMsg(
                    static_cast<const char*>(pErrors->GetBufferPointer()),
                    pErrors->GetBufferSize()
                );
                AE_ERROR(errorMsg.c_str());
            }

            AE_ERROR("Shader compilation failed");
        }

        // Extract SPIR-V output
        CComPtr<IDxcBlob> pSpirv;
        result = pResult->GetOutput(DXC_OUT_OBJECT, IID_PPV_ARGS(&pSpirv), nullptr);
        ASSERT(SUCCEEDED(result) && pSpirv != nullptr, "Failed to get SPIR-V output");

        // Copy to output vector
        std::vector<uint32> SPIRV_Code;

        const uint32* pSpirvData = static_cast<const uint32*>(pSpirv->GetBufferPointer());
        size_t spirvSize = pSpirv->GetBufferSize() / sizeof(uint32);
        SPIRV_Code.assign(pSpirvData, pSpirvData + spirvSize);

        CreateShaderModule(SPIRV_Code);
    }


    void VulkanShader::CreateShaderModule(std::vector<uint32>& SPIRV_Code)
    {
        VkShaderModuleCreateInfo shaderCreateInfo = {
            .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
            .codeSize = SPIRV_Code.size() * sizeof(uint32),
            .pCode = SPIRV_Code.data(),
        };

        VkResult result = vkCreateShaderModule(m_Device, &shaderCreateInfo, nullptr, &m_ShaderModule);
        ASSERT(result == VK_SUCCESS, "Failed to create shader module!");
    }


    void VulkanShader::DestroyShaderModule()
    {
        vkDestroyShaderModule(m_Device, m_ShaderModule, nullptr);
    }

}

