//
// Created by Andrew Fagan on 4/24/24.
//

#pragma once
#include "Ayla/Events/Event.h"

using namespace Ayla::Events;

namespace Ayla::Core::Layers
{

    class ILayer
    {
    public:
        ILayer();
        virtual ~ILayer();

        /**  */
        virtual void OnAttach() = 0;

        /**  */
        virtual void OnDetach() = 0;

        /**  */
        virtual void OnUpdate() = 0;

        /**  */
        virtual void OnImGuiRender() = 0;

        /**  */
        virtual void OnEvent(Event& event) = 0;

        /**  */
        virtual EventCategory GetAcceptingEventFlags() = 0;

        /**  */
        void AttachLayer();

        /**  */
        void AttachOverlay();

        /**  */
        void DetachLayer();

        /**  */
        void DetachOverlay();

        /**  */
        void SetCallback(std::function<void(Event&)> callback);

        /**  */
        bool IsEnabled();

        /**  */
        void Enable();

        /**  */
        void Disable();

    protected:

        bool m_IsEnabled;
        bool m_IsInitializedInTower;
        std::function<void(Event & )> m_Callback = [](Event&){};

        // For debugging purposes
        std::string m_DebugName;

    };

}
