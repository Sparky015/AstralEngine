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

    private:

        std::tuple<ComponentPool<ComponentTypes>...> m_ComponentPoolSet;
    };

}

#include "ComponentPoolSet_Impl.h"