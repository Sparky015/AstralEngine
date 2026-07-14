/**
* @file DirectedGraphHashes.h
* @author Andrew Fagan
* @date 3/29/26
*/

#pragma once

#include "Core/Utilities/HashCombiner.h"
#include "Core/FixedIntegerTypes.h"

// Forward declarations for the classes we'll need for hash specializations
namespace Astral {

template<typename DataType>
class Vertex;
}


template <typename DataType>
struct std::hash<Astral::Vertex<DataType>>
{
    size_t operator()(Astral::Vertex<DataType> const& v) const noexcept
    {
        return std::hash<DataType>()(v.GetData());
    }
};

