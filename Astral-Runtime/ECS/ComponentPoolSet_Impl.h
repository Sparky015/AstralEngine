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

    template<typename ... ComponentTypes>
    void ComponentPoolSet<ComponentTypes...>::ResizeComponentPool(size_t size)
    {
        std::apply(
        [size](auto&... args)
        {
            (void)(args.ResizePool(size), ...);
        },
        m_ComponentPoolSet);
    }

}
