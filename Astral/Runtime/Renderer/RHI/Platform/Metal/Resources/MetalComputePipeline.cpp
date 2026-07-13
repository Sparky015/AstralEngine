/**
* @file MetalComputePipeline.cpp
* @author Andrew Fagan
* @date 5/24/26
*/

#include "MetalComputePipeline.h"

#include "MetalShader.h"
#include "Metal/MTLComputePipeline.hpp"
#include "Renderer/RHI/RendererAPI.h"
#include "Renderer/RHI/Platform/Metal/MetalRendererContext.h"

namespace Astral {

    MetalComputePipelineState::MetalComputePipelineState(const MetalComputePipelineStateDesc& computePipelineStateDesc) :
        m_Device(computePipelineStateDesc.Device),
        m_Pipeline(nullptr),
        m_DescriptorSetLayout(),
        m_CompiledComputeShader(computePipelineStateDesc.ComputeShader)
    {
        CreateDescriptorSetLayout(computePipelineStateDesc);
        CreatePipelineState(computePipelineStateDesc);
    }


    MetalComputePipelineState::~MetalComputePipelineState()
    {
        ReleasePipelineState();
    }


    PipelineType MetalComputePipelineState::GetPipelineType()
    {
        return PipelineType::COMPUTE;
    }


    ShaderHandle MetalComputePipelineState::GetCompiledVertexShader()
    {
        return nullptr;
    }


    ShaderHandle MetalComputePipelineState::GetCompiledFragmentShader()
    {
        return nullptr;
    }


    ShaderHandle MetalComputePipelineState::GetCompiledComputeShader()
    {
        return m_CompiledComputeShader;
    }


    const std::vector<DescriptorSetLayout>& MetalComputePipelineState::GetDescriptorSetLayout() const
    {
        return m_DescriptorSetLayout;
    }


    void* MetalComputePipelineState::GetPipelineLayout()
    {
        return nullptr;
    }


    void* MetalComputePipelineState::GetNativeHandle()
    {
        return m_Pipeline;
    }


    void MetalComputePipelineState::CreatePipelineState(const MetalComputePipelineStateDesc& computePipelineStateDesc)
    {
        MTL4::LibraryFunctionDescriptor* computeFunctionDescriptor = MTL4::LibraryFunctionDescriptor::alloc()->init();
        GraphicsRef<MetalShader> metalComputeShader = std::static_pointer_cast<MetalShader>(computePipelineStateDesc.ComputeShader);
        MTL::Library* computeLibrary = (MTL::Library*)metalComputeShader->GetNativeHandle();
        computeFunctionDescriptor->setLibrary(computeLibrary);
        computeFunctionDescriptor->setName(NS::String::string("main0", NS::UTF8StringEncoding));

        MTL4::ComputePipelineDescriptor* pipelineDescriptor = MTL4::ComputePipelineDescriptor::alloc()->init();
        pipelineDescriptor->setComputeFunctionDescriptor(computeFunctionDescriptor);
        computeFunctionDescriptor->release();

        MetalRenderingContext& renderingContext = static_cast<MetalRenderingContext&>(RendererAPI::GetContext());
        MTL4::Compiler* compiler = renderingContext.GetCompiler();
        MTL4::CompilerTaskOptions* options = MTL4::CompilerTaskOptions::alloc()->init();
        NS::Error* error = nullptr;

        m_Pipeline = compiler->newComputePipelineState(pipelineDescriptor, options, &error);

        pipelineDescriptor->release();

        if (m_Pipeline == nullptr)
        {
            AE_ERROR("Shader failed to compile! Error: " << error->localizedDescription()->utf8String())
            error->release();
        }
    }


    void MetalComputePipelineState::ReleasePipelineState()
    {
        if (m_Pipeline)
        {
            m_Pipeline->release();
        }
    }


    void MetalComputePipelineState::CreateDescriptorSetLayout(const MetalComputePipelineStateDesc& computePipelineStateDesc)
    {
        m_DescriptorSetLayout.reserve(computePipelineStateDesc.DescriptorSets.size());
        for (DescriptorSetHandle descriptorSet : computePipelineStateDesc.DescriptorSets)
        {
            m_DescriptorSetLayout.push_back(descriptorSet->GetDescriptorSetLayout());
        }
    }

}
