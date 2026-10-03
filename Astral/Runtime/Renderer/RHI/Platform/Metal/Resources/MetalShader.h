/**
* @file MetalShader.h
* @author Andrew Fagan
* @date 5/24/26
*/

#pragma once

#include "Renderer/RHI/Resources/Shader.h"

#include "Metal/MTLDevice.hpp"
#include "spirv-cross/spirv_msl.hpp"

namespace Astral {

    /**
    * @brief Information to create a Metal shader from
    */
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
        MetalShader(const MetalShaderDesc& shaderDesc);
        ~MetalShader() override;

        /**
         * @brief Gets the shader reflection information for the shader
         * @return The shader reflection information for the shader
         */
        const ShaderReflectionInfo& GetShaderReflectionInfo() override;

        /**
         * @brief Gets the native shader object handle
         * @return The native shader object handle (MTL::Library*)
         */
        void* GetNativeHandle() override;

        /**
         * @brief Gets the main function handle of the shader
         * @return the main function handle of the shader
         */
        MTL::Function* GetFunctionHandle();

    private:

        /**
         * @brief Converts SPIR-V IR into MSL source code
         * @param shaderSource The GLSL shader source
         * @return A vector with the SPIR-V bytecode
         */
        std::vector<uint32> CompileGLSLToSPIRV(const ShaderSource& shaderSource);

        /**
         * @brief Converts SPIR-V IR into MSL source code
         * @param spirv The compiled SPIR-V IR
         * @return A string containing the MSL source code
         */
        std::string ConvertSPIRVToMSLSourceCode(const std::vector<uint32>& spirv);

        /**
         * @brief Populates the shader's reflection info
         * @param compiler The SPIRV-Cross compiler to pull reflection data from
         */
        void PopulateShaderReflectionInfo(const spirv_cross::CompilerMSL& compiler);

        /**
         * @brief Compiles Metal Shading Language (MSL) source code into Metal IR
         * @param mslSourceCode The MSL source code
         */
        void CompileShaderLibrary(const std::string& mslSourceCode);

        /**
         * @brief Releases the shader library from memory
         */
        void ReleaseShaderLibrary();

        /**
         * @brief Creates the entry point function object
         */
        void CreateFunctionEntry();

        /**
         * @brief Releases the entry point function object
         */
        void ReleaseFunctionEntry();


        MTL::Device* m_Device;
        MTL::Library* m_Library;
        MTL::Function* m_Function;

        ShaderReflectionInfo m_ShaderReflectionInfo;
    };

}
