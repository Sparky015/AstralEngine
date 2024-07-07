//
// Created by Andrew Fagan on 7/4/24.
//

#include "ExampleLayer.h"

#include "Ayla/Core/Core.h"
#include "imgui.h"


ExampleLayer::ExampleLayer(){
    AY_TRACE("ExampleLayer: Initializing Template Layer");
    this->AttachLayer();
    m_DebugName = "Template Layer";
}

ExampleLayer::~ExampleLayer(){
    this->DetachLayer();
}

void ExampleLayer::OnAttach() {
    m_IsEnabled = true;
}

void ExampleLayer::OnDetach() {
    m_IsEnabled = false;
}

void ExampleLayer::OnUpdate() {

}

void ExampleLayer::OnEvent(IEvent& event) {
    // Do Stuff
    //event.setIsHandled(x);
}

EventCategory ExampleLayer::GetAcceptingEventFlags() {
    //return flag category
    return None;
}

void ExampleLayer::OnImGuiRender() {
    ImGui::Begin("Test Example!");
    ImGui::Text("Hi hihih!");
    ImGui::End();
}

