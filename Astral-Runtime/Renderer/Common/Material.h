/**
* @file Material.h
* @author Andrew Fagan
* @date 5/24/2025
*/

#pragma once

#include "Renderer/RHI/Resources/Shader.h"
#include "Renderer/RHI/Resources/Texture.h"
#include "Asset/Asset.h"
#include "Asset/AssetRegistry.h"
#include "Renderer/RHI/Resources/DescriptorSet.h"

namespace Astral {

    enum class ShaderModel : uint8
    {
        UNLIT,
        PBR,
    };

    enum class TextureConvention : uint8
    {
        UNPACKED,
        ORM_PACKED
    };

    struct Material : public Asset
    {
        ShaderModel ShaderModel;
        TextureConvention TextureConvention;
        Ref<Shader> FragmentShader;
        std::vector<Ref<Texture>> Textures;
        DescriptorSetHandle DescriptorSet;
        bool HasNormalMap;
        bool HasDirectXNormals;
        bool IsAlphaBlended;

        AssetType GetAssetType() override { return GetStaticAssetType(); };
        static AssetType GetStaticAssetType() { return AssetType::Material; }
    };

    inline std::string_view ShaderModelToString(ShaderModel shaderModel)
    {
        switch (shaderModel)
        {
            case ShaderModel::UNLIT: return "Unlit";
            case ShaderModel::PBR: return "PBR";
            default: return "None";
        }
    }

    inline std::string_view TextureConventionToString(TextureConvention textureConvention)
    {
        switch (textureConvention)
        {
            case TextureConvention::UNPACKED: return "Unpacked";
            case TextureConvention::ORM_PACKED: return "AO-Roughness-Metallic Packed";
            default: return "None";
        }
    }

}
