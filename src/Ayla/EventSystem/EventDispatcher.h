//
// Created by Andrew Fagan on 4/17/24.
//
#pragma once

#include "Ayla/EventSystem/Event.h"


namespace Ayla{

    /**
 * @class EventDispatcher
 * @brief A class used for dispatching event objects.
 *
 * This class contains all of the necessary methods and variables to enqueue,
 * dequeue, and process Event class objects.
 */
    class EventDispatcher {
    public:

        /**
         * @brief Add an event to the event queue.
         *
         * @param event The event to add to the queue.
         */
        static void addEventToQueue(Event &);

        /**
         * @brief Dispatch all events currently in the event queue.
         */
        void dispatchAllEvents();

        /**
         * @brief Dispatch a blocking event.
         *
         * @param event The event to be dispatched.
         */
        void dispatchBlockingEvent(Event &);

    private:

        /**
         * @brief Take an event from the event queue.
         *
         * @return A shared_ptr to the event taken from the queue.
         */
        static std::shared_ptr <Event> takeEventFromQueue();

        /**
         * @brief Dispatch a single event.
         *
         * @param event A shared_ptr to the event to be dispatched.
         */
        void dispatchEvent(const std::shared_ptr <Event> &);

        /**
         * @brief Poll an event.
         *
         * @param event The event to poll.
         *
         * @return The result of the poll operation.
         */
        int PollEvent(Event &);

        /**
         * @brief The event queue.
         */
        static std::queue <std::shared_ptr<Event>> eventQueue;
    };
}
