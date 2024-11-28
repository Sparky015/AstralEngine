//
// Created by Andrew Fagan on 11/27/24.
//

#include "ECS.h"

namespace ECS {

    void ECS::ReserveMemory()
    {
        PROFILE_SCOPE();
        m_Entities.reserve(MAX_ENTITIES);
        m_ActiveEntities.reserve(MAX_ENTITIES);
        //TODO: Reserve space for each component.
    }

    void ECS::FreeMemory()
    {
        PROFILE_SCOPE();
        m_Entities.clear();
        m_Entities.shrink_to_fit();
//        m_Transform.clear();
//        m_Transform.shrink_to_fit();
//        for (auto c : m_Components)
//        {
//
//        }

    }

    Entity ECS::AddEntity()
    {
        ASSERT(m_EntityCounter != MAX_ENTITIES, "Max amount of entities reached! Can't add any more!");

        // Find the next empty slot
        auto iterator = std::find(m_ActiveEntities.begin(), m_ActiveEntities.end(), false);
        EntityPoolSize index = std::distance(m_ActiveEntities.begin(), iterator);

        // Create new entity at the open slot and set it to active
        Entity entity = Entity(index);
        m_ActiveEntities[index] = true;
        m_EntityCounter++;

        return entity;
    }

    void ECS::RemoveEntity(Entity entity)
    {
        // Set the slot to being not active
        EntityPoolSize id = entity.GetID();
        m_ActiveEntities[id] = false;
        m_EntityCounter--;
    }

}