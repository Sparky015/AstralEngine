//
// Created by Andrew Fagan on 7/23/24.
//

#pragma once

#include "Core/CoreMacroDefinitions.h"
#include <algorithm>
#include <functional>

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
            if (m_IsEventBeingRaised)
            {
                m_TempCallbackBuffer.push_back(callback);
            }
            else
            {
                m_Callbacks.push_back(callback);
            }
            IncrementListenerCount();
        }


        /**@brief Removes a listener from the callback list. */
        void RemoveListener(std::function<void(T)>* callback)
        {
            if (m_IsEventBeingRaised)
            {
                // Check if the callback is present in the vector before erasing
                auto tempEnd = std::remove(m_TempCallbackBuffer.begin(), m_TempCallbackBuffer.end(), callback);

                if (tempEnd != m_TempCallbackBuffer.end())
                {
                    m_TempCallbackBuffer.erase(tempEnd, m_TempCallbackBuffer.end());
                    DecrementListenerCount();
                }
            }
            else
            {
                // Check if the callback is present in the vector before erasing
                auto callbacksEnd = std::remove(m_Callbacks.begin(), m_Callbacks.end(), callback);

                if (callbacksEnd != m_Callbacks.end())
                {
                    m_Callbacks.erase(callbacksEnd, m_Callbacks.end());
                    DecrementListenerCount();
                }
            }
        }


        /**@brief Takes an event and propagates it to listeners. */
        void RaiseEvent(const T& event)
        {
            m_IsEventBeingRaised = true;

            for (const std::function<void(T)>* callback : m_Callbacks)
            {
                (*callback)(event);
            }

            m_IsEventBeingRaised = false;

            // Add the callbacks that were added during the event being raised.
            // This avoids the callback vector being resized when a new listener
            // is added during the event being raised. (If the vector resizes during calling
            // of the callbacks, it will cause the current iterator to point to freed memory resulting in a
            // crash)
            for (std::function<void(T)>* callback : m_TempCallbackBuffer)
            {
                m_Callbacks.push_back(callback);
            }
            m_TempCallbackBuffer.clear();
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

        bool m_IsEventBeingRaised{false};
        std::vector<std::function<void(T)>*> m_TempCallbackBuffer;

        std::vector<std::function<void(T)>*> m_Callbacks;

        inline void IncrementListenerCount() { m_NumberOfListeners++; }
        inline void DecrementListenerCount() { m_NumberOfListeners--; }
    };

} // namespace Solas::EventManagement
