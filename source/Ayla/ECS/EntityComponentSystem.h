//
// Created by Andrew Fagan on 7/23/24.
//

#pragma once

#include "Entity.h"

//TEMP
namespace Ayla::ECS {

    class EntitySystem
    {
    public:

        static void Init();
        static EntitySystem& Get();
        static void Destroy();

        ~EntitySystem() = default;

        /** Adds an entity to the ECS and returns a handle back. */
        uint8 AddEntity(Entity entity);

        /** Edit the data of an entity. */
        Entity& EditEntity(uint8 handle);

        /** Read the data from an entity. */
        [[nodiscard]] const Entity& ReadEntity(uint8 handle) const;

        /** Removes an entity from the ECS. */
        void RemoveEntity(uint8 handle);

        EntitySystem(const EntitySystem&) = delete;
        EntitySystem& operator=(const EntitySystem&) = delete;
        EntitySystem(EntitySystem&&) = delete;
        EntitySystem& operator=(EntitySystem&&) = delete;

    private:
        EntitySystem();
        static EntitySystem* m_Instance;

        std::array<Entity, 256> m_EntityPool; // TEMP now for testing
        uint8 m_HandleIDTracker{0};

    };


} // namespace Ayla::ECS