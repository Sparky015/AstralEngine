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
        [size](const auto&... args)
        {
            (void)std::initializer_list<int>
            {
                (args.ResizePool(size))...
            };
        },
        m_ComponentPoolSet);
    }

}
