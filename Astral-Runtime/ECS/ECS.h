/**
* @file ECS.h
* @author Andrew Fagan
* @date 3/8/2025
*/

#pragma once

#include <stack>

#include "Entity.h"
#include "ComponentPoolSet.h"
#include "Components/SpriteComponent.h"
#include "Components/TransformComponent.h"
#include "Components/MeshComponent.h"
#include "ECSErrorHandling.h"

namespace Astral {

    constexpr EntityID MAX_ENTITIES = 255;

    class ECS
    {
    public:
        template <typename ComponentType>
        using ComponentView = std::vector<ComponentType>;

        ECS();
        ~ECS() = default;

        /**@brief Initializes the ECS and its component pools */
        void Init();

        /**@brief Shuts down the ECS */
        void Shutdown();

        /**@brief Creates an entity in the ECS and returns its instance */
        Entity CreateEntity(std::string_view debugName);

        /**@brief Deletes an entity from the ECS */
        void DeleteEntity(Entity entity);
        void DeleteEntity(EntityID entityID);

        /**@brief Checks if an entity is alive and in the ECS */
        bool IsEntityAlive(Entity entity);

        /**@brief Checks if an entity is alive and in the ECS */
        bool IsEntityAlive(EntityID entityID);

        /**@brief Gets the number of entities that are alive. */
        uint32 GetNumberOfActiveEntities();

        /**@brief Adds a component to an entity
         * @param entity The entity to add a component to
         * @param component The component being added to the entity */
        template <typename ComponentType>
        void AddComponent(Entity entity, const ComponentType& component);

        /**@brief Retrieves the requested component of an entity.
         * @param outComponent Populates with the requested component's data
         * @return ECS_SUCCESS if entity has the component and can return it or ECS_COMPONENT_NOT_PRESENT if the
         *         entity does not have the component. */
        template <typename ComponentType>
        [[nodiscard]] ECS_Result GetComponent(Entity entity, ComponentType& outComponent);

        /**@brief Removes the given component from the entity. Does nothing if the entity does not have the given component.
         * @param entity The entity handle to remove the component from. */
        template <typename ComponentType>
        void RemoveComponent(Entity entity);

        /**@brief Checks if an entity has the requested component.
         * @return True if the entity has the component and false otherwise. */
        template <typename ComponentType>
        bool HasComponent(Entity entity);

        /**@brief Gets a view of the components of the requested type that can be iterated over.
         * @return An iterable ComponentView of the requested type. */
        template <typename ComponentType>
        ComponentView<ComponentType>& GetView(); // TODO: Return an iterator of the view instead of actual view

        class Iterator
        {
        public:
            // Iterator Traits
            using value_type = Entity;
            using difference_type = std::ptrdiff_t;
            using pointer = Entity*;
            using reference = Entity&;
            using iterator_category = std::forward_iterator_tag;

            explicit Iterator(std::vector<bool>* entities, std::vector<std::string>& debugNames, EntityID startingIndex);

            value_type operator*() const;
            Iterator& operator++();
            Iterator operator++(int);
            bool operator==(const Iterator& other) const;
            bool operator!=(const Iterator& other) const;


        private:

            size_t FindNextAliveEntity();

            std::vector<bool>* m_Entities;
            std::vector<std::string>& m_EntityDebugNames;
            EntityID m_CurrentIndex;
        };

        Iterator begin() { return Iterator(&m_Entities, m_EntityDebugNames, 0); }
        Iterator end() { return Iterator(&m_Entities, m_EntityDebugNames, m_Entities.size()); }

    private:

        /**@brief Gets the next unused entity ID
         * @return The next unused entity ID */
        EntityID GetNextInactiveEntity();

        uint32 m_NumberOfActiveEntities;
        std::vector<bool> m_Entities;
        std::vector<std::string> m_EntityDebugNames;
        std::stack<EntityID, std::vector<EntityID>> m_FreeEntities;

        /// This ComponentPoolSet will have all the types registered listed here. If you need to add a component type,
        /// add it here.
        ComponentPoolSet<TransformComponent, SpriteComponent, MeshComponent> m_ComponentPoolSet;

    };

}

#include "ECS_Impl.h"