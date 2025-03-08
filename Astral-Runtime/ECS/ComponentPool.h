/**
* @file ComponentPool.h
* @author Andrew Fagan
* @date 3/8/2025
*/


#pragma once

#include <vector>

namespace Astral {

    /**@struct ComponentPool
     * @brief Contains all the data for one component type in the ECS */
    template <typename ComponentType>
    struct ComponentPool
    {
        std::vector<ComponentType> componentData;
        std::vector<bool> entityRegistry; // Should behave like dynamic bitset because this is a vector specialization
    };

}