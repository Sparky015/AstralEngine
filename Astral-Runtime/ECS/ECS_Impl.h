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
        m_ComponentPoolSet.GetComponentPool<ComponentType>().componentData[entity.GetID()] = component;
    }


    template<typename ComponentType>
    ECS_Result ECS::GetEntityComponent(Entity entity, ComponentType& outComponent)
    {
        if (!HasComponent<ComponentType>(entity)) { return ECS_Result::ECS_COMPONENT_NOT_PRESENT; }
        outComponent = m_ComponentPoolSet.GetComponentPool<ComponentType>().componentData[entity.GetID()];
        return ECS_Result::ECS_SUCCESS;
    }


    template<typename ComponentType>
    bool ECS::HasComponent(Entity entity)
    {
        return m_ComponentPoolSet.GetComponentPool<ComponentType>().entityRegistry[entity.GetID()];
    }


    template<typename ComponentType>
    ECS::ComponentView<ComponentType>& ECS::GetView()
    {
        return m_ComponentPoolSet.GetComponentPool<ComponentType>().componentData;
    }

}