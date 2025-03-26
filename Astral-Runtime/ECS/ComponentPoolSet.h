/**
* @file ComponentPoolSet.h
* @author Andrew Fagan
* @date 3/8/2025
*/


#pragma once

#include <tuple>

#include "ComponentPool.h"

namespace Astral {

    /**@class ComponentPoolSet
     * @brief Holds all the component pools in the ECS */
    template <typename... ComponentTypes>
    class ComponentPoolSet
    {
    public:
        template <typename ComponentType>
        ComponentPool<ComponentType>& GetComponentPool();

        /**@brief Resizes the component pools to the given size number. Does nothing if the current pool size is greater than
         *        or equal to the given size.
         * @param size The size to resize the component pool to
         */
        void ResizeComponentPool(size_t size);

        /**@brief Resets all the components of the given entity to be inactive (not used).
         * @param entity The entity to reset all the components on. */
        void ResetEntityUsedComponentFlags(Entity entity);

    private:

        std::tuple<ComponentPool<ComponentTypes>...> m_ComponentPoolSet;
    };

}

#include "ComponentPoolSet_Impl.h"