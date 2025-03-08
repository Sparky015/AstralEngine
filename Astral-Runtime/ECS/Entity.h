//
// Created by Andrew Fagan on 11/27/24.
//

#pragma once

#include "Core/FixedIntegerTypes.h"
#include "ECSErrorHandling.h"

namespace Astral {

    using EntityID = uint32;
    static constexpr EntityID NullEntity = 0;

    class Entity
    {
    public:
        Entity() = default;

        /** @brief Deletes the entity from the ECS */
        void Delete();

        /**@brief Checks if the entity is alive and in the ECS
         * @return True if the entity is alive and in the ECS and false otherwise */
        bool IsAlive();

        /**@brief Sets the component of the entity with the given component. Creates a component if
         * one is not already in use */
        template <typename ComponentType>
        void SetComponent(const ComponentType& componentType);

        /**@brief Retrieves the requested component of the entity.
         * @param outComponent Populates with the requested component's data
         * @return ECS_SUCCESS if entity has the component and can return it or ECS_COMPONENT_NOT_PRESENT if the
         *         entity does not have the component. */
        template <typename ComponentType>
        [[nodiscard]] ECS_Result GetComponent(ComponentType& outComponent);

        /**@brief Checks if the entity has the requested component.
         * @return True if the entity has the requested component and false otherwise. */
        template <typename ComponentType>
        bool HasComponent();

    private:

        EntityID m_ID{NullEntity};
    };

}

#include "Entity_Impl.h"
