//
// Created by Andrew Fagan on 11/27/24.
//

#pragma once

#include "ECSTypes.h"

namespace ECS {

    class Entity
    {
    public:
        explicit Entity(EntityPoolSize id);
        explicit Entity(EntityPoolSize id, bool isAlive);

        [[nodiscard]] inline EntityPoolSize GetID() const {return m_ID;}
        [[nodiscard]] inline bool IsAlive() const { return m_IsAlive; }
    private:
        bool m_IsAlive;
        EntityPoolSize m_ID;
    };

}