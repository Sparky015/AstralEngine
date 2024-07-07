//
// Created by Andrew Fagan on 4/24/24.
//

#include "LayerStack.h"

namespace Ayla::Core::Layers
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


    void LayerStack::InsertLayer(ILayer* layer, int position)
    {

    }


    void LayerStack::RemoveLayer(ILayer* layer)
    {
        // TODO: Iterate through vector, find matching layer pointer and remove from vector
    }


    void LayerStack::AppendOverlay(ILayer* layer)
    {
        m_OverlayLayers.push_back(layer);
    }


    void LayerStack::InsertOverlay(ILayer* layer, int position)
    {

    }


    void LayerStack::RemoveOverlay(ILayer* layer)
    {
        // TODO: Iterate through vector, find matching layer pointer and remove from vector
    }


    void LayerStack::DispatchEventBackToFront(Ayla::Events::IEvent& event)
    {
            for (ILayer* layer : m_Layers)
            {
                if (event.IsHandled()) {return;}
                if (!event.IsInCategory(layer->GetAcceptingEventFlags())) {continue;}
                layer->OnEvent(event);
            }
            for (ILayer* layer : m_OverlayLayers)
            {
                if (event.IsHandled()) {return;}
                if (!event.IsInCategory(layer->GetAcceptingEventFlags())) {continue;}
                layer->OnEvent(event);
            }
    }


    void LayerStack::DispatchEventFromFrontToBack(Ayla::Events::IEvent& event)
    {
        for (ILayer* layer : m_OverlayLayers)
        {
            if (event.IsHandled()) {return;}
            if (!event.IsInCategory(layer->GetAcceptingEventFlags())) {return;}
            layer->OnEvent(event);
        }
        for (ILayer* layer : m_Layers)
        {
            if (event.IsHandled()) {return;}
            if (!event.IsInCategory(layer->GetAcceptingEventFlags())) {return;}
            layer->OnEvent(event);
        }
    }



    void LayerStack::Update()
    {
        for (ILayer* layer : m_OverlayLayers)
        {
            layer->OnUpdate();
        }
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
        for (ILayer* layer : m_OverlayLayers)
        {
            layer->OnImGuiRender();
        }
    }


} // namespace Ayla::Core::Layers