//
// Created by Andrew Fagan on 11/27/24.
//

#include "ECS.h"


namespace ECS {

    Entity ECS::AddEntity()
    {
        ASSERT(m_EntityCounter != MAX_ENTITIES, "Max amount of entities reached! Can't add any more!");

        // Find the next empty slot
        EntityPoolSize index = FindNextInactiveIndex();

        // Create new entity at the open slot and set it to active
        Entity entity = Entity(index);
        m_ActiveEntities[index] = true;
        m_EntityCounter++;

        return entity;
    }


    void ECS::RemoveEntity(Entity entity)
    {
        m_EntityCounter--;

        // Setting the entity slot to not being used
        EntityPoolSize entityID = entity.GetID();
        m_ActiveEntities[entityID] = false;

        // Sets all the possible components of an entity to not being used.
        RemoveComponent<TransformComponent>(entity);
        RemoveComponent<SpriteComponent>(entity);
    }


    bool ECS::IsEntityUsed(const Entity entity)
    {
        return entity.IsAlive();
    }


    bool ECS::IsEntityUsed(const EntityPoolSize entityID)
    {
        return m_ActiveEntities[entityID];
    }


    EntityPoolSize ECS::FindNextInactiveIndex()
    {
        // Iterate through the bitfield and find the first slot that isn't being used
        for (EntityPoolSize i = 0; i < MAX_ENTITIES; i++)
        {
            if (m_ActiveEntities[i] == false) { return i; }
        };

        // We did not find a empty slot since we did not return during the loop.
        ERROR("Max amount of entities reached! Can't add any more!");
    }


    ECS::ECS() : m_EntityCounter(0), m_Components(std::make_tuple(std::array<TransformComponent, MAX_ENTITIES>(), std::array<SpriteComponent, MAX_ENTITIES>()))
    {

    }

}