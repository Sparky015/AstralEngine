/**
* @file MetalShader.cpp
* @author Andrew Fagan
* @date 5/24/26
*/

#include "MetalShader.h"

#include "Profiler/Instrumentation/ScopeProfiler.h"
#include "Core/Utilities/Asserts.h"
#include "Core/Utilities/Error.h"

#include "glslang/Include/glslang_c_interface.h"
#include "glslang/Public/resource_limits_c.h"
#include "glslang/Public/ShaderLang.h"
#include "spirv-cross/spirv_cross.hpp"
#include "spirv-cross/spirv_msl.hpp"
#include <Metal/Metal.hpp>

namespace Astral {

    MetalShader::MetalShader(const MetalShaderDesc& shaderDesc) :
        m_Device(shaderDesc.Device)
    {
        std::vector<uint32> spirv = CompileGLSLToSPIRV(shaderDesc.ShaderSource);
        std::string mslSourceCode = ConvertSPIRVToMSLSourceCode(spirv);
        CompileShaderLibrary(mslSourceCode);
        CreateFunctionEntry();
    }


    MetalShader::~MetalShader()
    {
        ReleaseFunctionEntry();
        ReleaseShaderLibrary();
    }


    const ShaderReflectionInfo& MetalShader::GetShaderReflectionInfo()
    {
        return m_ShaderReflectionInfo;
    }


    void* MetalShader::GetNativeHandle()
    {
        return m_Function;
    }


    std::vector<uint32> MetalShader::CompileGLSLToSPIRV(const ShaderSource& shaderSource)
    {
        PROFILE_SCOPE("VulkanShader::CompileShader");
        m_ShaderReflectionInfo.ShaderType = shaderSource.GetShaderType();
        m_ShaderReflectionInfo.ShaderFileName = shaderSource.GetFileName();

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
            case NONE: AE_ERROR("Invalid shader type!"); break;
            default: AE_ERROR("Invalid shader type!"); break;
        }


        const glslang_input_t input = {
            .language = GLSLANG_SOURCE_GLSL,
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

        return SPIRV_Code;
    }


    std::string MetalShader::ConvertSPIRVToMSLSourceCode(const std::vector<uint32>& spirv)
    {
        spirv_cross::CompilerMSL compiler(spirv);

        spirv_cross::CompilerMSL::Options compilerOptions;
        compilerOptions.platform = spirv_cross::CompilerMSL::Options::macOS;
        compilerOptions.set_msl_version(3, 2);
        compilerOptions.argument_buffers = true;

        // Rebind the push constant buffer if applicable to bind slot 30
        spirv_cross::MSLResourceBinding pushConstantResourceBinding{};
        pushConstantResourceBinding.desc_set = spirv_cross::ResourceBindingPushConstantDescriptorSet;
        pushConstantResourceBinding.binding = spirv_cross::ResourceBindingPushConstantBinding;
        pushConstantResourceBinding.msl_buffer = 30;
        compiler.add_msl_resource_binding(pushConstantResourceBinding);

        std::string mslSourceCode = compiler.compile();

        PopulateShaderReflectionInfo(compiler);

        return mslSourceCode;
    }


    void MetalShader::PopulateShaderReflectionInfo(const spirv_cross::CompilerMSL& compiler)
    {
        // === Get the workgroup size of a compute shader if applicable =====================

        if (m_ShaderReflectionInfo.ShaderType == ShaderType::COMPUTE_SHADER)
        {
            const spirv_cross::SPIREntryPoint& entryPoint = compiler.get_entry_point("main", spv::ExecutionModelGLCompute);
            m_ShaderReflectionInfo.WorkgroupDimensions = Vec3(entryPoint.workgroup_size.x, entryPoint.workgroup_size.y, entryPoint.workgroup_size.z);
        }


        // === Get the counts of each shader resource type declared in the shader =====================

        spirv_cross::ShaderResources shaderResources = compiler.get_shader_resources();
        ShaderResourceCounts& resourceCounts = m_ShaderReflectionInfo.ShaderResourceCounts;
        resourceCounts.CombinedSampledImagesCount = shaderResources.sampled_images.size();
        resourceCounts.SeparateImagesCount = shaderResources.separate_images.size();
        resourceCounts.SeparateSamplersCount = shaderResources.separate_samplers.size();
        resourceCounts.StorageImages = shaderResources.storage_images.size();
        resourceCounts.StorageBuffers = shaderResources.storage_buffers.size();
        resourceCounts.UniformBuffers = shaderResources.uniform_buffers.size();
        resourceCounts.PushConstants = shaderResources.push_constant_buffers.size();
        resourceCounts.StageInputs = shaderResources.stage_inputs.size();
        resourceCounts.StageOutputs = shaderResources.stage_outputs.size();


        // === Get the resource binding layout of the shader =====================

        std::vector<ShaderResourceBindSlot>& resourceDecorations = m_ShaderReflectionInfo.DeclaredResources;

        std::array<spirv_cross::SmallVector<spirv_cross::Resource>*, 6> shaderResourceInfos =
            {&shaderResources.sampled_images, &shaderResources.separate_images, &shaderResources.separate_samplers,
            &shaderResources.storage_images, &shaderResources.storage_buffers, &shaderResources.uniform_buffers};

        std::array<ShaderResourceType, 6> shaderResourceTypes = {
            ShaderResourceType::COMBINED_SAMPLED_IMAGE, ShaderResourceType::SEPARATE_IMAGE, ShaderResourceType::SEPARATE_SAMPLER,
            ShaderResourceType::STORAGE_IMAGE, ShaderResourceType::STORAGE_BUFFER, ShaderResourceType::UNIFORM_BUFFER
        };

        for (size_t i = 0; i < shaderResourceInfos.size(); i++)
        {
            for (const spirv_cross::Resource& resource : *shaderResourceInfos[i])
            {
                ShaderResourceBindSlot resourceBindSlot = {};
                resourceBindSlot.DescriptorSet = compiler.get_decoration(resource.id, spv::DecorationDescriptorSet);
                resourceBindSlot.BindSlot = compiler.get_decoration(resource.id, spv::DecorationBinding);
                resourceBindSlot.ResourceType = shaderResourceTypes[i];
                resourceBindSlot.Name = compiler.get_name(resource.id);
                resourceDecorations.push_back(resourceBindSlot);
            }
        }

        std::ranges::sort(m_ShaderReflectionInfo.DeclaredResources,
                          [](ShaderResourceBindSlot& resourceOne, ShaderResourceBindSlot& resourceTwo) {
                              return (resourceOne.DescriptorSet < resourceTwo.DescriptorSet) ||
                                     (resourceOne.DescriptorSet == resourceTwo.DescriptorSet && resourceOne.BindSlot < resourceTwo.BindSlot);
                          }
        );
    }


    void MetalShader::CompileShaderLibrary(const std::string& mslSourceCode)
    {
        NS::Error* errorCode = nullptr;
        NS::String* sourceCode = NS::String::string(mslSourceCode.c_str(), NS::StringEncoding::UTF8StringEncoding);
        MTL::CompileOptions* compileOptions = MTL::CompileOptions::alloc();
        compileOptions->setLanguageVersion(MTL::LanguageVersion3_2);
        m_Device->newLibrary(sourceCode, &errorCode);

        if (m_Library == nullptr)
        {
            AE_ERROR("Shader failed to compile! Error: " << errorCode->localizedDescription()->utf8String())
            errorCode->release();
        }
    }


    void MetalShader::ReleaseShaderLibrary()
    {
        m_Library->release();
        m_Library = nullptr;
    }


    void MetalShader::CreateFunctionEntry()
    {
        NS::String* entryName = NS::String::string("main", NS::StringEncoding::UTF8StringEncoding);
        m_Library->newFunction(entryName);
    }


    void MetalShader::ReleaseFunctionEntry()
    {
        m_Function->release();
        m_Function = nullptr;
    }

}
