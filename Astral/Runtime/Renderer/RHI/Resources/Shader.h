/**
* @file Shader.h
* @author Andrew Fagan
* @date 12/4/2024
*/

#pragma once

#include "Asset/Asset.h"
#include "ShaderSource.h"
#include "Renderer/RHI/Common/GraphicsSmartPointers.h"

namespace Astral {

    /**
     * @brief Defines a RHI shader interface
     */
    class Shader : public Asset
    {
    public:
        virtual ~Shader() = default;

        /**
         * @brief Gets the native shader object handle
         * @return The native shader object handle
         */
        virtual void* GetNativeHandle() = 0;

        /**
         * @brief Compiles a shader and creates a shader object from the given shader source
         * @param shaderSource The shader source to create the shader object with
         * @return A shader object for the given shader source
         */
        static GraphicsRef<Shader> CreateShader(const ShaderSource& shaderSource);

        /**
         * @brief Gets the type of asset this Asset object is
         * @return The type of asset this Asset object is
         */
        AssetType GetAssetType() override { return GetStaticAssetType(); }

        /**
         * @brief Gets the type of asset this class is
         * @return The type of asset this class is
         */
        static AssetType GetStaticAssetType() { return AssetType::Shader; }
    };

    using ShaderHandle = GraphicsRef<Shader>;

} // Renderer