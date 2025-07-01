/**
* @file RenderingSystem.h
* @author Andrew Fagan
* @date 12/10/2024
*/

#pragma once

#include "Core/Math/Math.h"
#include "ECS/Components/PointLightComponent.h"
#include "ECS/Components/TransformComponent.h"
#include "Renderer/Cameras/Camera.h"

namespace Astral {

    class RenderingSystem
    {
    public:

        static void RenderEntities();
        static Camera& GetCamera();

    private:

        static void SubmitMeshComponents();
        static void SubmitSpriteComponents();
        static void GetPointLightComponents(std::vector<Light>& outLights);

        static Mat4 CreateTransform(const TransformComponent& transform);
    };



}
