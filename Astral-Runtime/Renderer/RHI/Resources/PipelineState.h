/**
* @file PipelineState.h
* @author Andrew Fagan
* @date 5/13/25
*/

#pragma once

#include "CommandBuffer.h"
#include "DescriptorSet.h"
#include "Shader.h"
#include "VertexBufferLayout.h"
#include "Renderpass.h"
#include "Renderer/RHI/Common/GraphicsSmartPointers.h"

namespace Astral {

    static constexpr uint32 MaxPushConstantRange = 128;

    struct GraphicsPipelineStateCreateInfo
    {
        RenderPassHandle RenderPass;
        ShaderHandle VertexShader;
        ShaderHandle FragmentShader;
        const std::vector<DescriptorSetHandle>& DescriptorSets;
        const VertexBufferLayout& BufferLayout;
        uint32 SubpassIndex;
        bool IsAlphaBlended;
        SampleCount MSAASamples;
    };

    struct ComputePipelineStateCreateInfo
    {
        ShaderHandle ComputeShader;
        const std::vector<DescriptorSetHandle>& DescriptorSets;
    };

    enum class PipelineType
    {
        GRAPHICS,
        COMPUTE
    };

    class PipelineState
    {
    public:
        virtual ~PipelineState() = default;

        virtual void BindPipeline(CommandBufferHandle commandBufferHandle) = 0;
        virtual void BindDescriptorSet(CommandBufferHandle commandBufferHandle, DescriptorSetHandle descriptorSetHandle, uint32 binding) = 0;

        virtual void SetViewportAndScissor(CommandBufferHandle commandBufferHandle, UVec2 dimensions) = 0;

        virtual PipelineType GetPipelineType() = 0;
        virtual void* GetPipelineLayout() = 0;
        virtual void* GetHandleHandle() = 0;
    };

    using PipelineStateHandle = GraphicsRef<PipelineState>;

}
