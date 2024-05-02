//
// Created by sparky on 4/24/2024.
//
#pragma once

#include <Ayla/Core/Layer.h>

namespace Ayla {

    class ImGuiLayer : public Layer {
    public:
        ImGuiLayer();
        ~ImGuiLayer() override;

        void onAttach() override;
        void onDetach() override;
        void onUpdate() override;
        void onEvent(Event&) override;

    private:
        float m_time = 0.0f;
    };
}
