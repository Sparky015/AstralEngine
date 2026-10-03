/**
* @file CubeLUTLoader.h
* @author Andrew Fagan
* @date 9/4/25
*/

#pragma once

#include "Asset/Asset.h"
#include "Asset/Ref.h"

namespace Astral::CubeLUTLoader {

    Ref<Asset> LoadAsset(const std::filesystem::path& filePath);

}