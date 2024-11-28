//
// Created by Andrew Fagan on 11/27/24.
//

#pragma once

#include "ECS/Components/Transform.h"
#include "ECS/Entity.h"
#include "ECS/ECSTypes.h"

namespace ECS {


    const EntityPoolSize MAX_ENTITIES = 255;

    class ECS
    {
    public:
        Entity AddEntity();
        void RemoveEntity(Entity entity);

        void ReserveMemory();
        void FreeMemory();

    private:
        EntityPoolSize m_EntityCounter;
        // container for entities
        std::vector<Entity> m_Entities;

        // container for components
        std::vector<TransformComponent> m_Transform;
    };

}