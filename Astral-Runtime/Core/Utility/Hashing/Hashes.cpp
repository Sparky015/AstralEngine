/**
* @file Hashes.cpp
* @author Andrew Fagan
* @date 6/27/2025
*/

#include "Hashes.h"

#include "Core/Containers/DirectedGraph.h"
#include "Renderer/RHI/Resources/PipelineStateCache.h"


std::size_t std::hash<Astral::VertexBufferAttribute>::operator()(const Astral::VertexBufferAttribute& attr) const noexcept
{
    return std::hash<uint8>{}(attr.DataType);
}

std::size_t std::hash<Astral::VertexBufferLayout>::operator()(const Astral::VertexBufferLayout& layout) const noexcept
{
    std::size_t seed = 0;
    Astral::HashCombine(seed, std::hash<uint32>{}(layout.GetStride()));
    for (const auto& attr : layout.GetAttributes())
    {
        Astral::HashCombine(seed, std::hash<Astral::VertexBufferAttribute>{}(attr));
    }
    return seed;
}

size_t std::hash<Astral::PipelineStateConfiguration>::operator()(const Astral::PipelineStateConfiguration& config) const noexcept
{
    std::size_t seed = 0;

    if (config.RenderPass)
    {
        Astral::HashCombine(seed, std::hash<void*>{}(config.RenderPass->GetNativeHandle()));
    } else {
        Astral::HashCombine(seed, 0);
    }

    if (config.VertexShader)
    {
        Astral::HashCombine(seed, std::hash<void*>{}(config.VertexShader->GetNativeHandle()));
    } else {
        Astral::HashCombine(seed, 0);
    }

    if (config.FragmentShader)
    {
        Astral::HashCombine(seed, std::hash<void*>{}(config.FragmentShader->GetNativeHandle()));
    } else {
        Astral::HashCombine(seed, 0);
    }


    for (Astral::Descriptor descriptor : config.ShaderDataLayout.Descriptors)
    {
        Astral::HashCombine(seed, std::hash<Astral::Descriptor>{}(descriptor));
    }

    Astral::HashCombine(seed, std::hash<Astral::VertexBufferLayout>{}(config.VertexBufferLayout));

    Astral::HashCombine(seed, std::hash<bool>{}(config.IsAlphaBlended));

    return seed;
}




