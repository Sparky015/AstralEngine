//
// Created by Andrew Fagan on 6/25/24.
//

#include "TemplateLayer.h"

namespace Ayla {

    TemplateLayer::TemplateLayer(){
        AY_TRACE("[Sholas] TemplateLayer: Initializing Template Layer");
        this->attachLayer();
        m_debugName = "Template Layer";
    }

    TemplateLayer::~TemplateLayer(){
        this->detachLayer();
    }

    void TemplateLayer::onAttach() {
        m_isEnabled = true;
    }

    void TemplateLayer::onDetach() {
        m_isEnabled = false;
    }

    void TemplateLayer::onUpdate() {

    }

    void TemplateLayer::onEvent(Event& event) {
        // Do Stuff
        //event.setIsHandled(x);
    }

    EventCategory TemplateLayer::getAcceptingEventFlags() {
        //return flag category
    }

}