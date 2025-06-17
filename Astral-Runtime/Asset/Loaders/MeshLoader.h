/**
* @file MeshLoader.h
* @author Andrew Fagan
* @date 6/16/25
*/

#pragma once

#include "Asset/Asset.h"
#include "Core/SmartPointers.h"

#include <filesystem>

namespace Astral::MeshLoader {

    Ref<Asset> LoadAsset(const std::filesystem::path& filePath);

}
