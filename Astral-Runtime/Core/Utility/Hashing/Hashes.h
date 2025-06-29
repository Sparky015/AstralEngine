/**
* @file Hashes.h
* @author Andrew Fagan
* @date 6/27/2025
*/

#pragma once

#include "HashCombiner.h"

// Forward declarations for the classes we'll need for hash specializations
namespace Astral {
    struct VertexBufferAttribute;
    class VertexBufferLayout;
    struct PipelineStateConfiguration;
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
struct std::hash<Astral::PipelineStateConfiguration>
{
    size_t operator()(Astral::PipelineStateConfiguration const& v) const noexcept;
};


