/**
* @file MetalGraphicsPipeline.cpp
* @author Andrew Fagan
* @date 5/24/26
*/

#include "MetalGraphicsPipeline.h"

#include "Metal/MTL4Compiler.hpp"
#include "Metal/MTL4LibraryFunctionDescriptor.hpp"
#include "Metal/MTL4RenderPipeline.hpp"
#include "Metal/MTLRenderPipeline.hpp"
#include "Metal/MTLVertexDescriptor.hpp"
#include "Renderer/RHI/RendererAPI.h"
#include "Renderer/RHI/Platform/Metal/MetalRendererContext.h"
#include "Renderer/RHI/Platform/Metal/Common/MTLEnumConversions.h"

namespace Astral {

    MetalGraphicsPipelineState::MetalGraphicsPipelineState(const MetalGraphicsPipelineStateDesc& graphicsPipelineStateDesc) :
        m_Device(graphicsPipelineStateDesc.Device),
        m_PipelineDescriptor(nullptr),
        m_Pipeline(nullptr),
        m_DescriptorSetLayout(),
        m_CompiledVertexShader(graphicsPipelineStateDesc.VertexShader),
        m_CompiledFragmentShader(graphicsPipelineStateDesc.FragmentShader)
    {
        CreateDescriptorSetLayout(graphicsPipelineStateDesc);
        CreatePipelineState(graphicsPipelineStateDesc);
    }


    MetalGraphicsPipelineState::~MetalGraphicsPipelineState()
    {
        ReleasePipelineState();
    }


    PipelineType MetalGraphicsPipelineState::GetPipelineType()
    {
        return PipelineType::GRAPHICS;
    }


    ShaderHandle MetalGraphicsPipelineState::GetCompiledVertexShader()
    {
        return m_CompiledVertexShader;
    }


    ShaderHandle MetalGraphicsPipelineState::GetCompiledFragmentShader()
    {
        return m_CompiledFragmentShader;
    }


    ShaderHandle MetalGraphicsPipelineState::GetCompiledComputeShader()
    {
        return nullptr;
    }


    const std::vector<DescriptorSetLayout>& MetalGraphicsPipelineState::GetDescriptorSetLayout() const
    {
        return m_DescriptorSetLayout;
    }


    void* MetalGraphicsPipelineState::GetPipelineLayout()
    {
        return nullptr;
    }


    void* MetalGraphicsPipelineState::GetNativeHandle()
    {
        return m_Pipeline;
    }


    void MetalGraphicsPipelineState::CreatePipelineState(const MetalGraphicsPipelineStateDesc& graphicsPipelineStateDesc)
    {
        m_PipelineDescriptor = MTL4::RenderPipelineDescriptor::alloc()->init();
        m_PipelineDescriptor->setInputPrimitiveTopology(MTL::PrimitiveTopologyClassTriangle);
        m_PipelineDescriptor->setRasterSampleCount(ConvertSampleCountToIntSampleCount(graphicsPipelineStateDesc.MSAASamples));

        PopulatePipelineColorAttachmentDescriptor(graphicsPipelineStateDesc);
        PopulatePipelineVertexDescriptor(graphicsPipelineStateDesc);
        PopulateFunctionDescriptors(graphicsPipelineStateDesc);

        MetalRenderingContext& renderingContext = static_cast<MetalRenderingContext&>(RendererAPI::GetContext());
        MTL4::Compiler* compiler = renderingContext.GetCompiler();
        MTL4::CompilerTaskOptions* options = MTL4::CompilerTaskOptions::alloc()->init();
        NS::Error* error = nullptr;

        m_Pipeline = compiler->newRenderPipelineState(m_PipelineDescriptor, options, &error);

        m_PipelineDescriptor->release();
        options->release();

        if (m_Pipeline == nullptr)
        {
            AE_ERROR("Shader failed to compile! Error: " << error->localizedDescription()->utf8String())
            error->release();
        }
    }


    void MetalGraphicsPipelineState::ReleasePipelineState()
    {
        if (m_Pipeline)
        {
            m_Pipeline->release();
        }
    }


    void MetalGraphicsPipelineState::PopulatePipelineColorAttachmentDescriptor(const MetalGraphicsPipelineStateDesc& graphicsPipelineStateDesc)
    {
        MTL4::RenderPipelineColorAttachmentDescriptorArray* colorAttachmentDescriptorArray = m_PipelineDescriptor->colorAttachments();
        const std::vector<AttachmentReference>& colorAttachmentReferences = graphicsPipelineStateDesc.RenderPass->GetColorAttachmentReferences();
        MTL4::BlendState blendState = (graphicsPipelineStateDesc.IsAlphaBlended) ? MTL4::BlendStateEnabled : MTL4::BlendStateDisabled;

        for (size_t i = 0; i < colorAttachmentReferences.size(); i++)
        {
            const AttachmentReference& colorAttachmentReference = colorAttachmentReferences[i];
            AttachmentDescription colorAttachmentDescription = graphicsPipelineStateDesc.RenderPass->GetAttachmentDescription(colorAttachmentReference.AttachmentIndex);
            MTL::PixelFormat pixelFormat = ConvertImageFormatToMTLPixelFormat(colorAttachmentDescription.Format);

            MTL4::RenderPipelineColorAttachmentDescriptor* colorAttachmentDescriptor = MTL4::RenderPipelineColorAttachmentDescriptor::alloc()->init();

            colorAttachmentDescriptor->setPixelFormat(pixelFormat);
            colorAttachmentDescriptor->setBlendingState(blendState);

            colorAttachmentDescriptorArray->setObject(colorAttachmentDescriptor, i);

            colorAttachmentDescriptor->release();
        }
    }


    void MetalGraphicsPipelineState::PopulatePipelineVertexDescriptor(const MetalGraphicsPipelineStateDesc& graphicsPipelineStateDesc)
    {
        MTL::VertexDescriptor* vertexDescriptor = MTL::VertexDescriptor::alloc()->init();

        uint32 offset = 0;
        for (size_t i = 0; i < graphicsPipelineStateDesc.BufferLayout.GetAttributes().size(); i++)
        {
            const VertexBufferAttribute& attribute = graphicsPipelineStateDesc.BufferLayout.GetAttributes()[i];

            MTL::VertexAttributeDescriptor* vertexAttributeDescriptor = MTL::VertexAttributeDescriptor::alloc()->init();

            vertexAttributeDescriptor->setFormat(ConvertShaderDataTypeToMTLVertexFormat(attribute.DataType));
            vertexAttributeDescriptor->setBufferIndex(29);
            vertexAttributeDescriptor->setOffset(offset);
            vertexDescriptor->attributes()->setObject(vertexAttributeDescriptor, i);

            offset += attribute.GetAttributeSize();
            vertexAttributeDescriptor->release();
        }


        MTL::VertexBufferLayoutDescriptor* vertexBufferLayoutDescriptor = MTL::VertexBufferLayoutDescriptor::alloc()->init();

        uint32 stride = graphicsPipelineStateDesc.BufferLayout.GetStride();
        vertexBufferLayoutDescriptor->setStride(stride);
        vertexBufferLayoutDescriptor->setStepFunction(MTL::VertexStepFunctionPerVertex);
        vertexBufferLayoutDescriptor->setStepRate(1);

        vertexDescriptor->layouts()->setObject(vertexBufferLayoutDescriptor, 29);

        m_PipelineDescriptor->setVertexDescriptor(vertexDescriptor);

        vertexDescriptor->release();
        vertexBufferLayoutDescriptor->release();
    }


    void MetalGraphicsPipelineState::PopulateFunctionDescriptors(const MetalGraphicsPipelineStateDesc& graphicsPipelineStateDesc)
    {
        MTL4::LibraryFunctionDescriptor* vertexFunctionDescriptor = MTL4::LibraryFunctionDescriptor::alloc()->init();
        vertexFunctionDescriptor->setName(NS::String::string("main0", NS::UTF8StringEncoding));
        MTL::Library* vertexLibrary = (MTL::Library*)graphicsPipelineStateDesc.VertexShader->GetNativeHandle();
        vertexFunctionDescriptor->setLibrary(vertexLibrary);
        m_PipelineDescriptor->setVertexFunctionDescriptor(vertexFunctionDescriptor);
        vertexFunctionDescriptor->release();

        MTL4::LibraryFunctionDescriptor* fragmentFunctionDescriptor = MTL4::LibraryFunctionDescriptor::alloc()->init();
        fragmentFunctionDescriptor->setName(NS::String::string("main0", NS::UTF8StringEncoding));
        MTL::Library* fragmentLibrary = (MTL::Library*)graphicsPipelineStateDesc.FragmentShader->GetNativeHandle();
        fragmentFunctionDescriptor->setLibrary(fragmentLibrary);
        m_PipelineDescriptor->setFragmentFunctionDescriptor(fragmentFunctionDescriptor);
        fragmentFunctionDescriptor->release();
    }


    void MetalGraphicsPipelineState::CreateDescriptorSetLayout(const MetalGraphicsPipelineStateDesc& graphicsPipelineStateDesc)
    {
        m_DescriptorSetLayout.reserve(graphicsPipelineStateDesc.DescriptorSets.size());
        for (DescriptorSetHandle descriptorSet : graphicsPipelineStateDesc.DescriptorSets)
        {
            m_DescriptorSetLayout.push_back(descriptorSet->GetDescriptorSetLayout());
        }
    }

}
