//
// Created by Andrew Fagan on 10/31/24.
//

#pragma once

#include "Core/SystemUpdateEvent.h"
#include "Core/Events/EventListener.h"

class SystemManager
{
public:
    SystemManager();
    virtual ~SystemManager() = default;

private:
    virtual void Update();

    Event::EventListener<SystemUpdateEvent> m_UpdateListener;
};