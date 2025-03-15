/**
* @file ComponentPoolSet_Impl.h
* @author Andrew Fagan
* @date 3/13/25
*/

#pragma once


namespace Astral {

    template<typename ... ComponentTypes>
    template<typename ComponentType>
    ComponentPool<ComponentType>& ComponentPoolSet<ComponentTypes...>::GetComponentPool()
    {
        return std::get<ComponentPool<ComponentType>>(m_ComponentPoolSet);
    }

}