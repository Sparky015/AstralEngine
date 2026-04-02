/**
* @file MaterialLoader.h
* @author Andrew Fagan
* @date 6/1/25
*/

#pragma once

#include "Asset/Asset.h"
#include "Core/SmartPointers.h"

#include <filesystem>

#include "assimp/material.h"
#include "Renderer/Common/Material.h"

namespace Astral {

    class MaterialLoader
    {
    public:
        static Ref<Asset> LoadAsset(const std::filesystem::path& filePath);

        static void SerializeMaterial(Ref<Material> material, std::filesystem::path& outFilePath);
    };




}
