//
// Created by Andrew Fagan on 11/27/24.
//

#include "Entity.h"

namespace ECS {

    Entity::Entity(EntityPoolSize id) : m_IsAlive(true), m_ID(id) {}

    Entity::Entity(EntityPoolSize id, bool isAlive) : m_IsAlive(false), m_ID(id) {}

    Entity::Entity() {}

}