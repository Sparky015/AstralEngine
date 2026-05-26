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

    /**
     * @brief Defines a Metal RHI shader module object
     */
    class MetalShader : public Shader
    {
    public:

        MetalShader(const MetalShaderDesc& shaderDesc); // TODO
        ~MetalShader() override; // TODO

        void* GetNativeHandle() override; // TODO

    private:

        /**
         * @brief Converts HLSL/GLSL source code into MSL
         */
        void ConvertShaderToMSL(const ShaderSource& shaderSource);

        /**
         * @brief Compiles Metal Shading Language (MSL) source code into Metal IR
         */
        void CompileShaderLibrary(const std::string& MSLSourceCode);

        /**
         * @brief Releases the shader library from memory
         */
        void ReleaseShaderLibrary();

        /**
         * @brief Creates the entry point function object
         */
        void CreateFunctionEntry();

        /**
         * @brief Destroys the entry point function object
         */
        void DestroyFunctionEntry();


        MTL::Library* m_Library;
        MTL::Function* m_Function;
    };

}