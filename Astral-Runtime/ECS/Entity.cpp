//
// Created by Andrew Fagan on 11/27/24.
//

#include "Entity.h"

#include "ECSManager.h"

namespace Astral {

    void Entity::Delete() const
    {
        ECS& ecs = g_ECSManager.GetECS();
        ecs.DeleteEntity(m_ID);
    }

    bool Entity::IsAlive() const
    {
        ECS& ecs = g_ECSManager.GetECS();
        return ecs.IsEntityAlive(m_ID);
    }


}


