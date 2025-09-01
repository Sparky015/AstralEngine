/**
* @file Asset.h
* @author Andrew Fagan
* @date 4/11/25
*/

#pragma once

#include "Core/FixedIntegerTypes.h"
#include "AssetErrorCodes.h"
#include "Debug/Utilities/Error.h"

namespace Astral {

    enum class AssetType : uint8
    {
        Texture,
        Material,
        Shader,
        Mesh,
        EnvironmentMap
    };

    using AssetID = size_t;
    static constexpr AssetID NullAssetID = -1;

    class Asset
    {
    public:
        virtual ~Asset() = default;

        void SetAssetID(AssetID assetID) { m_AssetID = assetID; }
        AssetID GetAssetID() const { return m_AssetID; }

        virtual AssetType GetAssetType() = 0; // For debug and editor purposes

    private:

        AssetID m_AssetID{NullAssetID};
    };

    inline const char* AssetTypeToString(AssetType assetType)
    {
        switch (assetType)
        {
            case AssetType::Texture: return "Texture";
            case AssetType::Material: return "Material";
            case AssetType::Shader: return "Shader";
            case AssetType::Mesh: return "Mesh";
            case AssetType::EnvironmentMap: return "Environment Map";
            default: ASTRAL_ERROR("Invalid asset type value given!");
        }
    }

}
