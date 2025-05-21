/**
* @file Sampler.h
* @author Andrew Fagan
* @date 5/13/25
*/

#pragma once

namespace Astral {

    class Sampler
    {
    public:
        virtual ~Sampler() = default;

        virtual void* GetNativeHandle() = 0;
    };

    using SamplerHandle = GraphicsRef<Sampler>;

}