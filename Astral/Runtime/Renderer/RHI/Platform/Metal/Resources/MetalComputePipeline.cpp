/**
* @file MetalComputePipeline.cpp
* @author Andrew Fagan
* @date 5/24/26
*/

#include "MetalComputePipeline.h"

#include "Metal/MTLComputePipeline.hpp"

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
        NS::Error* error = nullptr;
        MTL::Function* function = (MTL::Function*)computePipelineStateDesc.ComputeShader->GetNativeHandle();
        MTL::ComputePipelineDescriptor* pipelineDescriptor = MTL::ComputePipelineDescriptor::alloc();
        pipelineDescriptor->setComputeFunction(function);

        m_Pipeline = m_Device->newComputePipelineState(pipelineDescriptor, MTL::PipelineOptionNone, nullptr, &error);

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
