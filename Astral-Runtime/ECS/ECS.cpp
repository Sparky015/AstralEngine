/**
* @file ECS.cpp
* @author Andrew Fagan
* @date 3/8/2025
*/

#include "ECS.h"

#include "Entity.h"

namespace Astral {

    ECS::ECS() : m_NumberOfActiveEntities(0)
    {

    }


    void ECS::Init()
    {
        constexpr size_t INITIAL_ENTITY_COUNT = 64;
        m_ActiveEntities.resize(INITIAL_ENTITY_COUNT);
        m_ComponentPoolSet.ResizeComponentPool(INITIAL_ENTITY_COUNT);
    }


    void ECS::Shutdown()
    {

    }


    Entity ECS::CreateEntity()
    {
        if (m_NumberOfActiveEntities == m_ActiveEntities.size())
        {
            // Resize all the component pools
            const size_t size = m_ActiveEntities.size();
            m_ActiveEntities.resize(2 * size, false);
            m_ComponentPoolSet.ResizeComponentPool(2 * size);
        }

        EntityID newEntityID = GetNextInactiveEntity();

        // This should not go out of bounds due to the resizing above if m_ActiveEntities is at max capacity.
        m_ActiveEntities[newEntityID] = true;

        m_NumberOfActiveEntities++;
        return Entity(newEntityID);
    }


    void ECS::DeleteEntity(Entity entity)
    {
        m_NumberOfActiveEntities--;
        m_ActiveEntities[entity.GetID()] = false;
        m_FreeEntities.push(entity.GetID());
        m_ComponentPoolSet.ResetEntityUsedComponentFlags(entity);
    }


    void ECS::DeleteEntity(EntityID entityID)
    {
       DeleteEntity(Entity(entityID));
    }


    bool ECS::IsEntityAlive(Entity entity)
    {
        return m_ActiveEntities[entity.GetID()];
    }


    bool ECS::IsEntityAlive(EntityID entityID)
    {
        return m_ActiveEntities[entityID];
    }


    uint32 ECS::GetNumberOfActiveEntities()
    {
        return m_NumberOfActiveEntities;
    }


    EntityID ECS::GetNextInactiveEntity()
    {
        if (!m_FreeEntities.empty())
        {
            EntityID selectedEntityID = m_FreeEntities.top();
            m_FreeEntities.pop();
            return selectedEntityID;
        }

        return m_NumberOfActiveEntities; // This is the entity ID
    }

}


