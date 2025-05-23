//
// Created by Andrew Fagan on 7/24/24.
//

#pragma once

#include "EventBus.h"

namespace Core {

    /** Manages the listener callback with the event bus. */
    template<typename T>
    class EventListener
    {
    public:
        explicit EventListener(std::function<void(T)>&& callback) : m_Callback(std::move(callback)), m_IsListening(false) {};

        ~EventListener()
        {
            StopListening();
        }

        /** Adds the listener callback to the event bus. */
        void StartListening()
        {
            if (m_IsListening) {return;}
            m_IsListening = true;
            EventBus<T>& eventBus = EventBus<T>::Get();
            eventBus.AddListener(&m_Callback);
        };

        /** Removes the listener callback to the event bus. */
        void StopListening()
        {
            if (!m_IsListening) {return;}
            EventBus<T>& eventBus = EventBus<T>::Get();
            eventBus.RemoveListener(&m_Callback);
            m_IsListening = false;
        }

        EventListener(const EventListener&) = delete;
        EventListener& operator=(const EventListener&) = delete;
        EventListener(EventListener&&) = default;
        EventListener& operator=(EventListener&&) = default;

    private:
        std::function<void(T)> m_Callback;
        bool m_IsListening;
    };



} // namespace Event