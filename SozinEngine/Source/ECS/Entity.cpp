//
// Created by Andrew Fagan on 11/27/24.
//

#include "Entity.h"

namespace ECS {

    Entity::Entity(EntityPoolSize id) : m_ID(id), m_IsAlive(true) {}

    Entity::Entity(EntityPoolSize id, bool isAlive) : m_ID(id), m_IsAlive(false) {}

    Entity::Entity() {}

}