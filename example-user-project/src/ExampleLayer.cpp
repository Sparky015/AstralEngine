//
// Created by Andrew Fagan on 7/4/24.
//

#include "ExampleLayer.h"

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
//    ImGui::Begin("Test Example!");
//    ImGui::Text("Hi hihih!");
//    ImGui::End();
}


Solas::EventManagement::EEventCategory ExampleLayer::GetAcceptingEventFlags()
{
    return Solas::EventManagement::None;
}



