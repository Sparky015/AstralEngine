//
// Created by sparky on 4/24/2024.
//
#pragma once

#include "Ayla/Core/Layers/Layer.h"

using namespace Ayla::Events;
using namespace Ayla::Core::Layers;

namespace Ayla::GUI {

    class ImGuiLayer : public ILayer {
    public:
        ImGuiLayer();
        ~ImGuiLayer() override;

        void OnAttach() override;
        void OnDetach() override;
        void OnUpdate() override;
        void OnImGuiRender() override;
        static void begin();
        static void end();
        void OnEvent(IEvent&) override;
        EventCategory GetAcceptingEventFlags() override;

    private:
        static float m_time;
    };
}
