//
// Created by Andrew Fagan on 4/24/24.
//

#pragma once
#include "Ayla/Events/Event.h"

using namespace Ayla::Events;

namespace Ayla::Core::Layers {

    class Layer {
    public:

        Layer();
        virtual ~Layer();

        virtual void onAttach() = 0;
        virtual void onDetach() = 0;
        virtual void onUpdate() = 0;
        virtual void onImGuiRender() = 0;
        virtual void onEvent(Event &event) = 0;
        virtual EventCategory getAcceptingEventFlags() = 0;

        void attachLayer();
        void attachOverlay();

        void detachLayer();
        void detachOverlay();

        void setCallback(std::function<void(Event&)> callback);
        bool isEnabled();
        void enable();
        void disable();



    protected:

        bool m_isEnabled;
        bool m_isInitializedInTower;
        std::function<void(Event & )> m_callback = [](Event&){};

        // For debug
        std::string m_debugName;

    };



}





