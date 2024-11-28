//
// Created by Andrew Fagan on 11/27/24.
//

#include "ECS.h"

namespace ECS {

    void ECS::ReserveMemory()
    {
        PROFILE_SCOPE();
        m_Entities.reserve(MAX_ENTITIES);
        m_Transform.reserve(MAX_ENTITIES);
    }

    void ECS::FreeMemory()
    {
        PROFILE_SCOPE();
        m_Entities.clear();
        m_Entities.shrink_to_fit();
        m_Transform.clear();
        m_Transform.shrink_to_fit();
    }

    Entity ECS::AddEntity()
    {
        ASSERT(m_EntityCounter != MAX_ENTITIES, "Max amount of entities reached! Can't add any more!");
        Entity entity = Entity(m_EntityCounter);
        m_EntityCounter++;
        return entity;
    }

    void ECS::RemoveEntity(Entity entity)
    {
        EntityPoolSize id = entity.GetID();

    }

}