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
#include "Renderer/Renderer.h"


namespace Astral {

    void RenderingSystem::RenderEntities(Astral::ShaderProgram* shader)
    {
        PROFILE_SCOPE("RenderingSystem::RenderEntities");
        ECS& ecs = Engine::Get().GetECSManager().GetECS();

        const ECS::ComponentView<SpriteComponent>& spriteDisplay = ecs.GetView<SpriteComponent>();
        const ECS::ComponentView<TransformComponent>& transformDisplay = ecs.GetView<TransformComponent>();

        // TODO Make a templated iterator that does the below checking on iterations and holds the state of which ID it is at
        for (EntityID entityID = 0; entityID < spriteDisplay.size(); entityID++)
        {
            Entity e = Entity(entityID);
            if (!ecs.IsEntityAlive(entityID)) { continue; }
            if (!ecs.HasComponent<SpriteComponent>(e)) { continue; }
            if (!transformDisplay[entityID].isUsed) { continue; }

            const TransformComponent& transformComponent = transformDisplay[entityID];
            const SpriteComponent& spriteComponent = spriteDisplay[entityID];

            Vec3 position = Vec3(transformComponent.x, transformComponent.y, transformComponent.z);
            Mat4 transform = CreateTransform(position, Vec3(transformComponent.scaleX, transformComponent.scaleY, 1));

            g_AssetManager.GetRegistry().GetAsset<Texture>(spriteComponent.textureAssetID)->Bind(0);
            Astral::Renderer::Submit(*shader, spriteComponent.vertexArrayObject, transform);
        }
    }


    Mat4 RenderingSystem::CreateTransform(Vec3 position, Vec3 scale) {
        Window::Window& window = Engine::Get().GetWindowManager().GetWindow();
        Mat4 scaleMatrix = glm::scale(Mat4(1.0f), scale);

        Vec3 normalizedPosition = Vec3(1.0f);
        normalizedPosition.x = position.x / window.GetWidth() * 2 - 1;
        normalizedPosition.y = position.y / window.GetHeight() * 2 - 1;

        return glm::translate(Mat4(1.0f), normalizedPosition) * scaleMatrix;
    }

}
