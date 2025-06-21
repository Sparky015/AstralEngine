/**
* @file MeshLoader.h
* @author Andrew Fagan
* @date 6/16/25
*/

#pragma once

#include "Asset/Asset.h"
#include "Core/SmartPointers.h"

#include <filesystem>
#include "assimp/mesh.h"
#include "Renderer/Common/Mesh.h"

namespace Astral::MeshLoader {

    Ref<Asset> LoadAsset(const std::filesystem::path& filePath);

    Ref<Mesh> LoadMesh(aiMesh* mesh);

}
