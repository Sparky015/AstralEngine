/**
* @file MeshComponent.h
* @author Andrew Fagan
* @date 6/18/25
*/

#pragma once

#include "Asset/AssetRegistry.h"
#include "Renderer/Common/Material.h"
#include "Renderer/Common/Mesh.h"

namespace Astral {

    struct MeshComponent
    {
        MeshComponent() : Material(nullptr), MeshData(nullptr) {}
        explicit MeshComponent(Ref<Material> material, Ref<Mesh> mesh) :
            Material(material),
            MeshData(mesh) {}
        Ref<Material> Material;
        Ref<Mesh> MeshData;
    };

}