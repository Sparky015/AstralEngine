//
// Created by Andrew Fagan on 11/27/24.
//

#pragma once

#include "ECSTypes.h"

namespace ECS {

    class Entity
    {
    public:
        Entity() = default;
        explicit Entity(EntityPoolSize id);

        [[nodiscard]] inline EntityPoolSize GetID() const {return m_ID;}

        constexpr operator EntityPoolSize() const { return m_ID; }
    private:
        EntityPoolSize m_ID{};
    };

}