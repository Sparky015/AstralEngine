//
// Created by Andrew Fagan on 4/24/24.
//

#pragma once
#include "Layer.h"
#include "Ayla/Events/Event.h"

namespace Ayla::Core::Layers
{
    // TODO: Refactor how the layer stack organizes layers.

    /** Stores the all layers in a centralized location for easy access to call Layer's Update and RenderImGui functions */
    class LayerStack
    {
    public:
        LayerStack();
        ~LayerStack();

        /**  */
        void AppendLayer(ILayer* layer);

        // Problem: Systems might have to fight to be able to receive an event of a type that is contested by another system.
        // TODO: Create channels that layers can be in instead of a layer and an "overlay" layer
        void InsertLayer(ILayer*, int position);

        // Problem: Systems might be updated out of order (i.e. Debug layers update before the input state is updated)
        // Problem: Order of layers updating should be protected to minimize issues.
        /**  */
        void RemoveLayer(ILayer* layer);

        /**  */
        void AppendOverlay(ILayer* layer);

        /**  */
        void InsertOverlay(ILayer* layer, int position);

        /**  */
        void RemoveOverlay(ILayer* layer);

        /**  */
        void DispatchEventFromFrontToBack(IEvent& event);

        /**  */
        void DispatchEventBackToFront(IEvent& event);

        /**  */
        void Update();

        /**  */
        void RenderImGui();


        // Disallow copy and move constructors and operators.
        LayerStack(const LayerStack &) = delete;
        LayerStack(LayerStack &&) = delete;
        LayerStack &operator=(const LayerStack &) = delete;
        LayerStack &operator=(LayerStack &&) = delete;

    private:

        /** Stores all the layers */
        std::vector<ILayer *> m_Layers;

        /** Stores all the overlays */
        std::vector<ILayer *> m_OverlayLayers;
    };

} // namespace Ayla::Core::Layers
