/**
* @file RenderingSystem.h
* @author Andrew Fagan
* @date 12/10/2024
*/

#pragma once

#include "Core/Math/Math.h"
#include "ECS/Components/TransformComponent.h"

namespace Astral {

    class RenderingSystem
    {
    public:

        static void RenderEntities();

    private:

        static void SubmitMeshComponents();
        static void SubmitSpriteComponents();

        static Mat4 CreateTransform(const TransformComponent& transform);
    };



}
