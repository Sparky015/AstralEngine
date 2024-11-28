//
// Created by Andrew Fagan on 11/27/24.
//

#pragma once

#include "pch.h"

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


        template<class T>
        void AddComponent(Entity entity, T component)
        {
            std::get<T>(m_Components)[entity.GetID()] = component;
            std::get<T>(m_Components)[entity.GetID()].isUsed = true;
        };

        template<class T>
        T& GetComponent(Entity entity)
        {
            return std::get<T>(m_Components);
        }

        template<class T>
        void RemoveComponent(Entity entity, T component)
        {
            std::get<T>(m_Components)[entity.GetID()].isUsed = false;
        };

        void ReserveMemory();
        void FreeMemory();

    private:
        EntityPoolSize m_EntityCounter;
        // container for entities
        std::vector<Entity> m_Entities;
        std::vector<bool> m_ActiveEntities;

        // container for components
        std::tuple<std::vector<TransformComponent>> m_Components;


    };

}