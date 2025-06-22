/**
* @file Scene.h
* @author Andrew Fagan
* @date 6/20/2025
*/


#pragma once

#include "ECS/ECS.h"

namespace Astral {

    struct Scene
    {
        Scene() = default;
        ~Scene() = default;

        ECS ECS;
    };

}
