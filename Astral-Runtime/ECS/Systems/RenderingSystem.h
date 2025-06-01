/**
* @file RenderingSystem.h
* @author Andrew Fagan
* @date 12/10/2024
*/

#pragma once

#include "Renderer/RHI/Resources/Shader.h"

namespace Astral {

    class RenderingSystem
    {
    public:

        static void RenderEntities();

    private:

        static Mat4 CreateTransform(Vec3 position, Vec3 scale);


    };



}