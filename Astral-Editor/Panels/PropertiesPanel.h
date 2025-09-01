/**
* @file PropertiesPanel.h
* @author Andrew Fagan
* @date 6/23/2025
*/


#pragma once

#include "ECS/ECS.h"
#include "ECS/Entity.h"

namespace Astral {

    void PropertiesPanel();

    void ShowPointLightComponent(ECS& ecs, Entity& entity);
    void ShowDirectionalLightComponent(ECS& ecs, Entity& entity);

}
