/**
* @file ECS.h
* @author Andrew Fagan
* @date 3/8/2025
*/

#pragma once

#include "Entity.h"
#include "ComponentPoolSet.h"
#include "Components/Sprite.h"
#include "Components/Transform.h"
#include "ECSErrorHandling.h"

#include <vector>


namespace Astral {

    constexpr uint32 MAX_ENTITIES = 255;

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
        Entity CreateEntity();

        /**@brief Deletes an entity from the ECS */
        void DeleteEntity(Entity entity);
        void DeleteEntity(EntityID entityID);

        /**@brief Checks if an entity is alive and in the ECS */
        bool IsEntityAlive(Entity entity);

        /**@brief Checks if an entity is alive and in the ECS */
        bool IsEntityAlive(EntityID entityID);

        /**@brief Gets the number of entities that are alive. */
        uint32 GetNumberOfActiveEntities();

        template <typename ComponentType>
        void SetEntityComponent(Entity entity, const ComponentType& component);

        /**@brief Retrieves the requested component of the entity.
         * @param outComponent Populates with the requested component's data
         * @return ECS_SUCCESS if entity has the component and can return it or ECS_COMPONENT_NOT_PRESENT if the
         *         entity does not have the component. */
        template <typename ComponentType>
        [[nodiscard]] ECS_Result GetEntityComponent(Entity entity, ComponentType& outComponent);

        /**@brief Checks if an entity has the requested component.
         * @return True if the entity has the component and false otherwise. */
        template <typename ComponentType>
        bool HasComponent(Entity entity);

        /**@brief Gets a view of the components of the requested type that can be iterated over.
         * @return An iterable ComponentView of the requested type. */
        template <typename ComponentType>
        ComponentView<ComponentType>& GetView();

    private:

        EntityID GetNextInactiveEntity();

        uint32 m_NumberOfActiveEntities;
        std::vector<bool> m_ActiveEntities;
        std::stack<EntityID, std::vector<EntityID>> m_FreeEntities;

        /// This ComponentPoolSet will have all the types registered listed here. If you need to add a component type,
        /// add it here.
        ComponentPoolSet<TransformComponent, SpriteComponent> m_ComponentPoolSet;

    };

}

#include "ECS_Impl.h"