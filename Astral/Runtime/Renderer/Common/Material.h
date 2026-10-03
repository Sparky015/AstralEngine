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

    /**
    * @enum ShaderModel
    * @brief The shader model to use for a material
    */
    enum class ShaderModel : uint8
    {
        PBR,
        UNLIT
    };

    /**
    * @enum TextureConvention
    * @brief The convention of the texture channel data used in a material
    */
    enum class TextureConvention : uint8
    {
        UNPACKED,
        ORM_PACKED
    };

    /**
     * @struct Material
     * @brief Defines material properties and textures used by the material
     */
    struct Material : public Asset
    {
        ShaderModel ShaderModel;
        TextureConvention TextureConvention;
        Ref<Shader> FragmentShader;
        std::vector<Ref<Texture>> Textures;
        std::vector<float> ParameterValues;
        DescriptorSetHandle DescriptorSet;
        bool HasNormalMap;
        bool HasDirectXNormals;
        bool IsAlphaBlended;

        /**
         * @brief Gets the type of the asset (AssetType::Material)
         * @return The type of asset (AssetType::Material)
         */
        AssetType GetAssetType() override { return GetStaticAssetType(); }

        /**
         * @brief Gets the type of the asset (AssetType::Material)
         * @return The type of asset (AssetType::Material)
         */
        static AssetType GetStaticAssetType() { return AssetType::Material; }
    };

    /**
     * @brief Gets the string version of a shader model
     * @return The shader model name as a string
     */
    inline std::string_view ShaderModelToString(ShaderModel shaderModel)
    {
        switch (shaderModel)
        {
            case ShaderModel::UNLIT: return "Unlit";
            case ShaderModel::PBR: return "PBR";
            default: return "None";
        }
    }

    /**
     * @brief Gets the shader model enum value from a string version of the enum value
     * @return The shader model enum value
     */
    inline ShaderModel StringToShaderModel(std::string_view shaderModel)
    {
        if (shaderModel == "Unlit") return ShaderModel::UNLIT;
        if (shaderModel == "PBR") return ShaderModel::PBR;
        AE_ERROR("Unsupported shader model string given!");
    }

    /**
     * @brief Gets the string version of a texture convention enum value
     * @return The texture convention name as a string
     */
    inline std::string_view TextureConventionToString(TextureConvention textureConvention)
    {
        switch (textureConvention)
        {
            case TextureConvention::UNPACKED: return "Unpacked";
            case TextureConvention::ORM_PACKED: return "AO-Roughness-Metallic Packed";
            default: return "None";
        }
    }

    /**
     * @brief Gets the texture convention enum value from a string version of the enum value
     * @return The texture convention enum value
     */
    inline TextureConvention StringToTextureConvention(std::string_view textureConvention)
    {
        if (textureConvention == "Unpacked") return TextureConvention::UNPACKED;
        if (textureConvention == "AO-Roughness-Metallic Packed") return TextureConvention::ORM_PACKED;
        AE_ERROR("Unsupported texture convention string given!");
    }

}
