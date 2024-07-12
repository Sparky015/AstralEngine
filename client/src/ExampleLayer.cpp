//
// Created by Andrew Fagan on 7/4/24.
//

#include "ExampleLayer.h"
#include "Ayla/Core/Core.h"
#include "imgui.h"


ExampleLayer::ExampleLayer()
{
    AY_TRACE("ExampleLayer: Initializing Template Layer");
    this->AttachLayer();
    m_DebugName = "Template Layer";
}


ExampleLayer::~ExampleLayer()
{
    this->DetachLayer();
}


void ExampleLayer::OnImGuiRender()
{
    ImGui::Begin("Test Example!");
    ImGui::Text("Hi hihih!");
    ImGui::End();
}


Ayla::Events::EEventCategory ExampleLayer::GetAcceptingEventFlags()
{
    return Ayla::Events::None;
}



