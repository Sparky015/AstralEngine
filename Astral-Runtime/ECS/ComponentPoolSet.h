/**
* @file ComponentPoolSet.h
* @author Andrew Fagan
* @date 3/8/2025
*/


#pragma once

#include <tuple>

#include "ComponentPool.h"

#define ASTRAL_ECS_REGISTER_COMPONENT_TYPE(componentType) // TODO: do something with it

namespace Astral {

    /**@struct ComponentPoolSet
     * @brief Holds all the component pools in the ECS */
    struct ComponentPoolSet
    {
        // int and unsigned int are placeholders for real component types
        std::tuple<ComponentPool<int>, ComponentPool<unsigned int>> componentPoolSet;
    };

}