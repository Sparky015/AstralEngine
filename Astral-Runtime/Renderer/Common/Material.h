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

    struct Material : public Asset
    {
        ShaderModel ShaderModel;
        Ref<Shader> FragmentShader;
        std::vector<Ref<Texture>> Textures;
        DescriptorSetHandle DescriptorSet;

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

}
