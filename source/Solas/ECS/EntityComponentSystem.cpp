//
// Created by Andrew Fagan on 7/23/24.
//

#include "EntityComponentSystem.h"

namespace Solas::ECS{

    EntitySystem* EntitySystem::m_Instance = nullptr;


    void EntitySystem::Init()
    {
        AY_TRACE("[Sholas] EntitySystem: Initializing EntitySystem");
        AY_ASSERT(m_Instance == nullptr, "ECS has already been created!");
        m_Instance = new EntitySystem();
    }


    EntitySystem& EntitySystem::Get()
    {
        AY_ASSERT(m_Instance != nullptr, "ECS has not been initialized!");
        return *m_Instance;
    }


    void EntitySystem::Destroy()
    {
        AY_TRACE("[Sholas] EntitySystem: Destroying EntitySystem");
        delete m_Instance;
        m_Instance = nullptr;
    }


    EntitySystem::EntitySystem()
    {
        m_EntityPool.fill(Entity());
    }


    uint8 EntitySystem::AddEntity(Entity entity)
    {
        m_EntityPool[m_HandleIDTracker] = entity;
        return m_HandleIDTracker++;
    }


    /** uint8 will always fit within the array size of 256 so no bounds check is necessary. */
    Entity& EntitySystem::EditEntity(const uint8 handle)
    {
        return m_EntityPool[handle];
    }


    /** uint8 will always fit within the array size of 256 so no bounds check is necessary. */
    const Entity& EntitySystem::ReadEntity(uint8_t handle) const
    {
        return m_EntityPool[handle];
    }


    /** uint8 will always fit within the array size of 256 so no bounds check is necessary. */
    void EntitySystem::RemoveEntity(uint8_t handle)
    {
        /** Overwrites the entity at the index. */
        m_EntityPool[handle] = Entity();
    }




} // namespace Solas::ECS

