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

#include "Input/InputState.h"

namespace Astral {

    void RenderingSystem::RenderEntities()
    {
        PROFILE_SCOPE("RenderingSystem::RenderEntities");
        ECS& ecs = Engine::Get().GetECSManager().GetECS();

        ECS::ComponentView<SpriteComponent>& spriteDisplay = ecs.GetView<SpriteComponent>();
        const ECS::ComponentView<TransformComponent>& transformDisplay = ecs.GetView<TransformComponent>();

        // TODO Make a templated iterator that does the below checking on iterations and holds the state of which ID it is at
        static OrthographicCamera camera = OrthographicCamera(1.0, 800.0f);
        static constexpr float magnitude = 5;

        if (InputState::IsKeyDown(KEY_A))
        {
            const Vec3& position = camera.GetPosition();
            camera.SetPosition(Vec3(position.x - magnitude, position.y, position.z));
        }
        if (InputState::IsKeyDown(KEY_D))
        {
            const Vec3& position = camera.GetPosition();
            camera.SetPosition(Vec3(position.x + magnitude, position.y, position.z));
        }
        if (InputState::IsKeyDown(KEY_S))
        {
            const Vec3& position = camera.GetPosition();
            camera.SetPosition(Vec3(position.x, position.y + magnitude, position.z));
        }
        if (InputState::IsKeyDown(KEY_W))
        {
            const Vec3& position = camera.GetPosition();
            camera.SetPosition(Vec3(position.x, position.y - magnitude, position.z));
        }

        if (InputState::IsKeyDown(KEY_E))
        {
            const Vec3& position = camera.GetPosition();
            camera.SetPosition(Vec3(position.x, position.y, position.z - magnitude));
        }

        if (InputState::IsKeyDown(KEY_Q))
        {
            const Vec3& position = camera.GetPosition();
            camera.SetPosition(Vec3(position.x, position.y, position.z + magnitude));
        }

        if (InputState::IsKeyDown(KEY_H))
        {
            const float rotation = camera.GetRotation();
            camera.SetRotation(rotation + 1);
        }

        if (InputState::IsKeyDown(KEY_G))
        {
            const float rotation = camera.GetRotation();
            camera.SetRotation(rotation - 1);
        }

        if (InputState::IsKeyDown(KEY_T))
        {
            const float zoom = camera.GetZoomLevel();
            camera.SetZoom(zoom + 5);
        }

        if (InputState::IsKeyDown(KEY_Y))
        {
            const float zoom = camera.GetZoomLevel();
            camera.SetZoom(zoom - 5);
        }


        SceneDescription sceneDescription = {
            .Camera = camera,
        };

        SceneRenderer::BeginScene(sceneDescription);
        for (Entity entity : ecs)
        {
            if (!ecs.IsEntityAlive(entity)) { continue; }
            if (!ecs.HasComponent<SpriteComponent>(entity)) { continue; }
            EntityID entityID = entity.GetID();

            const TransformComponent& transformComponent = transformDisplay[entityID];
            SpriteComponent& spriteComponent = spriteDisplay[entityID];

            Mat4 transform = CreateTransform(transformComponent.position, transformComponent.scale);

            AssetRegistry& registry = Engine::Get().GetAssetManager().GetRegistry();
            Ref<Material> material = registry.GetAsset<Material>(spriteComponent.materialAssetID);
            SceneRenderer::Submit(spriteComponent.mesh, *material, transform);
        }
        SceneRenderer::EndScene();
    }


    Mat4 RenderingSystem::CreateTransform(Vec3 position, Vec3 scale)
    {
        Mat4 scaleMatrix = glm::scale(Mat4(1.0f), scale);
        return glm::translate(Mat4(1.0f), position) * scaleMatrix;
    }

}
