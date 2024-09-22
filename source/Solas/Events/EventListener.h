//
// Created by Andrew Fagan on 7/24/24.
//

#pragma once

#include "Solas/Events/EventBus.h"

namespace Ayla::EventManagement {

    /** Manages the listener callback with the event bus. */
    template<typename T>
    class EventListener
    {
    public:

        /** Adds the listener callback to the event bus. */
        explicit EventListener(std::function<void(T&)>&& callback) : m_Callback(std::move(callback))
        {
            EventBus<T>& eventBus = EventBus<T>::Get();
            eventBus.AddListener(&m_Callback);
        }

        /** Removes the listener callback to the event bus. */
        ~EventListener()
        {
            EventBus<T>& eventBus = EventBus<T>::Get();
            eventBus.RemoveListener(&m_Callback);
        }

    private:
        std::function<void(T&)> m_Callback;
    };

} // namespace Ayla::EventManagement