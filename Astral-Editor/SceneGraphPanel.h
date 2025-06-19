/**
* @file SceneGraphPanel.h
* @author Andrew Fagan
* @date 3/1/2025
*/

#pragma once

#include "ECS/ECS.h"
#include "ECS/Entity.h"

namespace Astral {

    void SceneGraphPanel();


    // Helpers

    void ShowTransformComponent(ECS& ecs, const Entity& entity);
    void ShowSpriteComponent(ECS& ecs, const Entity& entity);
    void ShowMeshComponent(ECS& ecs, const Entity& entity);

    void GetAssetFilePath(AssetRegistry& registry, AssetID materialAssetID, char* inputBuffer, int inputBufferSize);

}
