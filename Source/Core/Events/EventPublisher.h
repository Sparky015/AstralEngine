//
// Created by Andrew Fagan on 7/24/24.
//

#pragma once

#include "EventBus.h"

namespace Event {

    /** Publishes an event to any listeners. Also, helps manage the lifetime of event buses as
     * when there are no event publishers and event listeners, the event bus is destroyed. */
    template<typename T>
    class EventPublisher
    {
    public:
        /** Increments the Event Bus publisher tracker for lifetime management of the event bus. */
        EventPublisher()
        {
            PROFILE_SCOPE();
            EventBus<T>& eventBus = EventBus<T>::Get();
            eventBus.IncrementPublisherCount();
        }


        /** Decrements the Event Bus publisher tracker for lifetime management of the event bus. */
        ~EventPublisher()
        {
            PROFILE_SCOPE();
            EventBus<T>& eventBus = EventBus<T>::Get();
            eventBus.DecrementPublisherCount();
        }


        /** Propagates an event to listeners. */
        void PublishEvent(T event)
        {
            PROFILE_SCOPE();
            EventBus<T>& eventBus = EventBus<T>::Get();
            eventBus.RaiseEvent(event);
        }

    };

} // namespace Solas::EventManagement