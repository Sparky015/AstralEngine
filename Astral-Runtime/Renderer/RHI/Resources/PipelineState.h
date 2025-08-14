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

    struct PushConstant
    {
        Mat4 ModelMatrix;
        uint32 HasNormalMap;
        uint32 HasDirectXNormals;
    };
    static_assert(sizeof(PushConstant) <= 128, "Push constant can not be greater than 128 bytes in size");


    struct PipelineStateCreateInfo
    {
        RenderPassHandle RenderPass;
        ShaderHandle VertexShader;
        ShaderHandle FragmentShader;
        const std::vector<DescriptorSetHandle>& DescriptorSets;
        const VertexBufferLayout& BufferLayout;
        uint32 SubpassIndex;
        bool IsAlphaBlended;
    };

    class PipelineState
    {
    public:
        virtual ~PipelineState() = default;

        virtual void Bind(CommandBufferHandle commandBufferHandle) = 0;
        virtual void BindDescriptorSet(CommandBufferHandle commandBufferHandle, DescriptorSetHandle descriptorSetHandle, uint32 binding) = 0;

        virtual void SetViewportAndScissor(CommandBufferHandle commandBufferHandle, UVec2 dimensions) = 0;

        virtual void* GetPipelineLayout() = 0;
        virtual void* GetHandleHandle() = 0;
    };

    using PipelineStateHandle = GraphicsRef<PipelineState>;

}
