//
// Created by Andrew Fagan on 11/27/24.
//

#pragma once

#include "pch.h"

#include "ECS/Components/Transform.h"
#include "ECS/Components/Sprite.h"
#include "ECS/Entity.h"
#include "ECS/ECSTypes.h"

namespace ECS {

    const EntityPoolSize MAX_ENTITIES = 255;

    class ECS
    {
    public:
        ECS();
        Entity AddEntity();
        void RemoveEntity(Entity entity);

        template<class T>
        void AddComponent(Entity entity, T component)
        {
            GetComponent<T>(entity) = component;
            GetComponent<T>(entity).isUsed = true;
        };

        template<class T>
        T& GetComponent(Entity entity)
        {
            return std::get<std::array<T, MAX_ENTITIES>>(m_Components)[entity.GetID()];
        }

        template<class ComponentType>
        void RemoveComponent(Entity entity)
        {
            std::get<std::array<ComponentType, MAX_ENTITIES>>(m_Components)[entity.GetID()].isUsed = false;
        };

        template<class ComponentType>
        using ComponentDisplay = std::array<ComponentType, MAX_ENTITIES>;

        template<class ComponentType>
        const std::array<ComponentType, MAX_ENTITIES>& GetComponentDisplay()
        {
            return std::get<std::array<ComponentType, MAX_ENTITIES>>(m_Components);
        }

    private:
        EntityPoolSize FindNextInactiveIndex();

    private:
        EntityPoolSize m_EntityCounter;
        // container for entities
        std::bitset<MAX_ENTITIES> m_ActiveEntities;

        // Container for components
        std::tuple<std::array<TransformComponent, MAX_ENTITIES>, std::array<SpriteComponent, MAX_ENTITIES>> m_Components;

    };

}