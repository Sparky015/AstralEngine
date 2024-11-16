//
// Created by Andrew Fagan on 10/31/24.
//

#pragma once

#include "Core/SystemEvents.h"
#include "Core/Events/EventListener.h"
#include "Debug/DebugEvents.h"

class SystemManager
{
public:
    SystemManager();
    virtual ~SystemManager() = default;

    virtual void Init() = 0;
    virtual void Shutdown() = 0;
    virtual void Update() = 0;
    virtual void RenderImGui();

private:


    Event::EventListener<SubSystemUpdateEvent> m_UpdateListener;
    Event::EventListener<RenderImGuiEvent> m_RenderImGuiListener;
};