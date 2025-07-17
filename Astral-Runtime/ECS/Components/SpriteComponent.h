//
// Created by Andrew Fagan on 11/28/24.
//

#pragma once

#include "Asset/AssetManager.h"
#include "Asset/AssetRegistry.h"
#include "Core/Engine.h"
#include "Renderer/Common/Material.h"
#include "Renderer/Common/Mesh.h"

namespace Astral {

    struct SpriteComponent
    {
        SpriteComponent() :
            Material(nullptr),
            MeshData(Engine::Get().GetAssetManager().GetRegistry().GetAsset<Mesh>("Meshes/Quad.obj")) {}
        explicit SpriteComponent(Ref<Material> material) :
            Material(material),
            MeshData(Engine::Get().GetAssetManager().GetRegistry().GetAsset<Mesh>("Meshes/Quad.obj")) {}
        explicit SpriteComponent(Ref<Material> material, Ref<Mesh> mesh) :
            Material(material),
            MeshData(mesh) {}
        Ref<Material> Material;
        Ref<Mesh> MeshData;
    };

}
