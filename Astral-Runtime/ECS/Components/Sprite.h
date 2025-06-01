//
// Created by Andrew Fagan on 11/28/24.
//

#pragma once

#include "pch.h"
#include "Asset/AssetRegistry.h"
#include "Renderer/Common/Material.h"
#include "Renderer/Common/Mesh.h"

#include "Renderer/RHI/Resources/Texture.h"

#include "Renderer/RHI/Resources/VertexArrayObject.h"

struct SpriteComponent
{
    SpriteComponent() : material(), mesh(), isUsed(false) {}
    explicit SpriteComponent(Astral::Material material, Astral::Mesh mesh) : material(material), mesh(mesh), isUsed(true) {}
    Astral::Material material;
    Astral::Mesh mesh;
    bool isUsed;
};