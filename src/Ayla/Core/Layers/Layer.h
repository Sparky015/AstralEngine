//
// Created by Andrew Fagan on 4/24/24.
//

#pragma once
#include "Ayla/Events/Event.h"

namespace Ayla {


    class Layer {
    public:

        Layer();
        virtual ~Layer();

        void attachLayer();
        void attachOverlay();
        virtual void onAttach() = 0;

        void detachLayer();
        void detachOverlay();
        virtual void onDetach() = 0;

        virtual void onUpdate() = 0;

        void setCallback(std::function<void(Event&)> callback);
        virtual void onEvent(Event &event) = 0;
        virtual EventCategory getAcceptingEventFlags() = 0;

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



} // Ayla


