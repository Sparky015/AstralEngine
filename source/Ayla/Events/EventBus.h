//
// Created by Andrew Fagan on 7/23/24.
//

#pragma once

namespace Ayla::EventManagement {

    /** Holds function callbacks for when an event needs to be propagated. */
    template<typename T>
    class EventBus
    {
    public:

        /** Retrieves the event bus or creates (and returns) it on the first call */
        static EventBus<T>& Get()
        {
            if (m_Instance == nullptr)
            {
                AY_TRACE("Creating new type of EventBus!");
                m_Instance = new EventBus<T>;
            }
            return *m_Instance;
        }


        /** Deletes the singleton instance of the type of Eventbus */
        static void Destroy()
        {
            AY_TRACE("Destroying a type of EventBus!");
            delete m_Instance;
            m_Instance = nullptr;
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
        void RaiseEvent(T& event)
        {
            for (std::function<void(T)>* callback : m_Callbacks)
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

        static EventBus<T>* m_Instance;
        std::vector<std::function<void(T)>*> m_Callbacks;

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

    template<typename T>
    EventBus<T>* EventBus<T>::m_Instance = nullptr;

} // namespace Ayla::EventManagement
