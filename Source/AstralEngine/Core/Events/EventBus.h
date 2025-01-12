//
// Created by Andrew Fagan on 7/23/24.
//

#pragma once

#include "pch.h"

namespace Core {

    /** @brief Holds function callbacks for when an event needs to be propagated. */
    template<typename T>
    class EventBus
    {
    public:

        /** Retrieves the event bus or creates (and returns) it on the first call */
        static EventBus<T>& Get()
        {
            static EventBus<T> m_Instance;
            return m_Instance;
        }

        /**@brief Adds a listener to the callback list. */
        void AddListener(std::function<void(T)>* callback)
        {
            m_Callbacks.push_back(callback);
            IncrementListenerCount();
        }


        /**@brief Removes a listener from the callback list. */
        void RemoveListener(std::function<void(T)>* callback)
        {
            m_Callbacks.erase(std::remove(m_Callbacks.begin(), m_Callbacks.end(), callback), m_Callbacks.end());
            DecrementListenerCount();
        }


        /**@brief Takes an event and propagates it to listeners. */
        void RaiseEvent(const T& event)
        {
            for (const std::function<void(T)>* callback : m_Callbacks)
            {
                (*callback)(event);
            }
        }

        /**@brief Increments the publisher count for event type */
        inline void IncrementPublisherCount() { m_NumberOfPublishers++; }

        /**@brief Decrements the publisher count for event type */
        inline void DecrementPublisherCount() { m_NumberOfPublishers--; }

        /**@brief Gets the publisher count for event type */
        [[nodiscard]] inline uint16 GetNumberOfPublishers() const { return m_NumberOfPublishers; }

        /**@brief Gets the listener count for event type */
        [[nodiscard]] inline uint16 GetNumberOfListeners() const { return m_NumberOfListeners; }


        EventBus(EventBus&&) = delete;
        EventBus operator=(EventBus&&) = delete;
        EventBus(const EventBus&) = delete;
        EventBus& operator=(const EventBus&) = delete;
    private:

        EventBus() = default;
        ~EventBus() = default;

        uint16 m_NumberOfListeners{};
        uint16 m_NumberOfPublishers{};

        std::vector<std::function<void(T)>*> m_Callbacks;

        inline void IncrementListenerCount() { m_NumberOfListeners++; }
        inline void DecrementListenerCount() { m_NumberOfListeners--; }
    };

} // namespace Solas::EventManagement
