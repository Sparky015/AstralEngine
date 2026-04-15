/**
* @file EnvironmentMapLoader.h
* @author Andrew Fagan
* @date 8/27/25
*/

#pragma once

#include "Asset/Asset.h"
#include "Core/SmartPointers.h"

#include <filesystem>


namespace Astral::EnvironmentMapLoader {

    Ref<Asset> LoadAsset(const std::filesystem::path& filePath);

}