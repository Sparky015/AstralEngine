//
// Created by Andrew Fagan on 4/24/24.
//

#pragma once

#include "Layer.h"

#include "Solas/Events/Event.h"

namespace Solas::Core::Layers
{
    // Problem: Systems might have to fight to be able to receive an event of a type that is contested by another system.
    // TODO: Create channels that layers can be in instead of a layer and an "overlay" layer
    // TODO: Refactor how the layer stack organizes layers.
    // Problem: Systems might be updated out of order (i.e. Debug layers update before the input state is updated)
    // Problem: Order of layers updating should be protected to minimize issues.

    /** Stores the all layers in a centralized location for easy access to call Layer's Update and RenderImGui functions */
    class LayerStack
    {
    public:

        /** Constructor/Destructor */
        LayerStack();
        ~LayerStack();

        /** Adds a layer to the layer stack. */
        void AppendLayer(ILayer* layer);

        /** Removes a layer from the layer stack. */
        void RemoveLayer(ILayer* layer);

        /** Dispatches an event to the overlay stack and then the layer stack. */
        void DispatchEventFromFrontToBack(IEvent& event);

        /** Dispatches an event to the layer stack and then the overlay stack. */
        void DispatchEventBackToFront(IEvent& event);

        /** Calls the OnUpdate function on every layer in the layer stack and overlay stack. */
        void Update();

        /** Calls the OnImGuiRender function on every layer in the layer stack and overlay stack. */
        void RenderImGui();


        /** Disallow copy and move constructors and operators. */
        LayerStack(const LayerStack &) = delete;
        LayerStack(LayerStack &&) = delete;
        LayerStack &operator=(const LayerStack &) = delete;
        LayerStack &operator=(LayerStack &&) = delete;

    private:

        /** Stores all the layers */
        std::vector<ILayer*> m_Layers;
    };

} // namespace Solas::Core::Layers
