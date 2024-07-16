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

        /** Constructor/Destructor */
        ImGuiLayer();
        ~ImGuiLayer() override;

        /** Initializes ImGui and creates the ImGui context. */
        void OnAttach() override;

        /** Shuts down ImGui and destroys the context. */
        void OnDetach() override;

        /** ImGui render code that contains ImGui demo code and demo windows. */
        void OnImGuiRender() override;

        /** Sets up a new frame for ImGui to render. */
        static void Begin();

        /** Makes a render call with data from ImGui functions that were called after ImGuiLayer::Begin() was called. */
        static void End();

        /** The ImGuiLayer does not accept any events. */
        EEventCategory GetAcceptingEventFlags() override;

    private:
        static float m_Time;
    };
}
