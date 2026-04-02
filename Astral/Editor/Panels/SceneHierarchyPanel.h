/**
* @file SceneHierarchyPanel.h
* @author Andrew Fagan
* @date 3/1/2025
*/

#pragma once

#include "ECS/ECS.h"
#include "ECS/Entity.h"

namespace Astral {

    class SceneHierarchyPanel
    {
    public:
        static void Show();
        static Entity GetSelectedEntity() { return selectedEntity; }

    private:

        static Entity selectedEntity;
    };

    // Helpers

    void ShowTransformComponent(ECS& ecs, const Entity& entity);
    void ShowSpriteComponent(ECS& ecs, const Entity& entity);
    void ShowMeshComponent(ECS& ecs, const Entity& entity);


}
