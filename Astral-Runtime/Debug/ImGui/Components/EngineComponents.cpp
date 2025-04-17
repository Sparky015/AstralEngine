/**
* @file EngineComponents.cpp
* @author Andrew Fagan
* @date 1/12/2025
*/

#include "EngineComponents.h"

#include "Core/Engine.h"
#include "imgui.h"

namespace Debug {

    void EngineTimeComponent()
    {
        ImGui::Text("Engine Running Time: %.3f seconds", Astral::Engine::Get().GetTimeSinceInitialization());
    }

}