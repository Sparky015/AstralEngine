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
#include "ECS/Components/SpriteComponent.h"
#include "ECS/Components/TransformComponent.h"
#include "glm/ext/matrix_transform.hpp"
#include "Window/Window.h"
#include "Window/WindowManager.h"
#include "Renderer/SceneRenderer.h"

#include "Input/InputState.h"

#include <cmath>

namespace Astral {

    void RenderingSystem::RenderEntities()
    {
        PROFILE_SCOPE("RenderingSystem::RenderEntities");

        // TODO Make a templated iterator that does the below checking on iterations and holds the state of which ID it is at
        static Camera camera = Camera(CameraType::PERSPECTIVE, 1.0, 800.0f);
        static constexpr float magnitude = 5;
        static Vec2 initialMousePos{};
        static Vec3 initialRotation{};
        static bool isReadyToTrack = true;

        if (InputState::IsKeyDown(KEY_RIGHT_CLICK))
        {
            if (isReadyToTrack)
            {
                initialMousePos = InputState::MousePosition();
                initialRotation = camera.GetRotation();
                isReadyToTrack = false;
            }

            Vec2 currentMousePos = InputState::MousePosition();
            float xDiff = currentMousePos.x - initialMousePos.x;
            float yDiff = currentMousePos.y - initialMousePos.y;

            Vec3 newRotation = {initialRotation.x - yDiff, initialRotation.y - xDiff, initialRotation.z};
            camera.SetRotation(newRotation);
        }
        else
        {
            isReadyToTrack = true;
        }


        if (InputState::IsKeyDown(KEY_A))
        {
            const Vec3& rotation = camera.GetRotation();
            const Vec3& position = camera.GetPosition();
            camera.SetPosition(Vec3(position.x - (magnitude), position.y, position.z));
        }
        if (InputState::IsKeyDown(KEY_D))
        {
            const Vec3& rotation = camera.GetRotation();
            const Vec3& position = camera.GetPosition();
            camera.SetPosition(Vec3(position.x + (magnitude), position.y, position.z));
        }
        if (InputState::IsKeyDown(KEY_LEFT_SHIFT) || InputState::IsKeyDown(KEY_RIGHT_SHIFT))
        {
            const Vec3& position = camera.GetPosition();
            camera.SetPosition(Vec3(position.x, position.y + magnitude, position.z));
        }
        if (InputState::IsKeyDown(KEY_SPACE))
        {
            const Vec3& position = camera.GetPosition();
            camera.SetPosition(Vec3(position.x, position.y - magnitude, position.z));
        }

        if (InputState::IsKeyDown(KEY_W))
        {
            const Vec3& rotation = camera.GetRotation();
            const Vec3& position = camera.GetPosition();
            camera.SetPosition(Vec3(position.x, position.y, position.z - magnitude));
        }

        if (InputState::IsKeyDown(KEY_S))
        {
            const Vec3& rotation = camera.GetRotation();
            const Vec3& position = camera.GetPosition();
            camera.SetPosition(Vec3(position.x, position.y, position.z + magnitude));
        }

        if (InputState::IsKeyDown(KEY_H))
        {
            Vec3 rotation = camera.GetRotation();
            rotation.z += 1;
            camera.SetRotation(rotation);
        }

        if (InputState::IsKeyDown(KEY_G))
        {
            Vec3 rotation = camera.GetRotation();
            rotation.z -= 1;
            camera.SetRotation(rotation);
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

        SubmitMeshComponents();
        SubmitSpriteComponents();

        SceneRenderer::EndScene();
    }


    void RenderingSystem::SubmitMeshComponents()
    {
        ECS& ecs = Engine::Get().GetECSManager().GetECS();

        ECS::ComponentView<MeshComponent>& meshDisplay = ecs.GetView<MeshComponent>();
        const ECS::ComponentView<TransformComponent>& transformDisplay = ecs.GetView<TransformComponent>();

        for (Entity entity : ecs)
        {
            if (!ecs.HasComponent<MeshComponent>(entity)) { continue; }
            EntityID entityID = entity.GetID();

            const TransformComponent& transformComponent = transformDisplay[entityID];
            MeshComponent& meshComponent = meshDisplay[entityID];

            if (meshComponent.Material == nullptr) { continue; }
            if (meshComponent.MeshData == nullptr) { continue; }

            Mat4 modelTransform = CreateTransform(transformComponent.position, transformComponent.scale);

            SceneRenderer::Submit(*meshComponent.MeshData, *meshComponent.Material, modelTransform);
        }
    }


    void RenderingSystem::SubmitSpriteComponents()
    {
        ECS& ecs = Engine::Get().GetECSManager().GetECS();

        ECS::ComponentView<SpriteComponent>& spriteDisplay = ecs.GetView<SpriteComponent>();
        const ECS::ComponentView<TransformComponent>& transformDisplay = ecs.GetView<TransformComponent>();


        for (Entity entity : ecs)
        {
            if (!ecs.HasComponent<SpriteComponent>(entity)) { continue; }
            EntityID entityID = entity.GetID();

            const TransformComponent& transformComponent = transformDisplay[entityID];
            SpriteComponent& spriteComponent = spriteDisplay[entityID];

            if (spriteComponent.Material == nullptr) { continue; }
            if (spriteComponent.MeshData == nullptr) { continue; }

            Mat4 modelTransform = CreateTransform(transformComponent.position, transformComponent.scale);

            SceneRenderer::Submit(*spriteComponent.MeshData, *spriteComponent.Material, modelTransform);
        }
    }


    Mat4 RenderingSystem::CreateTransform(Vec3 position, Vec3 scale)
    {
        Mat4 scaleMatrix = glm::scale(Mat4(1.0f), scale);
        return glm::translate(Mat4(1.0f), position) * scaleMatrix;
    }

}
