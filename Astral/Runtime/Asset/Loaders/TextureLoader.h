/**
* @file TextureLoader.h
* @author Andrew Fagan
* @date 4/13/25
*/

#pragma once

#include "Asset/Asset.h"
#include "Core/SmartPointers.h"



namespace Astral::TextureLoader {

    Ref<Asset> LoadAsset(const std::filesystem::path& filePath);

}
