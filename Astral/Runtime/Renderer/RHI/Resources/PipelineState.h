/**
* @file PipelineState.h
* @author Andrew Fagan
* @date 5/13/25
*/

#pragma once

#include "DescriptorSet.h"
#include "Shader.h"
#include "VertexBufferLayout.h"
#include "Renderpass.h"
#include "Renderer/RHI/Common/GraphicsSmartPointers.h"
#include "Renderer/RHI/Common/PipelineRasterizerState.h"

namespace Astral {

    /**
     * @brief Defines the max number of bytes that can be used in a push constant
     */
    static constexpr uint32 MaxPushConstantRange = 128;

    /**
     * @brief Information to create a graphics pipeline state
     */
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
        CullMode CullMode;
    };

    /**
     * @brief Information to create a compute pipeline state
     */
    struct ComputePipelineStateCreateInfo
    {
        ShaderHandle ComputeShader;
        const std::vector<DescriptorSetHandle>& DescriptorSets;
    };

    /**
     * @brief Describes the type of pipeline
     */
    enum class PipelineType
    {
        GRAPHICS,
        COMPUTE
    };

    /**
     * @brief Defines the RHI PipelineState object
     */
    class PipelineState
    {
    public:
        virtual ~PipelineState() = default;

        /**
         * @brief Gets the pipeline type of this pipeline
         * @return The pipeline type of this pipeline
         */
        virtual PipelineType GetPipelineType() = 0;

        /**
         * @brief Gets the vertex shader compiled by this pipeline
         * @return The vertex shader compiled by this pipeline or nullptr if the pipeline did not use a vertex shader
         */
        virtual ShaderHandle GetCompiledVertexShader() = 0;

        /**
         * @brief Gets the fragment shader compiled by this pipeline
         * @return The fragment shader compiled by this pipeline or nullptr if the pipeline did not use a fragment shader
         */
        virtual ShaderHandle GetCompiledFragmentShader() = 0;

        /**
         * @brief Gets the compute shader compiled by this pipeline
         * @return The compute shader compiled by this pipeline or nullptr if the pipeline did not use a compute shader
         */
        virtual ShaderHandle GetCompiledComputeShader() = 0;

        /**
         * @brief Gets the descriptor set layouts of the pipeline
         * @return The descriptor set layouts of the pipeline
         */
        virtual const std::vector<DescriptorSetLayout>& GetDescriptorSetLayout() const = 0;

        /**
         * @brief Gets the native pipeline layout
         * @return The native pipeline layout
         */
        virtual void* GetPipelineLayout() = 0;

        /**
         * @brief Gets the native pipeline object
         * @return The native pipeline object
         */
        virtual void* GetNativeHandle() = 0;
    };

    using PipelineStateHandle = GraphicsRef<PipelineState>;

}
