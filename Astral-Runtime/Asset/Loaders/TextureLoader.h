/**
* @file TextureLoader.h
* @author Andrew Fagan
* @date 4/13/25
*/

#pragma once

#include <filesystem>

#include "Asset/Asset.h"

namespace Astral::TextureLoader {

    Asset* LoadAsset(const std::filesystem::path& filePath);

}
