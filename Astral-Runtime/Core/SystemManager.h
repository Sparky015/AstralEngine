//
// Created by Andrew Fagan on 10/31/24.
//

#pragma once

#include "Core/EngineLoopEvents.h"
#include "Core/Events/EventListener.h"
#include "Debug/ImGui/ImGuiEvents.h"

class SystemManager
{
public:
    SystemManager();
    virtual ~SystemManager() = default;

    virtual void Init() = 0;
    virtual void Shutdown() = 0;
    virtual void Update();
    virtual void RenderImGui(); // Override to display debug info

protected:

    Astral::EventListener<SubSystemUpdateEvent> m_UpdateListener;
    Astral::EventListener<RenderImGuiEvent> m_RenderImGuiListener;
};