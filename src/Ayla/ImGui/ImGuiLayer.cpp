//
// Created by sparky on 4/24/2024.
//

#include "ImGuiLayer.h"
#include "imgui.h"

namespace Ayla{

    ImGuiLayer::ImGuiLayer() {
        this->attachOverlay();
    }

    ImGuiLayer::~ImGuiLayer() {
        this->detachOverlay();
    }

    void ImGuiLayer::onAttach() {

    }

    void ImGuiLayer::onDetach() {

    }

    void ImGuiLayer::onUpdate() {

    }

    void ImGuiLayer::onEvent(Event& event) {

    }

}