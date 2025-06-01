/**
* @file Shader.h
* @author Andrew Fagan
* @date 12/4/2024
*/

#pragma once

#include "Asset/Asset.h"
#include "Renderer/Shaders/ShaderSource.h"
#include "Renderer/RHI/Common/GraphicsSmartPointers.h"

namespace Astral {

    class Shader : public Asset
    {
    public:
        virtual ~Shader() = default;

        static Shader* CreateShaderProgram(const ShaderSource& vertexShader, const ShaderSource& fragmentShader);
        static GraphicsRef<Shader> CreateShader(const ShaderSource& shaderSource);
        virtual void* GetNativeHandle() = 0;

        AssetType GetAssetType() override { return GetStaticAssetType(); }
        static AssetType GetStaticAssetType() { return AssetType::Shader; }
    };

    using ShaderHandle = GraphicsRef<Shader>;

} // Renderer