/**
* @file RenderingSystem.cpp
* @author Andrew Fagan
* @date 12/10/2024
*/
#include "RenderingSystem.h"

#include "Scenes/SceneManager.h"
#include "Asset/AssetManager.h"
#include "Core/Engine.h"
#include "Debug/Instrumentation/ScopeProfiler.h"
#include "ECS/Components/SpriteComponent.h"
#include "ECS/Components/TransformComponent.h"
#include "Window/Window.h"
#include "Window/WindowManager.h"
#include "Renderer/SceneRenderer.h"

#include <cmath>

#include "glm/ext/matrix_transform.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/euler_angles.hpp"

namespace Astral {

    void RenderingSystem::RenderEntities()
    {
        PROFILE_SCOPE("RenderingSystem::RenderEntities");

        Scene& scene = Engine::Get().GetSceneManager().GetActiveScene();

        std::vector<Light> lights;
        GetLightComponents(lights);

        SceneDescription sceneDescription = {
            .Camera = scene.PrimaryCamera,
            .Lights = lights,
            .EnvironmentMap = scene.EnvironmentMap,
            .AmbientLightConstant = scene.AmbientLightConstant,
            .Exposure = scene.Exposure
        };


        SceneRenderer::BeginScene(sceneDescription);

        SubmitMeshComponents();
        SubmitSpriteComponents();

        SceneRenderer::EndScene();
    }


    void RenderingSystem::SubmitMeshComponents()
    {
        ECS& ecs = Engine::Get().GetSceneManager().GetECS();

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

            Mat4 modelTransform = CreateTransform(transformComponent);

            SceneRenderer::Submit(*meshComponent.MeshData, *meshComponent.Material, modelTransform);
        }
    }


    void RenderingSystem::SubmitSpriteComponents()
    {
        ECS& ecs = Engine::Get().GetSceneManager().GetECS();

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

            Mat4 modelTransform = CreateTransform(transformComponent);

            SceneRenderer::Submit(*spriteComponent.MeshData, *spriteComponent.Material, modelTransform);
        }
    }

    void RenderingSystem::GetLightComponents(std::vector<Light>& outLights)
    {
        ECS& ecs = Engine::Get().GetSceneManager().GetECS();
        const ECS::ComponentView<TransformComponent>& transformDisplay = ecs.GetView<TransformComponent>();
        const ECS::ComponentView<PointLightComponent>& pointLightDisplay = ecs.GetView<PointLightComponent>();
        const ECS::ComponentView<DirectionalLightComponent>& directionalLightDisplay = ecs.GetView<DirectionalLightComponent>();

        for (Entity entity : ecs)
        {
            EntityID entityID = entity.GetID();

            if (ecs.HasComponent<PointLightComponent>(entity))
            {

                const TransformComponent& transformComponent = transformDisplay[entityID];
                const PointLightComponent& pointLightComponent = pointLightDisplay[entityID];

                if (pointLightComponent.Intensity == 0) { continue; }
                if (pointLightComponent.LightColor == Vec3(0.0f)) { continue; }

                Light light{};
                light.Position = transformComponent.position;
                light.LightColor = pointLightComponent.LightColor * pointLightComponent.Intensity;
                light.LightType = LightType::POINT;
                outLights.push_back(light);
            }
            else if (ecs.HasComponent<DirectionalLightComponent>(entity))
            {
                const TransformComponent& transformComponent = transformDisplay[entityID];
                const DirectionalLightComponent& directionalLightComponent = directionalLightDisplay[entityID];

                if (directionalLightComponent.Intensity == 0) { continue; }
                if (directionalLightComponent.LightColor == Vec3(0.0f)) { continue; }

                Light light{};

                Vec3 rotationInRadians = glm::radians(transformComponent.rotation);
                Mat4 rotationMatrix = glm::eulerAngleXYZ(rotationInRadians.x, rotationInRadians.y, rotationInRadians.z);
                Vec4 directionVec4 = Vec4(directionalLightComponent.Direction, 0.0f);
                light.Position = glm::normalize(rotationMatrix * directionVec4);

                light.LightColor = directionalLightComponent.LightColor * directionalLightComponent.Intensity;
                light.LightType = LightType::DIRECTIONAL;
                outLights.push_back(light);
            }

        }
    }


    Mat4 RenderingSystem::CreateTransform(const TransformComponent& transform)
    {
        Vec3 rotationInRadians = glm::radians(transform.rotation);
        Mat4 rotationMatrix = glm::eulerAngleXYZ(rotationInRadians.x, rotationInRadians.y, rotationInRadians.z);
        Mat4 scaleMatrix = glm::scale(Mat4(1.0f), transform.scale);
        return glm::translate(Mat4(1.0f), transform.position) * rotationMatrix * scaleMatrix;
    }

}
