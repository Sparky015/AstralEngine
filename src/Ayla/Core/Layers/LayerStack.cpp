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
        m_layers.push_back(layer);
    }


    void LayerStack::InsertLayer(ILayer*, int position)
    {

    }


    void LayerStack::RemoveLayer(ILayer* layer)
    {
        // TODO: Iterate through vector, find matching layer pointer and remove from vector
    }


    void LayerStack::AppendOverlay(ILayer* layer)
    {
        m_overlayLayers.push_back(layer);
    }


    void LayerStack::InsertOverlay(ILayer* layer, int position)
    {

    }


    void LayerStack::RemoveOverlay(ILayer* layer)
    {
        // TODO: Iterate through vector, find matching layer pointer and remove from vector
    }


    void LayerStack::DispatchEventBackToFront(Event& event)
    {
        for (ILayer* layer : m_layers)
        {
            if (event.isHandled()) {return;}
            if (!event.isInCategory(layer->GetAcceptingEventFlags())) {continue;}
            layer->OnEvent(event);
        }
        for (ILayer* layer : m_overlayLayers)
        {
            if (event.isHandled()) {return;}
            if (!event.isInCategory(layer->GetAcceptingEventFlags())) {continue;}
            layer->OnEvent(event);
        }
    }


    void LayerStack::DispatchEventFromFrontToBack(Event& event)
    {
        for (ILayer* layer : m_overlayLayers)
        {
            if (event.isHandled()) {return;}
            if (!event.isInCategory(layer->GetAcceptingEventFlags())) {return;}
            layer->OnEvent(event);
        }
        for (ILayer* layer : m_layers)
        {
            if (event.isHandled()) {return;}
            if (!event.isInCategory(layer->GetAcceptingEventFlags())) {return;}
            layer->OnEvent(event);
        }

    }


    void LayerStack::Update()
    {
        for (ILayer* layer : m_overlayLayers)
        {
            layer->OnUpdate();
        }
        for (ILayer* layer : m_layers)
        {
            layer->OnUpdate();
        }
    }


    void LayerStack::RenderImGui()
    {
        for (ILayer* layer : m_layers)
        {
            layer->OnImGuiRender();
        }
        for (ILayer* layer : m_overlayLayers)
        {
            layer->OnImGuiRender();
        }
    }


} // Ayla