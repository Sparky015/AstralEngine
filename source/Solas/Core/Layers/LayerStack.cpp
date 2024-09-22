//
// Created by Andrew Fagan on 4/24/24.
//

#include "LayerStack.h"

#include "Solas/aypch.h"

namespace Solas::Core::Layers
{

    LayerStack::LayerStack()
    {
        AY_TRACE("[Sholas] LayerStack: Initializing Layer Stack");
    }


    LayerStack::~LayerStack()
    {
        AY_TRACE("[Sholas] LayerStack: Destroying Layer Stack");
    }


    void LayerStack::AppendLayer(ILayer* layer)
    {
        m_Layers.push_back(layer);
    }


    void LayerStack::RemoveLayer(ILayer* layer)
    {
        m_Layers.erase(std::remove(m_Layers.begin(), m_Layers.end(), layer), m_Layers.end());
    }


    void LayerStack::DispatchEventBackToFront(Solas::EventManagement::IEvent& event)
    {
        for (ILayer* layer : m_Layers)
        {
            if (event.IsHandled()) {return;}
            if (!event.IsInCategory(layer->GetAcceptingEventFlags())) {continue;}
            layer->OnEvent(event);
        }
    }


    void LayerStack::DispatchEventFromFrontToBack(Solas::EventManagement::IEvent& event)
    {
        for (ILayer* layer : m_Layers)
        {
            if (event.IsHandled()) {return;}
            if (!event.IsInCategory(layer->GetAcceptingEventFlags())) {return;}
            layer->OnEvent(event);
        }
    }



    void LayerStack::Update()
    {
        for (ILayer* layer : m_Layers)
        {
            layer->OnUpdate();
        }
    }


    void LayerStack::RenderImGui()
    {
        for (ILayer* layer : m_Layers)
        {
            layer->OnImGuiRender();
        }
    }


} // namespace Solas::Core::Layers