//
// Created by Andrew Fagan on 4/24/24.
//

#pragma once
#include <Ayla/EventSystem/Event.h>

namespace Ayla {

    class Layer {
    public:
        Layer();
        ~Layer();

        void attachLayer();
        void detachLayer();
        void removeLayer();
        void setCallback(std::function<void(Event&)>);
        void onEvent(Event&);

        bool isEnabled();
        void enable();
        void disable();
    private:

        bool m_isEnabled;
        bool m_isInitializedInTower;
        std::function<void(Event&)> m_callback;
    };


    class ExampleLayer : public Layer {
    public:
        int getLayerType();
        int getEventCategoryFlags();
    private:
        int m_eventCategory = (InputCategory | ApplicationCategory);
    };


} // Ayla


