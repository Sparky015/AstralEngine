//
// Created by Andrew Fagan on 11/27/24.
//

#pragma once

#include "pch.h"

#include "ECS/Old/Entity.h"
#include "ECSTypes.h"
#include <any>
#include <bitset>
#include <typeindex>

#include "ECS/Old/Components/Sprite.h" // TEMP
#include "ECS/Old/Components/Transform.h" // TEMP


namespace ECS {

    constexpr EntityPoolSize MAX_ENTITIES = 255;

    class ECS
    {
    public:


        ECS();
        Entity AddEntity();
        void RemoveEntity(Entity entity);
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
            std::any_cast<ComponentDisplay<ComponentType>&>(m_ComponentsNew[typeid(ComponentType)])[entity].isUsed = false;

            // std::get<std::array<ComponentType, MAX_ENTITIES>>(m_Components)[entity.GetID()].isUsed = false;
        };


        template<class ComponentType>
        ComponentType& GetComponent(Entity entity)
        {
            return std::any_cast<ComponentDisplay<ComponentType>&>(m_ComponentsNew[typeid(ComponentType)])[entity];
            // return std::get<std::array<ComponentType, MAX_ENTITIES>>(m_Components)[entity.GetID()];
        }


        template<class ComponentType>
        using ComponentDisplay = std::array<ComponentType, MAX_ENTITIES>;

        template<class ComponentType>
        const std::array<ComponentType, MAX_ENTITIES>& GetComponentDisplay()
        {
            return std::any_cast<ComponentDisplay<ComponentType>&>(m_ComponentsNew[typeid(ComponentType)]);
            // return std::get<std::array<ComponentType, MAX_ENTITIES>>(m_Components);
        }


        template<class ComponentType>
        bool IsComponentUsed(const Entity entity, ComponentType component)
        {
            return std::any_cast<ComponentDisplay<ComponentType>&>(m_ComponentsNew[typeid(ComponentType)])[entity].isUsed;
            // return GetComponent<ComponentType>(entity).isUsed;
        }

        EntityPoolSize GetCurrentEntityCount() const;

    private:
        EntityPoolSize FindNextInactiveIndex();
        EntityPoolSize m_EntityCounter;
        std::bitset<MAX_ENTITIES> m_ActiveEntities;

        // Container for components
        // std::tuple<std::array<TransformComponent, MAX_ENTITIES>, std::array<SpriteComponent, MAX_ENTITIES>> m_Components;

        std::unordered_map<std::type_index, std::any> m_ComponentsNew;
    };

}