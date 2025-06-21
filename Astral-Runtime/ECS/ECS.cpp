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
        m_Entities.resize(INITIAL_ENTITY_COUNT, false);
        m_EntityDebugNames.resize(INITIAL_ENTITY_COUNT);
        m_ComponentPoolSet.ResizeComponentPool(INITIAL_ENTITY_COUNT);
    }


    void ECS::Shutdown()
    {

    }


    Entity ECS::CreateEntity(std::string_view debugName)
    {
        if (m_NumberOfActiveEntities == m_Entities.size())
        {
            // Resize all the component pools
            const size_t size = m_Entities.size();
            m_Entities.resize(2 * size, false);
            m_EntityDebugNames.resize(2 * size);
            m_ComponentPoolSet.ResizeComponentPool(2 * size);
        }

        EntityID newEntityID = GetNextInactiveEntity();

        // This should not go out of bounds due to the resizing above if m_ActiveEntities is at max capacity.
        m_Entities[newEntityID] = true;
        m_EntityDebugNames[newEntityID] = debugName;

        m_NumberOfActiveEntities++;
        return Entity(newEntityID, debugName);
    }


    void ECS::DeleteEntity(Entity entity)
    {
        m_NumberOfActiveEntities--;
        m_Entities[entity.GetID()] = false;
        m_FreeEntities.push(entity.GetID());
        m_ComponentPoolSet.ResetEntityUsedComponentFlags(entity);
    }


    void ECS::DeleteEntity(EntityID entityID)
    {
       DeleteEntity(Entity(entityID));
    }


    bool ECS::IsEntityAlive(Entity entity)
    {
        return m_Entities[entity.GetID()];
    }


    bool ECS::IsEntityAlive(EntityID entityID)
    {
        return m_Entities[entityID];
    }


    uint32 ECS::GetNumberOfActiveEntities()
    {
        return m_NumberOfActiveEntities;
    }


    ECS::Iterator::Iterator(std::vector<bool>* entities, std::vector<std::string>& debugNames, EntityID startingIndex) :
        m_Entities(entities),
        m_EntityDebugNames(debugNames),
        m_CurrentIndex(startingIndex - 1)
    {
        ASSERT(entities != nullptr, "Iterator was passed a null entities container!")
        FindNextAliveEntity();
    }


    ECS::Iterator::value_type ECS::Iterator::operator*() const
    {
        ASSERT(m_CurrentIndex != m_Entities->size(), "Tried to dereference a iterator pointing to end!")
        return Entity(m_CurrentIndex, m_EntityDebugNames[m_CurrentIndex]);
    }


    ECS::Iterator& ECS::Iterator::operator++()
    {
        FindNextAliveEntity();
        return *this;
    }


    ECS::Iterator ECS::Iterator::operator++(int)
    {
        Iterator temp = *this;
        FindNextAliveEntity();
        return temp;
    }


    bool ECS::Iterator::operator==(const Iterator& other) const
    {
        return m_CurrentIndex == other.m_CurrentIndex && m_Entities == other.m_Entities;
    }


    bool ECS::Iterator::operator!=(const Iterator& other) const
    {
        return !(*this == other);
    }


    size_t ECS::Iterator::FindNextAliveEntity()
    {
        m_CurrentIndex++;
        if (m_CurrentIndex > m_Entities->size()) { m_CurrentIndex = m_Entities->size(); }

        while (m_CurrentIndex < m_Entities->size() && m_Entities->at(m_CurrentIndex) == false)
        {
            m_CurrentIndex++;
        }

        return m_CurrentIndex;
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


