//
// Created by Andrew Fagan on 7/24/24.
//

#pragma once

#include "EventBus.h"

#include "Debug/Instrumentation/ScopeProfiler.h"

namespace Astral {

    /** Publishes an event to any listeners. */
    template<typename T>
    class EventPublisher
    {
    public:
        /** Increments the Event Bus publisher tracker for lifetime management of the event bus. */
        EventPublisher()
        {
            EventBus<T>& eventBus = EventBus<T>::Get();
            eventBus.IncrementPublisherCount();
        }


        /** Decrements the Event Bus publisher tracker for lifetime management of the event bus. */
        ~EventPublisher()
        {
            EventBus<T>& eventBus = EventBus<T>::Get();
            eventBus.DecrementPublisherCount();
        }


        /** Propagates an event to listeners. */
        void PublishEvent(T event)
        {
            PROFILE_SCOPE("EventPublisher::PublishEvent")
            EventBus<T>& eventBus = EventBus<T>::Get();
            eventBus.RaiseEvent(event);
        }

    };

} // namespace Solas::EventManagement