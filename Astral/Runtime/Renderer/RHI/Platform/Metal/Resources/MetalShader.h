/**
* @file MetalShader.h
* @author Andrew Fagan
* @date 5/24/26
*/

#pragma once

#include "Renderer/RHI/Resources/Shader.h"

#include "Metal/MTLDevice.hpp"

namespace Astral {

    struct MetalShaderDesc
    {
        MTL::Device* Device;
        const ShaderSource& ShaderSource;
    };

    class MetalShader : public Shader
    {
    public:

        MetalShader(const MetalShaderDesc& shaderDesc); // TODO
        ~MetalShader() override; // TODO

        void* GetNativeHandle() override; // TODO

    private:


    };

}