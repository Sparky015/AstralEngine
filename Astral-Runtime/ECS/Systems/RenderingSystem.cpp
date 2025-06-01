/**
* @file RenderingSystem.cpp
* @author Andrew Fagan
* @date 12/10/2024
*/
#include "RenderingSystem.h"

#include "../ECSManager.h"
#include "Asset/AssetManager.h"
#include "Core/Engine.h"
#include "Debug/Instrumentation/ScopeProfiler.h"
#include "ECS/Components/Sprite.h"
#include "ECS/Components/Transform.h"
#include "glm/ext/matrix_transform.hpp"
#include "Window/Window.h"
#include "Window/WindowManager.h"
#include "Renderer/SceneRenderer.h"


namespace Astral {

    void RenderingSystem::RenderEntities()
    {
        PROFILE_SCOPE("RenderingSystem::RenderEntities");
        ECS& ecs = Engine::Get().GetECSManager().GetECS();

        ECS::ComponentView<SpriteComponent>& spriteDisplay = ecs.GetView<SpriteComponent>();
        const ECS::ComponentView<TransformComponent>& transformDisplay = ecs.GetView<TransformComponent>();

        // TODO Make a templated iterator that does the below checking on iterations and holds the state of which ID it is at
        OrthographicCamera camera = OrthographicCamera(-10, 10, -10, 10);
        SceneRenderer::BeginScene(camera);
        for (EntityID entityID = 0; entityID < ecs.GetNumberOfActiveEntities(); entityID++)
        {
            Entity e = Entity(entityID);
            if (!ecs.IsEntityAlive(entityID)) { continue; }
            if (!ecs.HasComponent<SpriteComponent>(e)) { continue; }
            if (!transformDisplay[entityID].isUsed) { continue; }

            const TransformComponent& transformComponent = transformDisplay[entityID];
            SpriteComponent& spriteComponent = spriteDisplay[entityID];

            Vec3 position = Vec3(transformComponent.x, transformComponent.y, transformComponent.z);
            Mat4 transform = CreateTransform(position, Vec3(transformComponent.scaleX, transformComponent.scaleY, 1));

            AssetRegistry& registry = Engine::Get().GetAssetManager().GetRegistry();
            Ref<Material> material = registry.GetAsset<Material>(spriteComponent.materialAssetID);
            SceneRenderer::Submit(spriteComponent.mesh, *material, transform);
        }
        SceneRenderer::EndScene();
    }


    Mat4 RenderingSystem::CreateTransform(Vec3 position, Vec3 scale) {
        Astral::Window& window = Engine::Get().GetWindowManager().GetWindow();
        Mat4 scaleMatrix = glm::scale(Mat4(1.0f), scale);

        Vec3 normalizedPosition = Vec3(1.0f);
        normalizedPosition.x = position.x / window.GetWidth() * 2 - 1;
        normalizedPosition.y = position.y / window.GetHeight() * 2 - 1;

        return glm::translate(Mat4(1.0f), normalizedPosition) * scaleMatrix;
    }

}
