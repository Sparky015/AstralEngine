//
// Created by Andrew Fagan on 11/27/24.
//

#pragma once

#include "pch.h"

#include "ECS/Entity.h"
#include "ECS/ECSTypes.h"
#include <bitset>

#include "ECS/Components/Transform.h" // TEMP
#include "ECS/Components/Sprite.h" // TEMP


namespace ECS {

    const EntityPoolSize MAX_ENTITIES = 255;

    class ECS
    {
    public:
        ECS();
        Entity AddEntity();
        void RemoveEntity(Entity entity);
        bool IsEntityUsed(const Entity entity);
        bool IsEntityUsed(const EntityPoolSize entityID);


        template<class ComponentType>
        void AddComponent(Entity entity, const ComponentType& component)
        {
            GetComponent<ComponentType>(entity) = component;
            GetComponent<ComponentType>(entity).isUsed = true;
        };


        template<class ComponentType>
        void UpdateComponent(Entity entity, const ComponentType& component)
        {
            GetComponent<ComponentType>(entity) = component;
        };


        template<class ComponentType>
        void RemoveComponent(Entity entity)
        {
            std::get<std::array<ComponentType, MAX_ENTITIES>>(m_Components)[entity.GetID()].isUsed = false;
        };


        template<class ComponentType>
        ComponentType& GetComponent(Entity entity)
        {
            return std::get<std::array<ComponentType, MAX_ENTITIES>>(m_Components)[entity.GetID()];
        }


        template<class ComponentType>
        using ComponentDisplay = std::array<ComponentType, MAX_ENTITIES>;

        template<class ComponentType>
        const std::array<ComponentType, MAX_ENTITIES>& GetComponentDisplay()
        {
            return std::get<std::array<ComponentType, MAX_ENTITIES>>(m_Components);
        }


        template<class ComponentType>
        bool IsComponentUsed(const Entity entity, ComponentType component)
        {
            return GetComponent<ComponentType>(entity).isUsed;
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