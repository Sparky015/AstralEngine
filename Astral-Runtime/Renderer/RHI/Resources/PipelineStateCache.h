/**
* @file PipelineStateCache.h
* @author Andrew Fagan
* @date 6/27/2025
*/


#pragma once

#include "PipelineStateObject.h"
#include "Renderer/RHI/Resources/Renderpass.h"
#include "Renderer/Common/Mesh.h"
#include "Renderer/Common/Material.h"
#include "Core/Utility/Hashing/Hashes.h"

namespace Astral {

    struct PipelineStateConfiguration
    {
        RenderPassHandle RenderPass;
        ShaderHandle VertexShader;
        ShaderHandle FragmentShader;
        DescriptorSetHandle ShaderDataLayout;
        VertexBufferLayout VertexBufferLayout;

        bool operator==(const PipelineStateConfiguration& other) const;
    };

    class PipelineStateCache
    {
    public:
        void SetSceneDescriptorSet(DescriptorSetHandle sceneDescriptorSet);
        PipelineStateObjectHandle GetPipeline(RenderPassHandle renderPass, Material& material, Mesh& mesh, uint32 subpassIndex);

    private:

        DescriptorSetHandle m_SceneDescriptorSet{nullptr};
        std::unordered_map<PipelineStateConfiguration, PipelineStateObjectHandle> m_PipelineCache;
    };

}