//
// Created by Andrew Fagan on 11/27/24.
//

#pragma once

#include "Core/FixedIntegerTypes.h"

namespace Astral {

    using EntityID = uint32;
    static constexpr EntityID NULL_ENTITY = 0;

    class Entity
    {
    public:
        Entity() = default;
        explicit Entity(EntityID id) : m_ID(id) {}

        [[nodiscard]] EntityID GetID() const { return m_ID; }

    private:

        EntityID m_ID{NULL_ENTITY};
    };

}