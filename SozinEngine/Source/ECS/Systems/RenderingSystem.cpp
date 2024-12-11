/**
* @file RenderingSystem.cpp
* @author Andrew Fagan
* @date 12/10/2024
*/
#include "RenderingSystem.h"

#include "ECS/ECSManager.h"
#include "glm/ext/matrix_transform.hpp"
#include "Window/Window.h"
#include "Window/WindowManager.h"
#include "Renderer/Renderer.h"


namespace Sozin {

    void RenderingSystem::RenderEntities(Graphics::ShaderProgram* shader)
    {
        ECS::ECS& ecs = ECS::g_ECSManager.GetECS();

        ECS::ECS::ComponentDisplay<SpriteComponent> spriteDisplay = ecs.GetComponentDisplay<SpriteComponent>();
        ECS::ECS::ComponentDisplay<TransformComponent> transformDisplay = ecs.GetComponentDisplay<TransformComponent>();

        for (ECS::EntityPoolSize entityID = 0; entityID < ECS::MAX_ENTITIES; entityID++)
        {
            if (!spriteDisplay[entityID].isUsed) { continue; }
            if (!transformDisplay[entityID].isUsed) { continue; }

            TransformComponent transformComponent = transformDisplay[entityID];
            SpriteComponent spriteComponent = spriteDisplay[entityID];

            Vec3 position = Vec3(transformComponent.x, transformComponent.y, transformComponent.z);
            Mat4 transform = CreateTransform(position, Vec3(transformComponent.scaleX, transformComponent.scaleY, 1));

            spriteComponent.texture->Bind(0);
            Graphics::Renderer::Submit(*shader, spriteComponent.vertexArrayObject, transform);
        }
    }


    Mat4 RenderingSystem::CreateTransform(Vec3 position, Vec3 scale) {
        Window::Window& window = Window::g_WindowManager.GetWindow();
        Mat4 scaleMatrix = glm::scale(Mat4(1.0f), scale);

        Vec3 normalizedPosition = Vec3(1.0f);
        normalizedPosition.x = position.x / window.GetWidth() * 2 - 1;
        normalizedPosition.y = position.y / window.GetHeight() * 2 - 1;

        return glm::translate(Mat4(1.0f), normalizedPosition) * scaleMatrix;
    }

}
