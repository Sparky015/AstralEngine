/**
* @file RenderingSystem.h
* @author Andrew Fagan
* @date 12/10/2024
*/

#pragma once

#include "Renderer/AbstractionLayer/Primitives/ShaderProgram.h"

namespace Astral {

    class RenderingSystem
    {
    public:

        static void RenderEntities(Graphics::ShaderProgram* shader);

    private:

        static Mat4 CreateTransform(Vec3 position, Vec3 scale);


    };



}