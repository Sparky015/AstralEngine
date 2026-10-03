/**
* @file Hashes.h
* @author Andrew Fagan
* @date 6/27/2025
*/

#pragma once

#include "Core/Utilities/HashCombiner.h"

#include "Core/FixedIntegerTypes.h"

// Forward declarations for the classes we'll need for hash specializations
namespace Astral {
    struct VertexBufferAttribute;
    class VertexBufferLayout;
    struct GraphicsPipelineStateConfiguration;
    struct ComputePipelineStateConfiguration;
    class RenderGraphPass;
}

#include "Renderer/RHI/Resources/VertexBufferLayout.h"


template <>
struct std::hash<Astral::VertexBufferAttribute>
{
    size_t operator()(Astral::VertexBufferAttribute const& v) const noexcept;
};

template <>
struct std::hash<Astral::VertexBufferLayout>
{
    size_t operator()(Astral::VertexBufferLayout const& v) const noexcept;
};

template <>
struct std::hash<Astral::GraphicsPipelineStateConfiguration>
{
    size_t operator()(Astral::GraphicsPipelineStateConfiguration const& config) const noexcept;
};

template <>
struct std::hash<Astral::ComputePipelineStateConfiguration>
{
    size_t operator()(Astral::ComputePipelineStateConfiguration const& config) const noexcept;
};