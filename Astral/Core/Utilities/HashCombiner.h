/**
* @file Hash.h
* @author Andrew Fagan
* @date 6/27/2025
*/

#pragma once

#include <functional>

namespace Astral {

    inline void HashCombine(size_t& seed, size_t hash)
    {
        hash += 0x9e3779b9 + (seed << 6) + (seed >> 2);
        seed ^= hash;
    }

}

