//
// Created by Andrew Fagan on 7/23/24.
//

#pragma once

#include "../../../vendor/glm/glm/vec2.hpp"
#include "../../../vendor/glm/glm/vec4.hpp"

#include "Ayla/Renderer/Abstractions/IndexBufferObject.h"
#include "Ayla/Renderer/Abstractions/VertexBuffer.h"

// TEMP
namespace Ayla::ECS {

    class Entity
    {
    public:
        Entity();
        explicit Entity(glm::vec2 position);
        Entity(glm::vec2 position, glm::vec4 color);
        ~Entity() = default;


        glm::vec2 m_Position;
        glm::vec4 m_Color;
        glm::vec2 m_Vertices[4];

    private:
//        Renderer::IndexBufferObject m_IndexBuffer = Renderer::IndexBufferObject(,);
//        Renderer::VertexBuffer m_VertexBuffer = Renderer::VertexBuffer(,);

    };


} // namespace Ayla::ECS