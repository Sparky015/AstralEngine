//
// Created by Andrew Fagan on 7/4/24.
//

#include "ExampleLayer.h"

#include "Ayla/Core/Core.h"
#include "imgui.h"


ExampleLayer::ExampleLayer(){
    AY_TRACE("ExampleLayer: Initializing Template Layer");
    this->attachLayer();
    m_debugName = "Template Layer";
}

ExampleLayer::~ExampleLayer(){
    this->detachLayer();
}

void ExampleLayer::onAttach() {
    m_isEnabled = true;
}

void ExampleLayer::onDetach() {
    m_isEnabled = false;
}

void ExampleLayer::onUpdate() {

}

void ExampleLayer::onEvent(Event& event) {
    // Do Stuff
    //event.setIsHandled(x);
}

EventCategory ExampleLayer::getAcceptingEventFlags() {
    //return flag category
    return None;
}

void ExampleLayer::onImGuiRender() {
    ImGui::Begin("Test Example");
    ImGui::Text("Hi hihih !");
    ImGui::End();
}

