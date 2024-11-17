//
// Created by Andrew Fagan on 7/23/24.
//

#pragma once

#include "pch.h"

namespace Event {

    /** @brief Holds function callbacks for when an event needs to be propagated. */
    template<typename T>
    class EventBus
    {
    public:

        /** Retrieves the event bus or creates (and returns) it on the first call */
        static EventBus<T>& Get()
        {
            if (!m_IsInitialized)
            {
                m_IsInitialized = true;
                //TRACE("Creating " << typeid(T).name() << " EventBus! | Event Object Size: "  << sizeof(T) << " byte(s)");
            }

            static EventBus<T> m_Instance;
            return m_Instance;
        }

        /** Deletes the singleton instance of the type of Eventbus */
        static void Destroy()
        {
            //TRACE(typeid(T).name() << "EventBus has no listeners! | Event Object Size: " << sizeof(T) << " byte(s)");
        }


        /** Adds a listener to the callback list. */
        void AddListener(std::function<void(T)>* callback)
        {
            m_Callbacks.push_back(callback);
            IncrementListenerCount();
        }


        /** Removes a listener from the callback list. */
        void RemoveListener(std::function<void(T)>* callback)
        {
            m_Callbacks.erase(std::remove(m_Callbacks.begin(), m_Callbacks.end(), callback), m_Callbacks.end());
            DecrementListenerCount();
        }


        /** Takes an event and propagates it to listeners. */
        void RaiseEvent(const T& event)
        {
            for (const std::function<void(T)>* callback : m_Callbacks)
            {
                (*callback)(event);
            }
        }


        inline void IncrementPublisherCount() {m_NumberOfPublishers++;}
        inline void DecrementPublisherCount()
        {
            m_NumberOfPublishers--;
            if (m_NumberOfPublishers == 0 && m_NumberOfListeners == 0)
            {
                /** Deletes the static instance if there are no active publishers and listeners. */
                Destroy();
            }
        }

        EventBus(EventBus&&) = delete;
        EventBus operator=(EventBus&&) = delete;
        EventBus(const EventBus&) = delete;
        EventBus& operator=(const EventBus&) = delete;
    private:

        EventBus<T>() = default;
        ~EventBus<T>() = default;

        std::vector<std::function<void(T)>*> m_Callbacks;
        static bool m_IsInitialized;

        uint16 m_NumberOfListeners{0};
        uint16 m_NumberOfPublishers{0};

        inline void IncrementListenerCount() {m_NumberOfListeners++;}
        inline void DecrementListenerCount()
        {
            m_NumberOfListeners--;
            if (m_NumberOfListeners == 0 && m_NumberOfPublishers == 0)
            {
                /** Deletes the static instance if there are no active publishers and listeners. */
                Destroy();
            }
        }
    };

    template <typename T>
    bool EventBus<T>::m_IsInitialized = false;


} // namespace Solas::EventManagement
