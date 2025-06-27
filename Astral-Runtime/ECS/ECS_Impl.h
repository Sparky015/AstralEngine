/**
* @file ECS_Impl.h
* @author Andrew Fagan
* @date 3/8/2025
*/


#pragma once

#include "Entity.h"

namespace Astral {

    template<typename ComponentType>
    void ECS::AddComponent(Entity entity, const ComponentType& component)
    {
        if (entity.GetID() == NULL_ENTITY) { WARN("Trying to add component to null entity!"); return; }
        m_ComponentPoolSet.GetComponentPool<ComponentType>().componentData[entity.GetID()] = component;
        m_ComponentPoolSet.GetComponentPool<ComponentType>().entityRegistry[entity.GetID()] = true;
    }


    template<typename ComponentType>
    void ECS::UpdateComponent(Entity entity, const ComponentType& component)
    {
        if (entity.GetID() == NULL_ENTITY) { WARN("Trying to update component of null entity!"); return; }
        m_ComponentPoolSet.GetComponentPool<ComponentType>().componentData[entity.GetID()] = component;
        m_ComponentPoolSet.GetComponentPool<ComponentType>().entityRegistry[entity.GetID()] = true;
    }


    template<typename ComponentType>
    ECS_Result ECS::GetComponent(Entity entity, ComponentType& outComponent)
    {
        if (!HasComponent<ComponentType>(entity)) { return ECS_Result::ECS_COMPONENT_NOT_PRESENT; }
        outComponent = m_ComponentPoolSet.GetComponentPool<ComponentType>().componentData[entity.GetID()];
        return ECS_Result::ECS_SUCCESS;
    }


    template<typename ComponentType>
    void ECS::RemoveComponent(Entity entity)
    {
        if (entity.GetID() == NULL_ENTITY) { return; }
        m_ComponentPoolSet.GetComponentPool<ComponentType>().entityRegistry = false;
    }


    template<typename ComponentType>
    bool ECS::HasComponent(Entity entity)
    {
        if (entity.GetID() == NULL_ENTITY) { return false; }
        return m_ComponentPoolSet.GetComponentPool<ComponentType>().entityRegistry[entity.GetID()];
    }


    template<typename ComponentType>
    ECS::ComponentView<ComponentType>& ECS::GetView()
    {
        return m_ComponentPoolSet.GetComponentPool<ComponentType>().componentData;
    }

}