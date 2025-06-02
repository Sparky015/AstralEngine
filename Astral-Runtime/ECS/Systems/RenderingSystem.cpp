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
        static OrthographicCamera camera = OrthographicCamera();
        static constexpr float magnitude = 5;
        // Move Left (A key)
        if (InputState::IsKeyDown(KEY_A))
        {
            const Vec3& position = camera.GetPosition();
            camera.SetPosition(Vec3(position.x - magnitude, position.y, position.z));
        }
        // Move Backwards (S key)
        if (InputState::IsKeyDown(KEY_D))
        {
            const Vec3& position = camera.GetPosition();
            // Move along the Z-axis (backwards)
            camera.SetPosition(Vec3(position.x + magnitude, position.y, position.z));
        }
        // Move Right (D key)
        if (InputState::IsKeyDown(KEY_LEFT_SHIFT))
        {
            const Vec3& position = camera.GetPosition();
            // Move along the X-axis (right)
            camera.SetPosition(Vec3(position.x, position.y - magnitude, position.z));
        }
        // Move Forwards (W key)
        if (InputState::IsKeyDown(KEY_SPACE))
        {
            const Vec3& position = camera.GetPosition();
            // Move along the Z-axis (forwards)
            camera.SetPosition(Vec3(position.x, position.y + magnitude, position.z));
        }

        if (InputState::IsKeyDown(KEY_W))
        {
            const Vec3& position = camera.GetPosition();
            // Move along the Z-axis (forwards)
            camera.SetPosition(Vec3(position.x, position.y, position.z - magnitude));
        }

        if (InputState::IsKeyDown(KEY_S))
        {
            const Vec3& position = camera.GetPosition();
            // Move along the Z-axis (forwards)
            camera.SetPosition(Vec3(position.x, position.y, position.z + magnitude));
        }

        LOG("Camera Position: (" << camera.GetPosition().x << ", " << camera.GetPosition().y << ", " << camera.GetPosition().z << ")");

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
        Mat4 transform = glm::translate(Mat4(1.0f), position); // Apply position (world units)
        transform = glm::scale(transform, scale);
        return transform;
    }

}
