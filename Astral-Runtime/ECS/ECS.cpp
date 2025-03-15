/**
* @file ECS.cpp
* @author Andrew Fagan
* @date 3/8/2025
*/

#include "ECS.h"

namespace Astral {

    ECS::ECS() : m_NumberOfActiveEntities(0)
    {

    }

    void ECS::Init()
    {
        m_ActiveEntities.reserve(64);

    }


    void ECS::Shutdown()
    {

    }


    Entity ECS::CreateEntity()
    {
        EntityID newEntityID = GetNextInactiveEntity();
        m_ActiveEntities[newEntityID] = true;
        m_NumberOfActiveEntities++;
        return Entity(newEntityID);
    }


    void ECS::DeleteEntity(Entity entity)
    {
        m_NumberOfActiveEntities--;
        m_ActiveEntities[entity.GetID()] = false;
    }


    void ECS::DeleteEntity(EntityID entityID)
    {
        m_NumberOfActiveEntities--;
        m_ActiveEntities[entityID] = false;
    }


    bool ECS::IsEntityAlive(Entity entity)
    {
        return m_ActiveEntities[entity.GetID()];
    }


    bool ECS::IsEntityAlive(EntityID entityID)
    {
        return m_ActiveEntities[entityID];
    }


    EntityID ECS::GetNextInactiveEntity()
    {
        // Iterate through the bitfield and find the first slot that isn't being used
        for (EntityID i = 0; i < MAX_ENTITIES; i++)
        {
            if (m_ActiveEntities[i] == false) { return i; }
        };

        // We did not find a empty slot since we did not return during the loop.
        return std::numeric_limits<EntityID>::max();
    }

}


