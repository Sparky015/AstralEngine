//
// Created by Andrew Fagan on 7/23/24.
//

#include "Entity.h"

namespace Ayla::ECS{


    Entity::Entity() : m_Position(0, 0), m_Color(1, 1, 1, 1)
    {

    }


    Entity::Entity(glm::vec2 position) : m_Position(position), m_Color(1, 1, 1, 1)
    {

    }


    Entity::Entity(glm::vec2 position, glm::vec4 color) : m_Position(position), m_Color(color)
    {

    }

} // namespace Ayla::ECS
