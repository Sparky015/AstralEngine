/**
* @file EntityImpl.h
* @author Andrew Fagan
* @date 3/8/2025
*/


#pragma once

namespace Astral
{
    template<typename ComponentType>
    void Entity::SetComponent(const ComponentType& componentType)
    {

    }


    template <typename ComponentType>
    [[nodiscard]] ECS_Result GetComponent(ComponentType& outComponent)
    {
        return ECS_Result::ECS_UNKNOWN_ERROR;
    }


    template<typename ComponentType>
    bool Entity::HasComponent()
    {
        return false;
    }

}