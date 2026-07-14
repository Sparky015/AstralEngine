/**
* @file ShaderLoader.h
* @author Andrew Fagan
* @date 6/1/25
*/

#pragma once

#include "Asset/Asset.h"
#include "Core/SmartPointers.h"

#include <filesystem>

namespace Astral::ShaderLoader {

    Ref<Asset> LoadAsset(const std::filesystem::path& filePath);

}