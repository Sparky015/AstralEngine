/**
* @file ECS_Impl.h
* @author Andrew Fagan
* @date 3/8/2025
*/


#pragma once

namespace Astral {

    template<typename ComponentType>
    void ECS::SetEntityComponent(Entity entity, const ComponentType& component)
    {

    }


    template<typename ComponentType>
    ECS_Result ECS::GetEntityComponent(Entity entity, ComponentType& outComponent)
    {
        return ECS_Result::ECS_UNKNOWN_ERROR;
    }


    template<typename ComponentType>
    bool ECS::HasComponent(Entity entity)
    {
        return false;
    }


    template<typename ComponentType>
    ECS::ComponentView<ComponentType>& ECS::GetView()
    {
        static ECS::ComponentView<ComponentType> e;
        return e; // TEMP
    }

}