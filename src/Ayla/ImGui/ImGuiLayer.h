//
// Created by sparky on 4/24/2024.
//
#pragma once

#include "Ayla/Core/Layers/Layer.h"



namespace Ayla::GUI {
    using namespace Ayla::Events;
    using namespace Ayla::Core::Layers;

    class ImGuiLayer : public ILayer
    {
    public:
        ImGuiLayer();
        ~ImGuiLayer() override;

        void OnAttach() override;
        void OnDetach() override;
        void OnImGuiRender() override;
        static void Begin();
        static void End();
        void OnEvent(IEvent& event) override;
        EEventCategory GetAcceptingEventFlags() override;

    private:
        static float m_Time;
    };
}
