//
// Created by sparky on 4/16/2024.
//

#include <Ayla.h>
#include "imgui.h"

class Game : public Ayla::Core::Application {
public:
    Game()  = default;
    ~Game() = default;
    
};



class ExampleLayer : public Layer {
public:
    ExampleLayer();
    ~ExampleLayer();

    virtual void onAttach() override;
    virtual void onDetach() override;
    virtual void onUpdate() override;
    virtual void onImGuiRender() override;
    virtual void onEvent(Event &event) override;
    virtual EventCategory getAcceptingEventFlags() override;
};

void ExampleLayer::onAttach() {

}

void ExampleLayer::onDetach() {

}

void ExampleLayer::onUpdate() {

}

void ExampleLayer::onImGuiRender() {
    ImGui::Begin("Example");
    ImGui::Text("hiiii!");
    ImGui::End();
}

void ExampleLayer::onEvent(Event &event) {

}

EventCategory ExampleLayer::getAcceptingEventFlags() {
    return ApplicationCategory;
}



ExampleLayer::ExampleLayer() {
    AY_TRACE("ExampleLayer: Initializing Example Client Layer");
    this->attachLayer();
    m_debugName = "Example Client Layer";
}

ExampleLayer::~ExampleLayer() {
    this->detachLayer();
}



Ayla::Core::Application* Ayla::Core::CreateApplication() {
//    Ayla::Core::clientCallbackFun = [](){
//        ExampleLayer el = ExampleLayer();
//
//    };
    return new Game();
}


//TODO: Organize the engine set up for the user backend
/**
 * Probably use a init function that can be defined as a global static function in the Ayla::Core namespace which will be
 * called by Application's init.
 *
 * Have that init function pass a function signature for an update function
 *
 * OR (Option B)
 * Have a virtual function that can be called by the application for the init function and update function
 *
 * (Probably B because it is easy to understand and is intuitive)
 */
//TODO: Provide an onUpdate of sorts to the client executable
//TODO: Make an example layer on the heap