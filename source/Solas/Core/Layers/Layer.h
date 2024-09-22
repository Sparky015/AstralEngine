//
// Created by Andrew Fagan on 4/24/24.
//

#pragma once
#include "Solas/Events/Event.h"



namespace Ayla::Core::Layers
{
    using namespace Ayla::EventManagement;
    /**
     * Allows systems a way to receive and send data to each other through events without explicitly linking to each other.
     * @note Systems own the layer that they operate.
     */
    class ILayer
    {
    public:
        ILayer();
        virtual ~ILayer();

        /** Returns a layer or systems event flags that it wants to receive in order to filter unwanted events out. */
        virtual EEventCategory GetAcceptingEventFlags() = 0;

        /** Executes the function code when a layer is added to the Layer Stack for a system to initialize anything before the layer's update function gets called. */
        virtual void OnAttach();

        /** Executes the function code when a layer is removed from the Layer Stack to allow a system to destruct anything needed before the system/layer will stop being updated. */
        virtual void OnDetach();

        /** Executes the function code every tick (occurs every time the engine completes a loop) to give a system the chance to run anything it needs to. */
        virtual void OnUpdate();

        /** Allows a system to interact with and create ImGui windows. */
        virtual void OnImGuiRender();

        /** Executes the function code whenever a system receives an event to give a chance for a system or layer to store or act on any data received from the event. */
        virtual void OnEvent(IEvent& event);

        /** Adds the layer as a layer to the Layer Stack. */
        void AttachLayer();

        /** Removes the layer as a layer from the Layer Stack. */
        void DetachLayer();

        /** Allows a system to set a callback from the layer in order for systems to be able to react to events. */
        void SetCallback(std::function<void(IEvent&)> callback);

        /** Allows for a check to see if the layer is enabled */
        [[nodiscard]] bool IsEnabled() const;

        /** Enables the layer in the Layer Stack (it will start receiving events and calls to the Update function) */
        void Enable();

        /** Disables the layer in the Layer Stack (it will stop receiving events and calls to the Update function) */
        void Disable();

        enum EventDestination : uint8
        {
            FRONT_TO_BACK,
            BACK_TO_FRONT,
        };

        /** Allows a system to send events to the Layer Stack to be sent to other systems. */
        void SendEvent(IEvent& event, EventDestination eventDestination) const;

    protected:

        bool m_IsEnabled;
        bool m_IsInitializedInTower;
        std::function<void(IEvent & )> m_Callback = [](IEvent&){};

        // For debugging purposes
        std::string m_DebugName;
    };

} // namespace Ayla::Core::Layers
