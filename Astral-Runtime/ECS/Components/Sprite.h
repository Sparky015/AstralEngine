//
// Created by Andrew Fagan on 11/28/24.
//

#pragma once

#include "pch.h"
#include "Asset/AssetRegistry.h"

#include "Renderer/Primitives/Texture.h"

#include "Renderer/Primitives/VertexArrayObject.h"

struct SpriteComponent
{
    SpriteComponent() : textureAssetID(Astral::NullAssetID), vertexArrayObject(nullptr), isUsed(false) {}
    explicit SpriteComponent(Astral::AssetID textureAssetID, Astral::VertexArrayObject* vertexArrayObject) : textureAssetID(textureAssetID), vertexArrayObject(vertexArrayObject), isUsed(true) {}
    Astral::AssetID textureAssetID;
    Astral::VertexArrayObject* vertexArrayObject;
    bool isUsed;
};