/**
* @file ECS.cpp
* @author Andrew Fagan
* @date 3/8/2025
*/

#include "ECS.h"

namespace Astral {

    void ECS::Init()
    {

    }


    void ECS::Shutdown()
    {

    }


    Entity ECS::CreateEntity()
    {
        static Entity e;
        return e; // TEMP
    }


    void ECS::DeleteEntity(Entity entity)
    {

    }


    bool ECS::IsEntityAlive(Entity entity)
    {
        return false;
    }

}


