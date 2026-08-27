/**
* @file cpptraceOperators.h
* @author Andrew Fagan
* @date 8/26/26
*/

#pragma once

#include <cpptrace/cpptrace.hpp>
#include "Core/Utilities/HashCombiner.h"

template <>
struct std::hash<cpptrace::raw_trace>
{
    size_t operator()(cpptrace::raw_trace const& v) const noexcept;
};

inline std::size_t std::hash<cpptrace::raw_trace>::operator()(cpptrace::raw_trace const& v) const noexcept
{
    size_t hash = 0;

    for (uintptr_t frame : v.frames)
    {
        size_t frameHash = std::hash<uintptr_t>{}(frame);
        Astral::HashCombine(hash, frameHash);
    }

    return hash;
}

inline bool operator<(const cpptrace::raw_trace& l, const cpptrace::raw_trace& r)
{
    return std::hash<cpptrace::raw_trace>{}(l) < std::hash<cpptrace::raw_trace>{}(r);
}


