//
// Created by sparky on 4/24/2024.
//
#pragma once

#include <Ayla/Core/Layer.h>

namespace Ayla {

    class ImGuiLayer : public Layer {
    public:
        ~ImGuiLayer() override;

        void onAttach() override;

        void onDetach() override;

        void onUpdate() override;

    private:

    };
}
