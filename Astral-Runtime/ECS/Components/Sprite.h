//
// Created by Andrew Fagan on 11/28/24.
//

#pragma once

#include "Asset/AssetRegistry.h"
#include "Renderer/Common/Mesh.h"

struct SpriteComponent
{
    SpriteComponent() : materialAssetID(), mesh(), isUsed(false) {}
    explicit SpriteComponent(Astral::AssetID materialAssetID, Astral::Mesh mesh) : materialAssetID(materialAssetID), mesh(mesh), isUsed(true) {}
    Astral::AssetID materialAssetID;
    Astral::Mesh mesh;
    bool isUsed;
};